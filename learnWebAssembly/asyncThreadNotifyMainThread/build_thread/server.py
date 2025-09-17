# Python标准库的http.server模块中配置SharedArrayBuffer响应头

import os
from http.server import HTTPServer, SimpleHTTPRequestHandler

class CustomRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=os.getcwd(), **kwargs)

    def send_head(self):
        path = self.translate_path(self.path)
        
        # 检查文件是否存在并获取其MIME类型
        if not os.path.exists(path) or not os.path.isfile(path):
            return None
        
        ctype = self.guess_type(path)
        try:
            f = open(path, 'rb')
        except OSError:
            self.send_error(404, "File not found")
            return None
        
        self.send_response(200)
        self.send_header("Content-type", ctype)
        fs = os.fstat(f.fileno())
        self.send_header("Content-Length", str(fs[6]))
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.end_headers()
        return f

def run_server(host='127.0.0.1', port=4002):
    server_address = (host, port)
    httpd = HTTPServer(server_address, CustomRequestHandler)
    print(f'Starting server on {host}:{port}...')
    httpd.serve_forever()

if __name__ == '__main__':
    run_server()