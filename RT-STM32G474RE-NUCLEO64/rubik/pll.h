#ifndef PLL_H
#define PLL_H

#include "Cube.h"

static void solvePLL(Cube* cube);
void adjustUFace(Cube* cube);
void solvePLLCorners(Cube* cube);
int checkPLLCorners(Cube* cube);
int checkPLLEdges(Cube* cube);
void solvePLLEdges(Cube* cube);

#endif
