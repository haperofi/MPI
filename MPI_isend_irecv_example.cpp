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


        if(world_rank==0){      // Master process

                std::vector<MPI_Request> requests(world_size);
                std::vector<int> data(world_size);
                printf("(%s:) master sending data to all slaves.\n", processor_name);
                for(int i  = 1; i < world_size; i++){
                        data[i]=1000*(i%9)+100*((i+1)%9)+10*((i+2)%9)+(i+3)%9; //send 1234, 2345, etc
                        MPI_Isend(&data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i-1]);
                }
                MPI_Waitall(world_size-1, requests.data(),MPI_STATUSES_IGNORE);
                printf("(%s:) all messages sent successfully.\n", processor_name);

                //the same data+requests vectors can be used because we've waited for isend completion
                for(int i  = 1; i < world_size; i++){
                        MPI_Irecv(&data[i], 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i-1]);
                }

                MPI_Waitall(world_size-1, requests.data(),MPI_STATUSES_IGNORE);
                printf("(%s:) all slaves have responded.\n", processor_name);

                for(int i  = 1; i < world_size; i++){
                        printf("(%s:) master received data: %d from slave-%d.\n", processor_name, data[i], i);
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
                
