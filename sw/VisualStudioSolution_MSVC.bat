@echo off
if not exist build_msvc mkdir build_msvc
pushd build_msvc
cmake -G "Visual Studio 17 2022" -A x64 ..
popd
start build_msvc\sw.sln
