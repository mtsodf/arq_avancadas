#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]){
    int id;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    cout << "Hello World! Eu sou o processo " << id << "\n";

    MPI_Finalize();
    return 0;
}
