#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/**
 * between two processes only
 * but now with multiple exchanges between the two processes
*/

int main(int argc , char *argv []){
    int num_tasks, rank, len, rc;
    
    int message = 0;

    int num_exchange_max = atoi(argv[1]);
    int exchange_count = 0;

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

    while(exchange_count < num_exchange_max){

        if(rank == 0){
            //process 0 sends "ping" (0) to process 1
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("ping\n");

            //process 0 receives "pong" (1) from process 1  
            MPI_Recv(&message, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
            //printf("Process %d reveived %d\n", rank, message);
        }
        else if(rank == 1){
            //process 1 receives "ping" (0) from process 0
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("pong\n");

            //process 1 sends "pong" (1) to process 0
            MPI_Send(&message, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);   
            //printf("Process %d sent %d\n", rank, message);
        }
        exchange_count++;
    }

    MPI_Finalize();
}
