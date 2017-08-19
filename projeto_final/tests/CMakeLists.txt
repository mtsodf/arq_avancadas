add_executable(PathTest PathTest.cpp)
target_link_libraries(PathTest gtest_main TSP_LIB)


add_test(PathTest ./PathTest)