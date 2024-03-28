# 通信部分CMakeLists.txt测试
cmake_minimum_required(VERSION 3.0)
project(tcp_client)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})
include_directories(include)
include_directories(/usr/local/include/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
find_package(Threads REQUIRED)

add_executable(tcp_client cpp_client.cpp)
target_link_libraries(tcp_client  Threads::Threads)
