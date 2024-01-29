#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int my_rank, p;
    int row, col;  //dimensions    
    // Initialize MPI communication
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0) {
        // input matrix and vector dimensions    
        scanf("%d %d", &row, &col);
    }
    // Broadcast dimensions to all processes
    MPI_Bcast(&row, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Dynamic memory allocation
    float *matrix = (float *)malloc(row * col * sizeof(float));
    float *vector = (float *)malloc(col * sizeof(float));
    float *result = (float *)malloc(row * sizeof(float));

    if (my_rank == 0) {
        // Read matrix and vector values
        printf("Enter matrix elements:\n");
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                scanf("%f", &matrix[i * col + j]);
            }
        }
        printf("Enter vector elements:\n");
        for (int i = 0; i < col; i++) {
            scanf("%f", &vector[i]);
        }
    }
    // Distributing matrix rows among processes
    float *local_matrix = (float *)malloc(row / p * col * sizeof(float));
    MPI_Scatter(matrix, row / p * col, MPI_FLOAT, local_matrix, row / p * col, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // Broadcast vector to all processes
    MPI_Bcast(vector, col, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // Perform local matrix-vector multiplication

    float *local_result = (float *)malloc(row / p * sizeof(float));
    for (int i = 0; i < row / p; i++) {
        local_result[i] = 0.0;
        for (int j = 0; j < col; j++) {
            local_result[i] += local_matrix[i * col + j] * vector[j];
        }
    }
    MPI_Allgather(local_result, row / p, MPI_FLOAT, result, row / p, MPI_FLOAT, MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("Result:\n");
        for (int i = 0; i < row; i++) {
            printf("%f ", result[i]);
        }
        printf("\n");
    }
    free(matrix);
    free(vector);
    free(result);
    free(local_matrix);
    free(local_result);
    MPI_Finalize();
    return 0;
}