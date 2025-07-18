import React, { useState, useEffect } from 'react';
//import createModule from 'cppWasm.js';
import './App.css';

// Simple type for our WASM module
interface WasmModule {
  Worker: {
    new(): any;
  };
}

function App() {
  const [wasmModule, setWasmModule] = useState<WasmModule | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [workerCount, setWorkerCount] = useState(0);

  // Load WASM module when component mounts
  useEffect(() => {
    console.error = () => {};

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

    try {
      const cppObj = new wasmModule.Worker();
      console.log('Worker created:', cppObj);
      setWorkerCount(prev => prev + 1);
      cppObj.ShowMyself();
    } catch (err) {
      console.error('Failed to create worker:', err);
    }
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
            <p>Workers created: {workerCount}</p>
          </div>
        )}
        
      </header>
    </div>
  );
}

export default App;