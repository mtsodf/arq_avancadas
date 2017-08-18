cmake_minimum_required(VERSION 3.0.0)
project(CMakeTestProject VERSION 0.0.0)

include(CTest)
enable_testing()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


add_subdirectory(googletest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(main main.cpp City.cpp Path.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

