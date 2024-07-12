cmake_minimum_required(VERSION 3.20)
project(tcp) 
set(CMAKE_C_COMPILER "E:\\mingw64\\bin\\gcc.exe") 
set(CMAKE_CXX_COMPILER "E:\\mingw64\\bin\\g++.exe")
set(CMAKE_GENERATOR "MinGW Makefiles")
add_definitions(-DWIN32)

include_directories(inc tnet_core tnet_app lib/WpdPack/Include)
link_directories(lib/WpdPack/Lib/x64)

add_executable(tcp inc/pcap_device.c src/app.c)
add_subdirectory(tnet_core)
add_subdirectory(tnet_app)



target_link_libraries(tcp tnet_core tnet_app wpcap ws2_32)
