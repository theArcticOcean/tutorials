const CHILD_WORKERS = 10;
const childWorkers = [];
let activeChildWorkers = 0;

function reportWorkerCount() {
    postMessage({
        type: 'workerCount',
        count: activeChildWorkers
    });
}

// 创建子级 workers
for (let i = 0; i < CHILD_WORKERS; i++) {
    try {
        const worker = new Worker('childWorker.js');
        activeChildWorkers++;
        reportWorkerCount();
        
        worker.onmessage = function(e) {
            if (e.data.type === 'error' || e.data.type === 'resourceLimit') {
                postMessage({
                    type: 'warning',
                    childId: i,
                    message: e.data.message
                });
            } else {
                postMessage({
                    childId: i,
                    data: e.data
                });
            }
        };

        worker.onerror = function(error) {
            activeChildWorkers--;
            reportWorkerCount();
            postMessage({
                type: 'error',
                childId: i,
                message: error.message
            });
        };

        worker.postMessage({ id: i, type: 'start' });
        childWorkers.push(worker);
    } catch (e) {
        postMessage({
            type: 'error',
            message: `Failed to create child worker ${i}: ${e.message}`
        });
        break;
    }
}

// 接收来自主线程的消息
onmessage = function(e) {
    postMessage({
        type: 'info',
        message: `Parent worker ${e.data.id} started with ${CHILD_WORKERS} child workers`
    });
};