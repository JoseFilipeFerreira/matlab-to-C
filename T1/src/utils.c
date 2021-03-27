#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define TIME_RESOLUTION 1000000 // time measuring resolution (us)

long long unsigned start_time;
struct timeval t;

void start_timer(void) {
    gettimeofday(&t, NULL);
    start_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop_timer(void) {
    gettimeofday(&t, NULL);
    long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

    return final_time - start_time;
}

void print_matrix(size_t size, int** m) {
    size_t max_print = 8;
    if (size <= max_print) {
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                printf("%d\t", m[i][j]);
            }
            putchar('\n');
        }

    } else {
        for (size_t i = 0; i < max_print - 2; i++) {
            for (size_t j = 0; j < max_print - 2; j++) {
                printf("%d\t", m[i][j]);
            }
            printf("...\t%d\n", m[i][size - 1]);
        }

        for (size_t j = 0; j < max_print; j++) {
            printf("...\t");
        }
        putchar('\n');

        for (size_t j = 0; j < max_print - 2; j++) {
            printf("%d\t", m[size - 1][j]);
        }
        printf("...\t%d\n", m[size - 1][size - 1]);
    }
}

void rng_populate_rooms(size_t n, int room[n / 2][2]) {
    int p = 0;
    for(size_t r = 0; r < n/2; r++){
        for(size_t i = 0; i < 2; i++){
            room[r][i] = p++;
        }
    }

    for(size_t r = 0; r < n/2; r++){
        for(size_t i = 0; i < 2; i++){
            int nr = rand() % (n/2);
            int ni = rand() % 2;
            int tmp = room[r][i];
            room[r][i] = room[nr][ni];
            room[nr][ni] = tmp;
        }
    }
}

void rng_incompatability_matrix(size_t size, int** matrix) {
    for (size_t i = 0; i < size; i++) {
        matrix[i][i] = 0;
        for (size_t j = 0; j < i; j++) {
            int v = rand() % 10 + 1;
            matrix[i][j] = v;
            matrix[j][i] = v;
        }
    }
}

