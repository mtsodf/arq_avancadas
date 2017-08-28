add_executable(PathTest PathTest.cpp)
target_link_libraries(PathTest gtest_main TSP_LIB)

configure_file(ler_out.py ler_out.py COPYONLY)
add_test(PathTest ./PathTest)


add_executable(OpenMP OpenMP.cpp)
target_link_libraries(OpenMP gtest_main TSP_LIB)