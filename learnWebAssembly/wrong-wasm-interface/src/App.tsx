import React, { useState, useEffect } from 'react';
import './App.css';
import { createWorker2 } from './workerUtils.ts'; // Import the new function

// Simple type for our WASM module
interface WasmModule {
  Worker: {
    new(): any;
  };

  worker: Worker
}

function App() {
  const [wasmModule, setWasmModule] = useState<WasmModule | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [workerCount, setWorkerCount] = useState(0);

  // Load WASM module when component mounts
  useEffect(() => {
    const loadWasm = async () => {
      try {
        console.log('Loading WASM module...');
        
        // Load the script as text and evaluate it
        const response = await fetch('/cppWasm.js');
        const scriptText = await response.text();
        
        // Create a script element and execute it
        const script = document.createElement('script');
        script.textContent = scriptText;
        document.head.appendChild(script);
        
        // Access the global createModule function
        const createModule = (window as any).createModule;
        const module = await createModule();
        
        console.log('WASM module loaded successfully:', module);
        setWasmModule(module);
        setError(null);
      } catch (err) {
        console.error('Failed to load WASM:', err);
        setError(err instanceof Error ? err.message : 'Unknown error');
      } finally {
        setLoading(false);
      }
    };

    loadWasm();
  }, []);

  // Function to create a new Worker instance
  const createWorker = () => {
    if (!wasmModule) {
      console.error('WASM module not loaded yet');
      return;
    }
    const cppObj = new wasmModule.Worker();
    wasmModule.worker = cppObj; // Store the Worker instance in the wasmModule
    console.log('Worker created:', cppObj);
    setWorkerCount(prev => prev + 1);
    cppObj.ShowMyself();
  };

  // Example usage function for createWorker2
  const handleCreateWorkerAsync = async () => {
    try {
      const worker = await createWorker2(wasmModule, setWorkerCount);
      console.log('Successfully created worker:', worker);
    } catch (error) {
      console.error('Error creating worker:', error);
    }
  };

  // const handleCreateWorkerAsync2 = () => {
  //   wasmModule?.worker.ShowMyself();
  // };
  // ;(window as any).handleCreateWorkerAsync2 = handleCreateWorkerAsync2;

  const handleCreateWorkerAsync2 = () => {
    createWorker2(wasmModule, setWorkerCount)
    .then((val) => {})
    .catch((error) => { // if we catch here and don't output log, the browser will just hang and not show any error log on the console
      console.error("createWorker2 error: ", error)
    })
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>WASM + TypeScript Demo</h1>
        
        {loading && (
          <div>
            <p>Loading WASM module...</p>
          </div>
        )}
        
        {error && (
          <div style={{ color: 'red' }}>
            <p>Error: {error}</p>
          </div>
        )}
        
        {wasmModule && (
          <div>
            <p>✅ WASM module loaded successfully!</p>
            <button onClick={createWorker} style={{ margin: '10px', padding: '10px' }}>
              Create Worker
            </button>
            <button onClick={handleCreateWorkerAsync2} style={{ margin: '10px', padding: '10px' }}>
              Create Worker Async
            </button>
            <p>Workers created: {workerCount}</p>
          </div>
        )}
        
      </header>
    </div>
  );
}

export default App;