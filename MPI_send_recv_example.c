#include <mpi.h>
#include <stdio.h>
#include <vector>

int main(int argc, char** argv) {
        // Initialize the MPI environment
        MPI_Init(NULL, NULL);

        // Get the number of processes
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        // Get the rank of the process
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        // Get the name of the processor
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);

        // Print off a hello world message
        //printf("Hello world from processor %s, rank %d out of %d processors\n",

        if(world_rank==0){      // Master process
                printf("(%s:) master sending data to all slaves.\n", processor_name);
                for(int i  = 1; i < world_size; i++){
                        int data=1000*(i%9)+100*((i+1)%9)+10*((i+2)%9)+(i+3)%9; //send 1234, 2345, etc
                        MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                for(int i  = 1; i < world_size; i++){
                        int data;
                        MPI_Recv(&data, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        printf("(%s:) master received data: %d from slave-%d.\n", processor_name, data, i);
                }

        }
        else{   // Receive from master, modify, send back
                int data;
                MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("(%s:) slave-%d received data: %d. Modifying and sending back.\n", processor_name, world_rank, data);
                data=-data;
                MPI_Send(&data, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD);
        }

        // Finalize the MPI environment.
        MPI_Finalize();
}
