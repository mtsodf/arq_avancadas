add_executable(PathTest PathTest.cpp)
target_link_libraries(PathTest gtest_main TSP_LIB)

configure_file(ler_out.py ler_out.py COPYONLY)

configure_file(qsub_mpi.sh qsub_mpi.sh COPYONLY)
configure_file(qsub_mpi_2nos.sh qsub_mpi_2nos.sh COPYONLY)
configure_file(qsub_openmp.sh qsub_openmp.sh COPYONLY)



add_executable(Opt2Test Opt2Test.cpp)
target_link_libraries(Opt2Test gtest_main TSP_LIB)

add_executable(Opt2MPITest Opt2MPITest.cpp)
target_link_libraries(Opt2MPITest gtest_main TSP_LIB)

add_executable(Opt2OpenMPTest Opt2OpenMPTest.cpp)
target_link_libraries(Opt2OpenMPTest gtest_main TSP_LIB)

add_executable(Opt2OpenMPStaticTest Opt2OpenMPStaticTest.cpp)
target_link_libraries(Opt2OpenMPStaticTest gtest_main TSP_LIB)


add_executable(Opt2OpenMPDynamicTest Opt2OpenMPDynamicTest.cpp)
target_link_libraries(Opt2OpenMPDynamicTest gtest_main TSP_LIB)


add_test(PathTest ./PathTest)
add_test(Opt2Test ./Opt2Test)