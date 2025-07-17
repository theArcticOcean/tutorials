const PARENT_WORKERS = 100;
const parentWorkers = [];
let totalWorkers = 0;
const maxWorkersLimit = navigator.hardwareConcurrency*4; // 设置一个合理的上限

function checkWorkerLimit(count) {
    const memoryUsage = window.performance?.memory?.usedJSHeapSize;
    const memoryLimit = window.performance?.memory?.jsHeapSizeLimit;
    
    updateStatus(`Total workers: ${totalWorkers}`);
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

    const worker = new Worker('childWorker.js');
    totalWorkers++;
    
    worker.onmessage = function(e) {
        if (e.data.type === 'workerCount') {
            totalWorkers += e.data.count;
            checkWorkerLimit();
        } else {
            updateStatus(`Parent Worker ${i}: ${JSON.stringify(e.data)}`);
        }
    };

    worker.onerror = function(error) {
        totalWorkers--;
        updateStatus(`Parent Worker ${i} Error: ${error.message}`);
    };

    worker.postMessage({ id: i, type: 'start' });
    parentWorkers.push(worker);
}