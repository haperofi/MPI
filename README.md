# MPI
2 Simple Message Passing Interface (MPI) examples to demonstrate sending and receiving messages. These were written and tested using the OpenMPI library.

MPI_send_recv_example.c uses blocking sending and receiving, with MPI_Send and MPI_Recv.

MPI_isend_irecv_example.cpp uses non-blocking messaging, with MPI_Isend, MPI_Irecv, and MPI_Waitall.

Process 0 is the master process that sends data (integers of 1234, 2345, 3456, etc.) to each of the slave processes and waits for their reply. The slaves in turn modify the data they received by multiplying it by -1 and send it back.


Build using mpicc or mpiCC for c and c++ files, respectively.

Run with command, e.g., "mpirun -n 6 --hostfile hostfile ./example.run"
