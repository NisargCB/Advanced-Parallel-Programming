#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int my_rank;
	int p;
	MPI_Status status;
	// variables for algorithm
	int broadcast_integer = -1;
	int spacing;
	int stage;
	int source;
	int dest;
	int tag = 0;
	int c = 1;
	// variables for algorithm
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	// broadcast algorithm starts
	if(my_rank == 0)
		broadcast_integer = 100;
	spacing = p; stage = 0;
	
	while (c < p) {
    if (my_rank < c) {
        if (my_rank + c < p) {
            dest = my_rank + c;
            printf("%d sends message to %d, %d \n", my_rank, dest, stage);
            MPI_Send(&broadcast_integer, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    } else if (my_rank < 2 * c) {
        source = my_rank - c;
        printf("%d receives from %d, %d \n", my_rank, source, stage);
        MPI_Recv(&broadcast_integer, 1, MPI_INT, source, tag,MPI_COMM_WORLD, &status);
    }
    c =c*2;
    MPI_Barrier(MPI_COMM_WORLD);
    stage = stage + 1;
    }
    
	MPI_Finalize();
	printf("Process %d has %d \n",my_rank,broadcast_integer);

	return 0;
}