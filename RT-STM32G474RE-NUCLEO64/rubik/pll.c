#include "pll.h"
#include "cube.h"

void solvePLL(Cube* cube) {
  adjustUFace(cube);
  while (!checkPLLCorners(cube)) {
    solvePLLCorners(cube);
    adjustUFace(cube);
  }

  while (!checkPLLEdges(cube)) {
    solvePLLEdges(cube);
  }
}

void adjustUFace(Cube* cube) {
  while (cube->cubies[2][5] != 5 || cube->cubies[0][2] != 2) {
    moves(cube, "U");
  }
}

void solvePLLEdges(Cube* cube) {
  if (cube->cubies[1][2] == 5 && cube->cubies[1][3] == 2 && cube->cubies[1][4] == 4 && cube->cubies[1][5] == 3) {
    moves(cube, "RRURURRRUUURRRUUURRRURRR");
  }
  else if (cube->cubies[1][2] == 3 && cube->cubies[1][3] == 5 && cube->cubies[1][4] == 4 && cube->cubies[1][5] == 2) {
    moves(cube, "RUUURURURUUURRRUUURR");
  }
  else if (cube->cubies[1][2] == 4 && cube->cubies[1][3] == 5 && cube->cubies[1][4] == 2 && cube->cubies[1][5] == 3) {
    moves(cube, "RRLLDRRLLUURRLLDRRLL");
  }
  else if (cube->cubies[1][2] == 3 && cube->cubies[1][3] == 2 && cube->cubies[1][4] == 5 && cube->cubies[1][5] == 4) {
    moves(cube, "URRRUUURUUURURUUURRRURURRUUURRRU");
  }
  else {
    moves(cube, "RRURURRRUUURRRUUURRRURRR");
    adjustUFace(cube);
    moves(cube, "RRLLDRRLLUURRLLDRRLL");
    moves(cube, "RRURURRRUUURRRUUURRRURRR");
    adjustUFace(cube);
  }
}

void solvePLLCorners(Cube* cube) {
  if (cube->cubies[2][2] == 4 && cube->cubies[0][3] == 5 && cube->cubies[2][3] == 2
  && cube->cubies[0][4] == 3 && cube->cubies[2][4] == 3 && cube->cubies[0][5] == 4) {
    moves(cube, "RRRFRRRBBRFFFRRRBBRR");
  }
  else if (cube->cubies[2][2] == 4 && cube->cubies[0][3] == 5 && cube->cubies[2][3] == 3
  && cube->cubies[0][4] == 4 && cube->cubies[2][4] == 2 && cube->cubies[0][5] == 3) {
    moves(cube, "RBBBRRRFRBRRRFFLLLBLFLLLBBBL");
  }
  else {
    moves(cube, "RRRFRRRBBRFFFRRRBBRR");
  }
}

int checkPLLCorners(Cube* cube) {
  if (cube->cubies[0][2] != 2 || cube->cubies[2][2] != 2 || cube->cubies[0][3] != 3 || cube->cubies[2][3] != 3 || cube->cubies[0][4] != 4 || cube->cubies[2][4] != 4 || cube->cubies[0][5] != 5 || cube->cubies[2][5] != 5) {
    return 0;
  }
  else
    return 1;
}

int checkPLLEdges(Cube* cube) {
    if (cube->cubies[1][2] == 2 && cube->cubies[1][3] == 3 && cube->cubies[1][4] == 4 && cube->cubies[1][5] == 5) {
      return 1;
    }
    else {
      return 0;
    }
}
