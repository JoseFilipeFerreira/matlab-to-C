#include "roomsMC.h"
#include "roomsSA.h"
#include "rooms_greedy.h"
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

    int cost_pipe[2];
    if(pipe(cost_pipe) < 0) exit(1);

    for (int i = 0; i < procs; i++) {
        if (!fork()) {
            srand((unsigned) time(NULL) ^ (getpid() << 16));
            int room[n / 2][2];
            size_t cost;
            size_t steps;
            rooms_mc(n, D, room, &cost, &steps, maxi);
            write(cost_pipe[1], &cost, sizeof(size_t));
            return 0;
        }
    }

    for(int i = 0; i < procs; i++){
        size_t r;
        read(cost_pipe[0], &r, sizeof(size_t));
        fprintf(stderr, "%zu%s", r, i == procs - 1?"":",");
    }

    fprintf(stderr, "\n");


    for (int i = 0; i < procs; i++) {
        if (!fork()) {
            srand((unsigned) time(NULL) ^ (getpid() << 16));
            int room[n / 2][2];
            size_t cost;
            size_t steps;
            rooms_sa(n, D, room, &cost, &steps, maxi, tmin, cooldown);
            write(cost_pipe[1], &cost, sizeof(size_t));
            return 0;
        }
    }

    for(int i = 0; i < procs; i++){
        size_t r;
        read(cost_pipe[0], &r, sizeof(size_t));
        fprintf(stderr, "%zu%s", r, i == procs - 1?"":",");
    }

    fprintf(stderr, "\n");

    int room[n / 2][2];
    size_t cost;
    rooms_greedy(n, D, room, &cost);
    fprintf(stderr, "%zu\n", cost);

    for (int i = 0; i < n; i++) free(D[i]);
    free(D);

    return 0;
}
