#include <stdio.h>
#include <mpi.h>

#define message_LENGTH 8192 

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
	
	int message[message_LENGTH] = {111 * (my_rank + 1)};
	int r_message_1[message_LENGTH], r_message_2[message_LENGTH];

	int source = my_rank;
	int destination_1 = (my_rank + 1) % p; // limit from 0 to p-1
	int destination_2 = (my_rank + p - 1) % p; // incrementing with p while keeping it non-zero
	
	MPI_Request req[4];
	MPI_Status stat[4];
	
	MPI_Isend(message, message_LENGTH, MPI_INT, destination_1, my_rank, MPI_COMM_WORLD, &req[0]);
	MPI_Isend(message, message_LENGTH, MPI_INT, destination_2, my_rank, MPI_COMM_WORLD, &req[1]);
	MPI_Irecv(r_message_1, message_LENGTH, MPI_INT, destination_1, destination_1, MPI_COMM_WORLD, &req[2]);
	MPI_Irecv(r_message_2, message_LENGTH, MPI_INT, destination_2, destination_2, MPI_COMM_WORLD, &req[3]);

	MPI_Waitall(4, req, stat);

	printf("p%d communicated with %d & %d\n", my_rank, destination_1, destination_2);

	MPI_Finalize();
	return 0;
}