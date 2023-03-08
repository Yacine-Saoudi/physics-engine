cmake_minimum_required(VERSION 3.0.0)
project(PhysEng VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

find_package(Threads REQUIRED)
include(CTest)
enable_testing()

# link sdl2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(PhysEng "${SRC_DIR}/main.cpp")
target_link_libraries(PhysEng ${SDL2_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
