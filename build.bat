SET CC="C:\Program Files\LLVM\bin\clang.exe"
SET CXX="C:\Program Files\LLVM\bin\clang++.exe"
cmake -G "Visual Studio 15 2017 Win64" -T "LLVM-vs2017" -D CMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -D CMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -D CMAKE_BUILD_TYPE=Debug ..
cmake --build . -- /m /v:d
