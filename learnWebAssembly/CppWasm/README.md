simple cpp project to wasm file.  

build:  
cd <path-at-CMakeLists.txt>  
mkdir build_wasm  
cd build_wasm  
emcmake cmake ../  
make  
  
run:  
cd <path-at-CMakeLists.txt>  
python3 -m http.server 4002  
