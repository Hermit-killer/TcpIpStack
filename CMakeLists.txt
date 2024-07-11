cmake_minimum_required(VERSION 3.20)
project(tcp) 
set(CMAKE_C_COMPILER "E:\\mingw64\\bin\\gcc.exe") 
set(CMAKE_CXX_COMPILER "E:\\mingw64\\bin\\g++.exe")
set(CMAKE_GENERATOR "MinGW Makefiles")

include_directories(inc tnet_core "E:/WpdPack/Include")

add_executable(tcp  "C:/Users/Dan/Desktop/tcp/src/app.c")
