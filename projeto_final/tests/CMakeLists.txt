add_executable(PathTest PathTest.cpp)
target_link_libraries(PathTest gtest_main TSP_LIB)

configure_file(ler_out.py ler_out.py COPYONLY)
add_test(PathTest ./PathTest)


add_executable(OpenMP OpenMP.cpp)
target_link_libraries(OpenMP gtest_main TSP_LIB)


add_executable(Opt2Test Opt2Test.cpp)
target_link_libraries(Opt2Test gtest_main TSP_LIB)

add_executable(Opt2MpiTest Opt2MpiTest.cpp)
target_link_libraries(Opt2MpiTest gtest_main TSP_LIB)
