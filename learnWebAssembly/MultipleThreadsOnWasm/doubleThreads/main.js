const PARENT_WORKERS = 10;
const parentWorkers = [];
let totalWorkers = 0;
const childWorkerCounts = new Map(); // 存储每个父 Worker 的子 Worker 数量
const maxWorkersLimit = navigator.hardwareConcurrency * 4;

function updateTotalWorkers() {
    const totalChildWorkers = Array.from(childWorkerCounts.values()).reduce((sum, count) => sum + count, 0);
    totalWorkers = parentWorkers.length + totalChildWorkers;
    return totalWorkers;
}

function checkWorkerLimit(count) {
    const memoryUsage = window.performance?.memory?.usedJSHeapSize;
    const memoryLimit = window.performance?.memory?.jsHeapSizeLimit;
    
    const totalChildWorkers = Array.from(childWorkerCounts.values()).reduce((sum, count) => sum + count, 0);
    updateStatus(`Parent workers: ${parentWorkers.length}`);
    updateStatus(`Child workers: ${totalChildWorkers}`);
    updateStatus(`Total workers: ${updateTotalWorkers()}`);
    updateStatus(`CPU cores: ${navigator.hardwareConcurrency}`);
    
    if (memoryUsage && memoryLimit) {
        const memoryPercentage = (memoryUsage / memoryLimit) * 100;
        updateStatus(`Memory usage: ${memoryPercentage.toFixed(2)}%`);
    }

    if (totalWorkers >= maxWorkersLimit) {
        updateStatus('WARNING: Reached maximum worker limit!');
        return false;
    }
    
    return true;
}

function updateStatus(message) {
    console.log(message);

    const statusDiv = document.getElementById('status');
    const timestamp = new Date().toLocaleTimeString();
    const logEntry = document.createElement('div');
    logEntry.textContent = `[${timestamp}] ${message}`;
    statusDiv.appendChild(logEntry);

    // 保持最新的消息可见
    statusDiv.scrollTop = statusDiv.scrollHeight;
}

// 创建父级 workers
for (let i = 0; i < PARENT_WORKERS; i++) {
    if (!checkWorkerLimit(totalWorkers)) {
        updateStatus('Stopping worker creation due to resource limits');
        break;
    }

    const worker = new Worker('parentWorker.js');
    
    worker.onmessage = function(e) {
        if (e.data.type === 'workerCount') {
            childWorkerCounts.set(i, e.data.count); // 为每个父 Worker 单独记录子 Worker 数量
            checkWorkerLimit();
        } else {
            updateStatus(`Parent Worker ${i}: ${JSON.stringify(e.data)}`);
        }
    };

    worker.onerror = function(error) {
        childWorkerCounts.delete(i); // 发生错误时清除该父 Worker 的子 Worker 计数
        updateTotalWorkers();
        updateStatus(`Parent Worker ${i} Error: ${error.message}`);
    };

    worker.postMessage({ id: i, type: 'start' });
    parentWorkers.push(worker);
}