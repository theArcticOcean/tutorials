#include "worker.h"
#include "Log.hpp"
#include <cmath>
#include <cstddef>
#include <chrono>
#include <vector>
#include <unistd.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/threading.h>
#include <emscripten/heap.h>
#include <emscripten/emmalloc.h>
#include <mimalloc.h>

// Check memory fragmentation
static bool NeedMimallocFreeSpace(int taskId, size_t neededBytes)
{
    size_t bucket[32];
    const size_t regionCount = emmalloc_compute_free_dynamic_memory_fragmentation_map(bucket);

    int topIdx = -1;
    for (int i = 31; i >= 0; --i)
    {
        if (bucket[i] > 0)
        {
            topIdx = i;
            break;
        }
    }
    const size_t largestContinuousSpace = topIdx >= 0 ? (static_cast<size_t>(1) << topIdx) : 0;

    // use __builtin_clz to count leading zeros
    int needIdx = 0;
    if (neededBytes > 0)
    {
        needIdx = 31 - __builtin_clz(static_cast<unsigned int>(neededBytes));
    }
    const bool enoughContiguousSpace = topIdx >= needIdx;

    Log(IInfo, "XT-mem taskId ", taskId, " , enoughContiguousSpace: ", enoughContiguousSpace, ", ",
        "largestContinuousSpace: ", (int)(largestContinuousSpace / (1024 * 1024)), " MB, ",
        "free regions count: ", regionCount, ", need: ", (int)(neededBytes / (1024 * 1024)), " MB");

    return !enoughContiguousSpace;
}

#endif

// Global task id counter: keeps taskId unique when tasks are dispatched from multiple threads
static std::atomic<int> g_taskIdCounter(0);
const std::size_t g_ElemCount = 100*1024*1024; // X MB elements
// malloc in heap memory
const int g_AllocCount = 15;

// A single task: batch allocate/free heap memory (each task runs in its own thread)
static void AllocHeapTask(int taskId)
{
#ifdef __EMSCRIPTEN__
    emscripten_run_script("console.log('Complex task started in std::thread web worker');");
#else
    Log(IInfo, "Complex task started in std::thread web worker");
#endif

    bool** buffers = new bool*[g_AllocCount]();

    for (int j = 0; j < g_AllocCount; j++) {
        //bool needMimallocFreeSpace = NeedMimallocFreeSpace(taskId, count * sizeof(bool));

        unsigned long long heapSize = emscripten_get_heap_size();
        unsigned long long heapMax = emscripten_get_heap_max();
        unsigned long long needSize = g_ElemCount * sizeof(bool);

        bool needMimallocFreeSpace = (heapSize + needSize) > heapMax ? true : false;

        Log( IInfo, "XT-mem taskId ", taskId, " Before allocation: heapSize: ", (int)(heapSize / (1024 * 1024)), " MB, heapMax: ", (int)(heapMax / (1024 * 1024)), " MB, needSize: ", (int)(needSize / (1024 * 1024)), " MB, needMimallocFreeSpace: ", needMimallocFreeSpace );

        // if( needMimallocFreeSpace )
        // {
        //     // emsdk note: memory allocated with new[] must be released with delete[];
        //     // emscripten_free()/emscripten_builtin_free() only accept pointers from malloc, mixing them is undefined behavior
        //     for (int k = 0; k < j; k++) {
        //         delete[] buffers[k];
        //         buffers[k] = nullptr;   // avoid a double delete in the final cleanup
        //     }
        //     // Return mimalloc's free pages to the wasm heap, so later large allocations can be reused without growing the heap
        //     mi_collect(true);
        //     Log(IInfo, "XT-mem taskId ", taskId, " released ", j, " buffers, heap: ", (int)(emscripten_get_heap_size() / (1024 * 1024)), " MB, heapMax: ", (int)(emscripten_get_heap_max() / (1024 * 1024)), " MB.");

        //     continue;
        // }

        buffers[j] = new bool[g_ElemCount];

        buffers[j][0] = taskId%2 == 0;
        buffers[j][g_ElemCount - 1] = taskId%2 != 0;

        Log(IInfo, "XT-mem taskId ", taskId, " After allocation and deallocation: ", (int)(emscripten_get_heap_size() / (1024 * 1024)), " MB");

    }

    // Release all the blocks allocated by this task
    for (int j = 0; j < g_AllocCount; j++) {
        delete[] buffers[j];
    }
    delete[] buffers;

#ifdef __EMSCRIPTEN__
    emscripten_run_script("console.log('Complex task completed in std::thread web worker');");
#else
    Log(IInfo, "Complex task completed in std::thread web worker");
#endif
}

// C function for complex task in thread
void complex_task_thread_func(int len, int taskId)
{
    Log(IInfo, "XT-mem parent taskId ", taskId, " dispatching ", len, " tasks, one thread per task");

    // Heap memory needed by a single task: g_AllocCount blocks of g_ElemCount bools each
    const unsigned long long taskBytes = (unsigned long long)g_AllocCount * (unsigned long long)g_ElemCount * sizeof(bool);

    int dispatched = 0;
    while (dispatched < len)
    {
        // Available memory = still growable (wasm heap max - current size) + already free inside emmalloc
        const unsigned long long heapSize = emscripten_get_heap_size();
        const unsigned long long heapMax = emscripten_get_heap_max();
        const unsigned long long growRoom = (heapMax > heapSize) ? (heapMax - heapSize) : 0ULL;
        const unsigned long long freeMem = emmalloc_free_dynamic_memory();  // slow: walks all free blocks linearly, called once per batch boundary
        const unsigned long long available = growRoom + freeMem;

        // Keep 20% headroom, decide how many tasks run concurrently in this batch, falls back to 1 when memory is tight
        unsigned long long maxTasks = (available * 8 / 10) / taskBytes;
        if (maxTasks < 1) maxTasks = 1;
        const unsigned long long remain = (unsigned long long)(len - dispatched);
        if (maxTasks > remain) maxTasks = remain;
        const int batchSize = (int)maxTasks;

        Log(IInfo, "XT-mem parent taskId ", taskId, " batch: ", batchSize, " tasks, available: ", (int)(available / (1024 * 1024)), " MB (grow: ", (int)(growRoom / (1024 * 1024)), " MB, free: ", (int)(freeMem / (1024 * 1024)), " MB), per task: ", (int)(taskBytes / (1024 * 1024)), " MB");

        // Each task is handled by an independent thread
        std::vector<std::thread> threads;
        threads.reserve(batchSize);
        for (int i = 0; i < batchSize; i++) {
            const int newTaskId = g_taskIdCounter.fetch_add(1) + taskId * len; // keep taskId unique for every task
            threads.push_back(std::thread( AllocHeapTask, newTaskId ));
        }
        for (auto& thread : threads) {
            thread.join();
        }

        dispatched += batchSize;
    }

    // std::vector<std::thread> threads;
    // for (int i = 0; i < len; i++) {
    //     const int newTaskId = g_taskIdCounter.fetch_add(1) + taskId * len; // keep taskId unique for every task
    //     threads.push_back(std::thread( AllocHeapTask, newTaskId ));
    // }
    // for (auto& thread : threads) {
    //     thread.join();
    // }
}

Worker::Worker()
{
#ifdef __EMSCRIPTEN__
    #if VTK_MAJOR_VERSION <=9 && VTK_MINOR_VERSION < 3
        m_RenderWindowInteractor = vtkSPtr<vtkSDL2RenderWindowInteractor>::New();
        m_RenderWindow = vtkSPtr<vtkSDL2OpenGLRenderWindow>::New();
    #else
        m_RenderWindowInteractor = vtkSPtr<vtkWebAssemblyRenderWindowInteractor>::New();
        m_RenderWindow = vtkSPtr<vtkWebAssemblyOpenGLRenderWindow>::New();
    #endif

#else
    m_RenderWindowInteractor = vtkSPtr<vtkRenderWindowInteractor>::New();
    m_RenderWindow = vtkSPtr<vtkRenderWindow>::New();
#endif
    //m_SubThread = nullptr;
}

Worker::~Worker()
{

}

void Worker::Init()
{
    vtkSPtrNew( cone, vtkConeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cone->GetOutputPort() );

    m_Actor = vtkSPtr<vtkActor>::New();
    m_Actor->SetMapper( mapper );

    m_Renderer = vtkSPtr<vtkRenderer>::New();
    m_Renderer->SetBackground( 0, 0.2, 0 );
    m_Renderer->SetLayer( 0 );
    m_Renderer->AddActor(m_Actor);

    m_RenderWindow->AddRenderer( m_Renderer );
    m_RenderWindowInteractor->SetRenderWindow( m_RenderWindow );

    m_Renderer->ResetCamera();
    m_RenderWindow->Render();

    Log( IInfo, "Init!" );
}

void Worker::Start()
{
#ifdef __EMSCRIPTEN__
    emscripten_run_script( "console.log('hello world before start')" );
#endif
    m_RenderWindowInteractor->Start();

    Log( IInfo, "Start!" );
}

static void taskFinishCallback() {
    mi_collect(true);
    emscripten_run_script("OnTaskFinished();");
}

void Worker::SimuComplexTaskAsync()
{
    Log(IInfo, "Starting complex task in web worker thread using std::thread...");
    
    const int len = 10;
    
    // Create a lambda function to wrap the task with timing and logging
    auto task_wrapper = [len](int taskId) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        complex_task_thread_func(len, taskId);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        Log(IInfo, "Complex task duration: " + std::to_string(duration.count()) + " ms");
        Log(IInfo, "Complex task in web worker thread using std::thread finished!");

        //emscripten_run_script( "OnTaskFinished();" );
#ifdef __EMSCRIPTEN__
        emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_V, taskFinishCallback); // API from emscripten/threading.h
#endif
    };
    
    std::thread subThread(task_wrapper, 0);
    subThread.detach();

    Log(IInfo, "Complex task thread started, main thread continues...");
}