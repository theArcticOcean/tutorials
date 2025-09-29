#!/usr/bin/env python3

import os
import urllib.parse
from http.server import HTTPServer, SimpleHTTPRequestHandler

class CustomRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=os.getcwd(), **kwargs)

    def send_head(self):
        path = self.translate_path(self.path)
        
        # Handle source file mapping for debugging
        if self.path.startswith('/src/'):
            # Map /src/ requests to parent directory (where main.cpp is located)
            relative_path = self.path[5:]  # Remove '/src/' prefix
            mapped_path = os.path.join(os.path.dirname(os.getcwd()), relative_path)
            if os.path.exists(mapped_path) and os.path.isfile(mapped_path):
                path = mapped_path
        elif self.path == '/main.cpp' or self.path.endswith('/main.cpp'):
            # Direct mapping for main.cpp
            main_cpp_path = os.path.join(os.path.dirname(os.getcwd()), 'main.cpp')
            if os.path.exists(main_cpp_path):
                path = main_cpp_path
        
        # Check if file exists
        if not os.path.exists(path) or not os.path.isfile(path):
            return None
        
        # Set appropriate MIME types with enhanced WebAssembly support
        ctype = self.guess_type(path)
        if path.endswith('.js'):
            ctype = 'application/javascript; charset=utf-8'
        elif path.endswith('.wasm'):
            ctype = 'application/wasm'
        elif path.endswith('.wasm.map') or path.endswith('.map'):
            ctype = 'application/json; charset=utf-8'
        elif path.endswith('.cpp') or path.endswith('.c') or path.endswith('.h'):
            ctype = 'text/plain; charset=utf-8'
        elif path.endswith('.debug.wasm'):
            ctype = 'application/wasm'
        elif path.endswith('.html'):
            ctype = 'text/html; charset=utf-8'
        elif path.endswith('.css'):
            ctype = 'text/css; charset=utf-8'
            
        try:
            f = open(path, 'rb')
        except OSError:
            self.send_error(404, "File not found")
            return None
        
        self.send_response(200)
        self.send_header("Content-type", ctype)
        fs = os.fstat(f.fileno())
        self.send_header("Content-Length", str(fs[6]))
        
        # Enhanced CORS headers for WebAssembly debugging
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        
        # Special headers for WebAssembly files
        if path.endswith('.wasm'):
            self.send_header('Cross-Origin-Resource-Policy', 'cross-origin')
        
        # Cache control for debugging files
        if path.endswith(('.cpp', '.c', '.h', '.map', '.wasm.map')):
            self.send_header('Cache-Control', 'no-cache, no-store, must-revalidate')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Expires', '0')
        elif path.endswith('.wasm'):
            # Allow some caching for WASM files but make it short
            self.send_header('Cache-Control', 'public, max-age=300')
            
        self.end_headers()
        return f
    
    def do_OPTIONS(self):
        # Handle preflight requests
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
    
    def log_message(self, format, *args):
        # Enhanced logging for debugging
        print(f"[{self.address_string()}] {format % args}")
        if self.path.endswith(('.cpp', '.map', '.wasm')):
            print(f"  -> Serving debug-related file: {self.path}")

def run_server(host='127.0.0.1', port=4002):
    server_address = (host, port)
    httpd = HTTPServer(server_address, CustomRequestHandler)
    print(f'Starting enhanced debug server on {host}:{port}...')
    print(f'Serving files from: {os.getcwd()}')
    print(f'Source files mapped from: {os.path.dirname(os.getcwd())}')
    print('Debug features enabled:')
    print('  - Source file mapping (/src/ -> parent directory)')
    print('  - Enhanced MIME type support for .wasm, .wasm.map, .map files')
    print('  - CORS headers for WebAssembly debugging')
    print('  - Optimized cache control for debug and WASM files')
    httpd.serve_forever()

if __name__ == '__main__':
    run_server()