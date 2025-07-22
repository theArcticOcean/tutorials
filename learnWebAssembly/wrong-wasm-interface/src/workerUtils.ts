// Define the interface for the WASM module
interface WasmModule {
  Worker: {
    new(): any;
  };
  
  worker: Worker;
}

// Async function to create a new Worker instance
export const createWorker2 = async (wasmModule: WasmModule | null, setWorkerCount: React.Dispatch<React.SetStateAction<number>>) => {
  if (!wasmModule) {
    const error = 'WASM module not loaded yet';
    console.error(error);
    throw new Error(error);
  }

  const cppObj = new wasmModule.Worker();
  console.log('Worker created asynchronously:', cppObj);
  setWorkerCount(prev => prev + 1);
  wasmModule.worker = cppObj; // Store the Worker instance in the wasmModule
  wasmModule.worker.ShowMyself();
  //wasmModule.worker.OnInit();
};