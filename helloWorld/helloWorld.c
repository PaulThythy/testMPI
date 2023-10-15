#include <stdio.h>
#include <mpi.h>

int main(int argc , char *argv []){
    int num_tasks, rank, len, rc;

    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    //affect rank to a process (from 0(initiator) to num_tasks-1)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //get number of tasks (after -np in the compilation command line)
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);

    printf("Hello world from processor %s, rank %d out of %d processors\n", hostname, rank, num_tasks);
    MPI_Finalize();
}
