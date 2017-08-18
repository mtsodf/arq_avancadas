

using namespace std;




int main(int argc, char *argv[]){
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello World! Eu sou o processo %d\n", id);
        //cout << "Hello World! Eu sou o processo " << id << "\n";
    }


    return 0;
}