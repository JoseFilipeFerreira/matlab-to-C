#ifndef ___ROOMSSA_H___
#define ___ROOMSSA_H___

#include <stdlib.h>

void rooms_sa(size_t n, const int D[n][n], int room[n / 2][2], size_t* cost, size_t* steps);
#endif
