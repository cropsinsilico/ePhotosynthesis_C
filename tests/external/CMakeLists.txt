cmake_minimum_required(VERSION 3.10)

project(ePhotosynthesisExample LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example example.cpp)

find_package(ePhotosynthesis REQUIRED)

target_link_libraries(example PUBLIC ePhotosynthesis::EPhotosynthesis)
