#ifndef OLL_H
#define OLL_H

#include "cube.h"

static void solveOLL(Cube* cube);
int findState(Cube* cube); //Rotate U 4 times, check each cube state against file
int findEdges(Cube* cube);
void solveCase(Cube* cube, int n);
int checkOLL(Cube* cube);
int checkOLLEdges(Cube* cube);


#endif
