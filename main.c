#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    int processCount;
    int rootRank = 0;
    int elementCount = 4;
    int elements[4] = {-1, 2, 9, 5};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    int elementsPerProcess = elementCount / processCount;
    int local[elementsPerProcess];
    MPI_Scatter(elements, elementsPerProcess, MPI_INT, local, elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
    int offset = 0;
    if (rank == rootRank) {
        for (int i = 0; i < elementCount; i++) {
            if (elements[i] < offset) {
                offset = elements[i];
            }
        }
    }
    if (rank == rootRank) {
        for (int process = 0; process < processCount; process++) {
            if (process != rootRank) {
                MPI_Send(&offset, 1, MPI_INT, process, 0,
                         MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(&offset, 1, MPI_INT, rootRank, 0, MPI_COMM_WORLD, NULL);
    }

    for (int i = 0; i < elementsPerProcess; i++) {
        local[i] = local[i] - offset;
    }

    MPI_Gather(local, elementsPerProcess, MPI_INT, elements,
               elementsPerProcess, MPI_INT, rootRank, MPI_COMM_WORLD);

    if (rank == rootRank) {
        for (int i = 0; i < elementCount; i++) {
            printf("%i ", elements[i]);
        }
    }
    return 0;
}
