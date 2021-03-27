#include "roomsMC.h"
#include "roomsSA.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 6) return 1;
    int n = atoi(argv[1]);
    int procs = atoi(argv[2]);
    int maxi = atoi(argv[3]);
    double tmin = atof(argv[4]);
    double cooldown = atof(argv[5]);

    int** D = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        D[i] = malloc(sizeof(int) * n);
    }

    srand((unsigned) time(NULL) + clock());
    rng_incompatability_matrix(n, D);

    for (int i = 0; i < procs; i++) {
        if (!fork()) {
            srand((unsigned) time(NULL) ^ (getpid() << 16));
            int room[n / 2][2];
            size_t cost;
            size_t steps;
            rooms_mc(n, D, room, &cost, &steps, maxi);
            fprintf(stderr, "MC -> s: %zu, c: %zu\n", steps, cost);
            return 0;
        }
    }

    for (int i = 0; i < procs; i++) wait(NULL);
    fprintf(stderr, "\n");


    for (int i = 0; i < procs; i++) {
        if (!fork()) {
            srand((unsigned) time(NULL) ^ (getpid() << 16));
            int room[n / 2][2];
            size_t cost;
            size_t steps;
            rooms_sa(n, D, room, &cost, &steps, maxi, tmin, cooldown);
            fprintf(stderr, "SA -> s: %zu, c: %zu\n", steps, cost);
            return 0;
        }
    }
    for (int i = 0; i < procs; i++) wait(NULL);
    fprintf(stderr, "\n");

    for (int i = 0; i < n; i++) free(D[i]);
    free(D);

    return 0;
}
