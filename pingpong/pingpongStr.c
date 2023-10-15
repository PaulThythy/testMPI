#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

/**
 * between two processes only
 * but now with multiple exchanges between the two processes
*/

int main(int argc , char *argv []){
    int num_tasks, rank, len, rc;
    
    char message[4];

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
            //char* strcpy(char* destination, const char* source);
            strcpy(message, "ping");
            MPI_Send(message, strlen(message)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            printf("%s\n", message);

            //process 0 receives "pong" (1) from process 1  
            MPI_Recv(message, strlen(message)+1, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
            //printf("Process %d reveived %s\n", rank, message);
        }
        else if(rank == 1){
            //process 1 receives "ping" (0) from process 0
            MPI_Recv(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            strcpy(message, "pong");
            printf("%s\n", message);
            //printf("Process %d reveived %s\n", rank, message);

            //process 1 sends "pong" (1) to process 0
            MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);   
            //printf("Process %d sent %d\n", rank, message);
        }
        exchange_count++;
    }

    MPI_Finalize();
}
