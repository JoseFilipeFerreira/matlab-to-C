#ifndef ___UTILS_H___
#define ___UTILS_H___

#include <stdlib.h>

void start_timer(void);

long long unsigned stop_timer(void);

void print_matrix(size_t size, int m[size][size]);

void rng_populate_rooms(size_t n, int room[n / 2][2]);

void rng_incompatability_matrix(size_t size, int matrix[size][size]);

#endif
