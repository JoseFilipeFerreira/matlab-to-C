#include "roomsMC.h"
#include "roomsSA.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    time_t t;
    srand((unsigned) time(&t));

    if (argc != 3) return 1;
    int n = atoi(argv[1]);
    if (n <= 0) return 2;
    int procs = atoi(argv[2]);
    if (procs <= 0) return 3;

    int D[n][n];
    rng_incompatability_matrix(n, D);

    size_t* costsMC = malloc(sizeof(size_t) * procs);
    size_t* stepsMC = malloc(sizeof(size_t) * procs);
    size_t* costsSA = malloc(sizeof(size_t) * procs);
    size_t* stepsSA = malloc(sizeof(size_t) * procs);

    int roomMC[procs][n / 2][2];
    for (int i = 0; i < procs; i++) {
        rooms_mc(n, D, roomMC[i], costsMC + i, stepsMC + i);
        fprintf(stderr, "MC -> steps: %zu; cost: %zu\n", stepsMC[i], costsMC[i]);
    }

    int roomSA[procs][n / 2][2];
    for (int i = 0; i < procs; i++) {
        rooms_sa(n, D, roomSA[i], costsSA + i, stepsSA + i);
        fprintf(stderr, "SA -> steps: %zu; cost: %zu\n", stepsSA[i], costsSA[i]);
    }

    free(costsSA);
    free(stepsSA);
    free(costsMC);
    free(stepsMC);
}
