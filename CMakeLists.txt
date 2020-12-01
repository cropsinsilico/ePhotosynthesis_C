cmake_minimum_required(VERSION 3.5)

project(ePhotosynthesis LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SOURCES "source/*.cpp")
add_executable(ePhotosynthesis ${SOURCES})
target_link_libraries(ePhotosynthesis sundials_nvecserial sundials_kinsol sundials_cvode)
