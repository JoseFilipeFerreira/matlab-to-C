#include "roomsMC.h"
#include "roomsSA.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
    time_t t;
    srand((unsigned) time(&t));

    if (argc != 3) return 1;
    int n = atoi(argv[1]);
    if (n <= 0) return 2;
    int procs = atoi(argv[2]);
    if (procs <= 0) return 3;

    int** D = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        D[i] = malloc(sizeof(int) * n);
    }
    rng_incompatability_matrix(n, D);

    for (int i = 0; i < procs; i++) {
        if (!fork()) {
            int room[n / 2][2];
            size_t cost;
            size_t steps;
            rooms_mc(n, D, room, &cost, &steps);
            fprintf(stderr, "MC -> steps: %zu; cost: %zu\n", steps, cost);
            return 0;
        }
        if (!fork()) {
            int room[n / 2][2];
            size_t cost;
            size_t steps;
            rooms_sa(n, D, room, &cost, &steps);
            fprintf(stderr, "SA -> steps: %zu; cost: %zu\n", steps, cost);
            return 0;
        }
    }

    for (int i = 0; i < procs * 2; i++) wait(NULL);

    for (int i = 0; i < n; i++) free(D[i]);
    free(D);

    return 0;
}
