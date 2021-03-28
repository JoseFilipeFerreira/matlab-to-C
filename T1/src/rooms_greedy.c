#include "rooms_greedy.h"

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rooms_greedy(size_t n, int** D, int room[n / 2][2], size_t* cost) {
    (*cost) = 0;
    int tmp_cost = 0;
    unsigned char visited[n];
    for(size_t i = 0; i < n; visited[i++] = 0);
    for(size_t times = 0; times < n/2; times++) {
        for(size_t i = 0; i < n; i++) {
            if(visited[i]) continue;
            for(size_t j = i + 1; j < n; j++) {
                if(visited[j]) continue;
                if(!tmp_cost || D[i][j] < tmp_cost) {
                    tmp_cost = D[i][j];
                    room[times][0] = i;
                    room[times][1] = j;
                }
            }
        }
        *cost += tmp_cost;
        visited[room[times][0]] = visited[room[times][1]] = 1;
        tmp_cost = 0;
    }
}
