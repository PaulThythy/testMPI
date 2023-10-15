#include <stdio.h>
#include <mpi.h>

/**
 * between two processes only
*/

int main(int argc , char *argv []){
    int num_tasks, rank, len, rc;

    /**
     * initiator sends 0, prints ping
     * if rank 1 receives 0, prints pong and send 1
    */
    int message = 0;

    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);

    if(num_tasks != 2){
        printf("Only two processes required !");
        MPI_Finalize();
        return 1;
    }

    if(rank == 0){
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process %d sent %d : ping\n", rank, message);    
        //MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        //printf("Process %d reveived %d\n", rank, message);
    }
    else if(rank == 1){
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received %d : pong\n", rank, message);
        //MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);   
        //printf("Process %d sent %d\n", rank, message);
    }

    MPI_Finalize();
}
