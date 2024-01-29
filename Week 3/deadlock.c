#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int my_rank;
	int p;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if(p != 2)
	{
		printf("use 2 processes");	//only possible for 2 processes
		MPI_Finalize();
		return 0;
	}

	if(argc < 1)
	{
		printf("Provide max size kb");
		MPI_Finalize();
		return 0;
	}

	int max_size = atoi(argv[1]) * 1024; 
	int *send_msg, *recv_msg;
	
	// message size is doubled for each iter
	// code stops after reaching deadlock

	for(int i = 1 ; i < max_size ; i*=2)
	{
		int message_size = i;
		if(my_rank == 0)
		{
			printf("message size%d\n", message_size);
			send_msg = (int *)malloc(message_size * sizeof(int));
			recv_msg = (int *)malloc(message_size * sizeof(int));

			MPI_Send(send_msg, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(recv_msg, message_size, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else
		{
			send_msg = (int *)malloc(message_size * sizeof(int));
			recv_msg = (int *)malloc(message_size * sizeof(int));

			MPI_Send(send_msg, message_size, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Recv(recv_msg, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	free(send_msg);
	free(recv_msg);
	MPI_Finalize();
	return 0;
}