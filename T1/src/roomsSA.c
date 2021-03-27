#include "roomsSA.h"

#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void rooms_sa(size_t n, int** D, int room[n / 2][2], size_t* cost, size_t* steps, int MAXI, double TMIN, double COOLDOWN) {
    rng_populate_rooms(n, room);

    (*cost) = 0;
    for (size_t i = 0; i < n / 2; i++) {
        (*cost) += D[room[i][0]][room[i][1]];
    }

    int i = 0;
    *steps = 0;
    double T = 1;

    // stop if no changes for 100 trials
    while (i < MAXI) {
        (*steps)++;
        i++;
        // choose 2 sucessive rooms to swap
        size_t c = rand() % (n / 2);
        size_t d = (c + 1 == (n / 2)) ? 1 : c + 1;

        int delta = D[room[c][0]][room[d][1]] + D[room[d][0]][room[c][1]] -
                    D[room[c][0]][room[c][1]] - D[room[d][0]][room[d][1]];

        // if the result is better swap them
        double rng = (double) rand() / (RAND_MAX);
        if (delta < 0 || exp(-(double) delta / T) >= rng) {
            int tmp = room[c][0];
            room[c][0] = room[d][0];
            room[d][0] = tmp;
            (*cost) += delta;
            i = 0;
        }
        T *= COOLDOWN;

        if (T <= TMIN) return;
    }
}
