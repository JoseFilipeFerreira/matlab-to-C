#ifndef ___ROOMSMC_H___
#define ___ROOMSMC_H___

#include <stdlib.h>

void rooms_mc(size_t n, const int D[n][n], int room[n / 2][2], size_t* cost, size_t* steps);
#endif
