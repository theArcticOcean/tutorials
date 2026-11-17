#include "worker.h"
#include "Log.hpp"
#include <cmath>
#include <cstddef>
#include <chrono>
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

// 全局任务编号：多线程派发时保证 taskId 唯一
static std::atomic<int> g_taskIdCounter(0);
const std::size_t g_ElemCount = 100*1024*1024; // X MB elements
// malloc in heap memory
const int g_AllocCount = 15;

// 单个任务：批量申请/释放堆内存（每个任务运行在一个独立线程中）
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
        //     // emsdk 说明：new[] 申请的内存只能由 delete[] 释放；
        //     // emscripten_free()/emscripten_builtin_free() 只适用于 malloc 出来的指针，混用是未定义行为
        //     for (int k = 0; k < j; k++) {
        //         delete[] buffers[k];
        //         buffers[k] = nullptr;   // 统一释放时避免二次 delete
        //     }
        //     // 把 mimalloc 的空闲页归还给 wasm 堆，后续大块申请可直接复用而不必继续增长堆
        //     mi_collect(true);
        //     Log(IInfo, "XT-mem taskId ", taskId, " released ", j, " buffers, heap: ", (int)(emscripten_get_heap_size() / (1024 * 1024)), " MB, heapMax: ", (int)(emscripten_get_heap_max() / (1024 * 1024)), " MB.");

        //     continue;
        // }

        buffers[j] = new bool[g_ElemCount];

        buffers[j][0] = taskId%2 == 0;
        buffers[j][g_ElemCount - 1] = taskId%2 != 0;

        Log(IInfo, "XT-mem taskId ", taskId, " After allocation and deallocation: ", (int)(emscripten_get_heap_size() / (1024 * 1024)), " MB");

    }

    // 统一释放本任务申请的所有块
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

    // for (int i = 0; i < len; i++) {
    //     const int newTaskId = g_taskIdCounter.fetch_add(1) + taskId * len; // 保证每个任务的 taskId 唯一
    //     AllocHeapTask(  newTaskId );
    // }

    // 每一个任务交给一个独立的线程处理
    std::vector<std::thread> threads;
    int batchSize = 1;
    for (int i = 0; i < len; i += batchSize) {
        const int newTaskId = g_taskIdCounter.fetch_add(1) + taskId * len; // 保证每个任务的 taskId 唯一
        threads.push_back(std::thread( AllocHeapTask, newTaskId ));
    }
    for (auto& thread : threads) {
        thread.join();
    }
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
    
    // Create and detach the thread to run asynchronously
    for(  int i = 0; i < 2; i++ )
    {
        std::thread subThread(task_wrapper, i);
        subThread.detach();
    }
    Log(IInfo, "Complex task thread started, main thread continues...");
}