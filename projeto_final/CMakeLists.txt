cmake_minimum_required(VERSION 3.0.0)
project(CMakeTestProject VERSION 0.0.0)

include(CTest)
enable_testing()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(MPI)

if(MPI_FOUND)
    include_directories(${MPI_INCLUDE_PATH})
endif()

add_subdirectory(googletest)


add_library(TSP_LIB City.cpp Path.cpp AnnealingStealing.cpp Opt2.cpp rf-time.cc)
target_link_libraries(TSP_LIB ${MPI_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cpp)
target_link_libraries(main TSP_LIB)

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
