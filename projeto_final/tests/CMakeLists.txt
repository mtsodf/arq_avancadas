add_executable(PathTest PathTest.cpp)
target_link_libraries(PathTest gtest_main TSP_LIB)

configure_file(ler_out.py ler_out.py COPYONLY)

configure_file(qsub_mpi.sh qsub_mpi.sh COPYONLY)

configure_file(qsub_mpi_2nos.sh qsub_mpi_2nos.sh COPYONLY)

add_test(PathTest ./PathTest)


add_executable(OpenMP OpenMP.cpp)
target_link_libraries(OpenMP gtest_main TSP_LIB)


add_executable(Opt2Test Opt2Test.cpp)
target_link_libraries(Opt2Test gtest_main TSP_LIB)

add_executable(Opt2MPITest Opt2MPITest.cpp)
target_link_libraries(Opt2MPITest gtest_main TSP_LIB)

add_executable(Opt2OpenMPTest Opt2OpenMPTest.cpp)
target_link_libraries(Opt2OpenMPTest gtest_main TSP_LIB)
