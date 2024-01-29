#include <stdio.h>
#include <mpi.h>

#define message_Length 512 //  failed to run on message length of 1024

int main(int argc, char** argv)
{
	int my_rank;
	int p;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if(p < 3)
	{
		printf("Use atleast 3 processes");
		MPI_Finalize();
		return 0;
	}
	
	int message[message_Length] = {111 * (my_rank + 1)};
	int r_message_1[message_Length], r_message_2[message_Length];

	int source = my_rank;
	int destination_1 = (my_rank + 1) % p; // to keep it withing 0 to p-1
	int destination_2 = (my_rank + p - 1) % p; // incrementing with p then -1 to keep it above zero
	
	MPI_Send(message, message_Length, MPI_INT, destination_1, my_rank, MPI_COMM_WORLD);
	MPI_Send(message, message_Length, MPI_INT, destination_2, my_rank, MPI_COMM_WORLD);
	MPI_Recv(r_message_1, message_Length, MPI_INT, destination_1, destination_1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(r_message_2, message_Length, MPI_INT, destination_2, destination_2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	printf("p%d sent to %d & %d\n", my_rank, destination_1, destination_2);
	printf("p%d recevied from %d\n", my_rank, destination_1);
	printf("p%d recevied from %d\n", my_rank, destination_2);

	MPI_Finalize();
	return 0;
}