cmake_minimum_required(VERSION 3.15)
project(kis_admission)

set(CMAKE_CXX_STANDARD 11)

add_executable(kis_admission server.cpp game.cpp wordpool.cpp main.cpp)