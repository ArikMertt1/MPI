#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank < 2) {
        int other_rank = 1 - my_rank;
        int tag = 0;
        char message[14];
        sprintf(message, "Hello, I am %d", my_rank);
        MPI_Status status;
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, other_rank, tag, MPI_COMM_WORLD);
        MPI_Recv(message, 100, MPI_CHAR, other_rank, tag, MPI_COMM_WORLD, &status);
        printf("%s\n", message);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
