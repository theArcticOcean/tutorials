// VTK WebAssembly Worker
// 在独立线程中加载和初始化WASM模块

let wasmModule = null;
let workerObj = null;
let isInitialized = false;

// 导入WASM模块
importScripts('./vtkWasmInWebWorker.js');

// 监听主线程消息
self.onmessage = function(e) {
    const { type, data } = e.data;
    
    switch(type) {
        case 'INIT_WASM':
            initializeWasm(data);
            break;
        case 'START_RENDERING':
            startRendering();
            break;
        case 'UPDATE_SCENE':
            updateScene();
            break;
        case 'MOUSE_EVENT':
            handleMouseEvent(data);
            break;
        default:
            console.warn('Unknown message type:', type);
    }
};

// 初始化WASM模块
function initializeWasm(config) {
    try {
        console.log('Worker: 开始初始化WASM模块...');
        
        // 配置WASM模块
        const Module = {
            canvas: config.canvas || null,
            onRuntimeInitialized: function() {
                try {
                    console.log('Worker: WASM运行时初始化完成');
                    
                    // 创建Worker对象
                    workerObj = new Module.Worker();
                    workerObj.Init();
                    isInitialized = true;
                    
                    // 通知主线程初始化完成
                    self.postMessage({
                        type: 'WASM_INITIALIZED',
                        success: true
                    });
                    
                    console.log('Worker: VTK Worker对象创建并初始化完成');
                } catch (error) {
                    console.error('Worker: 初始化VTK Worker对象失败:', error);
                    self.postMessage({
                        type: 'WASM_INITIALIZED',
                        success: false,
                        error: error.message
                    });
                }
            },
            onAbort: function(what) {
                console.error('Worker: WASM模块中止:', what);
                self.postMessage({
                    type: 'WASM_ERROR',
                    error: 'WASM模块中止: ' + what
                });
            }
        };
        
        // 创建WASM模块实例
        wasmModule = createModule(Module);
        
    } catch (error) {
        console.error('Worker: 初始化WASM失败:', error);
        self.postMessage({
            type: 'WASM_INITIALIZED',
            success: false,
            error: error.message
        });
    }
}

// 开始渲染
function startRendering() {
    if (!isInitialized || !workerObj) {
        console.error('Worker: WASM模块未初始化，无法开始渲染');
        return;
    }
    
    try {
        console.log('Worker: 开始渲染...');
        workerObj.Start();
        
        self.postMessage({
            type: 'RENDERING_STARTED',
            success: true
        });
    } catch (error) {
        console.error('Worker: 开始渲染失败:', error);
        self.postMessage({
            type: 'RENDERING_STARTED',
            success: false,
            error: error.message
        });
    }
}

// 更新场景
function updateScene() {
    if (!isInitialized || !workerObj) {
        console.warn('Worker: WASM模块未初始化，无法更新场景');
        return;
    }
    
    try {
        // 这里可以添加场景更新逻辑
        console.log('Worker: 场景更新');
        
        self.postMessage({
            type: 'SCENE_UPDATED',
            success: true
        });
    } catch (error) {
        console.error('Worker: 更新场景失败:', error);
        self.postMessage({
            type: 'SCENE_UPDATED',
            success: false,
            error: error.message
        });
    }
}

// 处理鼠标事件
function handleMouseEvent(eventData) {
    if (!isInitialized || !workerObj) {
        return;
    }
    
    try {
        // 根据事件类型处理鼠标事件
        switch(eventData.eventType) {
            case 'mousedown':
                if (eventData.button === 0) { // 左键
                    workerObj.OnLeftButtonDown();
                }
                break;
            // 可以添加更多鼠标事件处理
        }
        
        self.postMessage({
            type: 'MOUSE_EVENT_HANDLED',
            success: true
        });
    } catch (error) {
        console.error('Worker: 处理鼠标事件失败:', error);
        self.postMessage({
            type: 'MOUSE_EVENT_HANDLED',
            success: false,
            error: error.message
        });
    }
}

// 错误处理
self.onerror = function(error) {
    console.error('Worker: 发生错误:', error);
    self.postMessage({
        type: 'WORKER_ERROR',
        error: error.message || '未知错误'
    });
};

console.log('Worker: VTK WebAssembly Worker已加载');