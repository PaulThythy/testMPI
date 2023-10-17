#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/**
 * ring-like program, using more than two processes
*/

/**
 * message transmission continues until message = threshold
 * when it does, the current process transmits the stop message to its neighbor
 * which in turn transmits it to the next process until it goes back to him
*/

int main(int argc , char *argv []){
    int num_tasks, rank, len, rc;
    
    int message = 0;
    int stop_message = -1;
    int threshold = 15;

    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    MPI_Get_processor_name(hostname, &len);

    if(num_tasks < 3){
        printf("At least 3 processes required !");
        MPI_Finalize();
        return 1;
    }

    while(1){
        if(rank == 0){  //initiator
            //process 0 sends 0 to 1
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            printf("Node %d sends message %d to %d\n", rank, message, rank + 1);
            MPI_Recv(&message, 1, MPI_INT, num_tasks - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if(message == -1){
                printf("Node %d detected stop message, sending it to %d\n", rank, rank + 1);
                MPI_Send(&stop_message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                break;
            }
            else{
                message = message + 1;
                if(message >= threshold){
                    printf("Node %d reached threshold, sending the stop message to %d\n", rank, rank + 1);
                    MPI_Send(&stop_message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    break;
                }
                else{
                    printf("Node %d sends message %d to %d\n", rank, message, rank + 1);
                    MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
            }
        }
        else if(rank == num_tasks - 1){
            MPI_Recv(&message, 1, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if(message == -1){
                printf("Node %d detected stop message, sending it to 0\n", rank);
                MPI_Send(&stop_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                break;
            }
            else{
                message = message + 1;
                if(message >= threshold){
                    printf("Node %d reached threshold, sending the stop message to 0\n", rank);
                    MPI_Send(&stop_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                    break;
                }
                else{
                    printf("Node %d sends message %d to 0\n", rank, message);
                    MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
            }
        }
        else{
            MPI_Recv(&message, 1, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if(message == -1){
                printf("Node %d detected stop message, sending it to %d\n", rank, rank + 1);
                MPI_Send(&stop_message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                break;
            }
            else{
                message = message + 1;
                if(message >= threshold){
                    printf("Node %d reached threshold, sending the stop message to %d\n", rank, rank + 1);
                    MPI_Send(&stop_message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    break;
                }
                else{
                    printf("Node %d sends message %d to %d\n", rank, message, rank + 1);
                    MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}
