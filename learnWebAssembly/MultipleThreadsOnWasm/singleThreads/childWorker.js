let count = 0;
let isResourceLimited = false;
let memoryLeakArray = []; // 用于存储大量数据

// 检查资源使用情况
function checkResources() {
    try {
        const memory = performance.memory;
        if (memory) {
            const usedHeap = memory.usedJSHeapSize;
            const heapLimit = memory.jsHeapSizeLimit;
            
            if (usedHeap / heapLimit > 0.8) {
                postMessage({
                    type: 'resourceLimit',
                    message: 'Memory usage too high',
                    usage: usedHeap,
                    limit: heapLimit
                });
                isResourceLimited = true;
                return false;
            }
        }
        return true;
    } catch (e) {
        return true; // 如果无法获取内存信息，继续执行
    }
}

// 执行计算任务
function performCalculations() {
    if (isResourceLimited || !checkResources()) {
        return null;
    }

    let result = 0;
    try {
        // const dataLen = 170000*1;
        // // 创建大量内存占用
        // let largeObject = new Array(dataLen).fill(0).map(() => ({
        //     data: new Float64Array(1000),
        //     string: 'x'.repeat(10000),
        //     array: new Array(1000).fill(Math.random()),
        //     date: new Date(),
        //     objects: new Array(100).fill(null).map(() => ({
        //         nested: new Array(100).fill(Math.random())
        //     }))
        // }));
        
        // // 使用完数据后进行释放
        // const processData = () => {
        //     memoryLeakArray.push(...largeObject);
        //     // 清理 largeObject
        //     largeObject.forEach(item => {
        //         item.data = null;
        //         item.array = null;
        //         item.objects = null;
        //     });
        //     largeObject = null;
            
        //     // 定期清理 memoryLeakArray
        //     if (memoryLeakArray.length > 5000000) {
        //         memoryLeakArray.splice(0, 1000000);
        //     }
        // };

        // processData();

        // 原有的计算任务
        const len = 1000;
        for (let i = 0; i < len; i++) {
            result += Math.sqrt(i) * Math.sin(i);
            for (let j = 0; j < len*50; j++) {
                result += Math.sqrt(j) * Math.sin(j);
            }
        }
        return result;
    } catch (e) {
        postMessage({
            type: 'error',
            message: `Calculation error: ${e.message}`
        });
        return null;
    }
}

// 定期执行任务并报告结果
const intervalId = setInterval(() => {
    if (isResourceLimited) {
        clearInterval(intervalId);
        return;
    }

    count++;
    const result = performCalculations();
    if (result !== null) {
        postMessage({
            type: 'calculation',
            iteration: count,
            result: result
        });
    }
}, 5000);

// 接收消息
onmessage = function(e) {
    postMessage({
        type: 'info',
        message: `Child worker ${e.data.id} started`
    });
};