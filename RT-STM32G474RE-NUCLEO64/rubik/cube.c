#include "cube.h"
#include <string.h>

void initCube(Cube* cube, char cube_state[TOTAL_STICKER]){

  for(int j=0; j<TOTAL_STICKER; j++){
      if(j>=0 && j<=8){
        cube->cubies[U][j] = sticketToInt(cube_state[j]);
      }
      if(j>=9 && j<=17){
        cube->cubies[R][j-9] = sticketToInt(cube_state[j]);
      }
      if(j>=18 && j<=26){
        cube->cubies[F][j-18] = sticketToInt(cube_state[j]);
      }
      if(j>=27 && j<=35){
        cube->cubies[D][j-27] = sticketToInt(cube_state[j]);
      }
      if(j>=36 && j<=44){
        cube->cubies[L][j-36] = sticketToInt(cube_state[j]);
      }
      if(j>=45 && j<=53){
        cube->cubies[B][j-45] = sticketToInt(cube_state[j]);
      }
  }
}

void createTempCubies(Cube* cube, int cubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE]){
  for (int i=0;i<NUM_CUBE_FACE;i++) {
    for (int j=0;j<NUM_CUBE_STICKER_PER_FACE;j++) {
      cubies[i][j] = cube->cubies[i][j];
    }
  }
}

void setCube(Cube* cube, int cubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE]){
  for (int i=0;i<NUM_CUBE_FACE;i++) {
    for (int j=0;j<NUM_CUBE_STICKER_PER_FACE;j++) {
      cube->cubies[i][j] = cubies[i][j];
    }
  }
}

void moves(Cube* cube, const char* moves){
  for (size_t i=0; i<strlen(moves); ++i) {
      switch (moves[i]) {
        case 'U':
          u(cube, 1);
          break;
        case 'D':
          d(cube, 1);
          break;
        case 'R':
          r(cube, 1);
          break;
        case 'L':
          l(cube, 1);
          break;
        case 'F':
          f(cube, 1);
          break;
        case 'B':
          b(cube, 1);
          break;
      }
    }
}

void r(Cube* cube, int n){
  int newCubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Orange->White
      newCubies[2][0] = cube->cubies[6][4];
      newCubies[5][0] = cube->cubies[3][4];
      newCubies[8][0] = cube->cubies[0][4];

      //White->Red
      newCubies[2][2] = cube->cubies[2][0];
      newCubies[5][2] = cube->cubies[5][0];
      newCubies[8][2] = cube->cubies[8][0];

      //Red->Yellow
      newCubies[2][1] = cube->cubies[2][2];
      newCubies[5][1] = cube->cubies[5][2];
      newCubies[8][1] = cube->cubies[8][2];

      //Yellow->Orange
      newCubies[6][4] = cube->cubies[2][1];
      newCubies[3][4] = cube->cubies[5][1];
      newCubies[0][4] = cube->cubies[8][1];

      //Rotate Green
      newCubies[0][3] = cube->cubies[6][3];
      newCubies[1][3] = cube->cubies[3][3];
      newCubies[2][3] = cube->cubies[0][3];
      newCubies[3][3] = cube->cubies[7][3];
      newCubies[5][3] = cube->cubies[1][3];
      newCubies[6][3] = cube->cubies[8][3];
      newCubies[7][3] = cube->cubies[5][3];
      newCubies[8][3] = cube->cubies[2][3];
    }
    setCube(cube, newCubies);
  }
}

void l(Cube* cube, int n){
  int newCubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //White->Ora[NUM_CUBE_FACE]nge
      newCubies[4][2] = cube->cubies[0][6];
      newCubies[4][5] = cube->cubies[0][3];
      newCubies[4][8] = cube->cubies[0][0];

      //Orange->Yellow
      newCubies[1][6] = cube->cubies[4][2];
      newCubies[1][3] = cube->cubies[4][5];
      newCubies[1][0] = cube->cubies[4][8];

      //Yellow->Red
      newCubies[2][0] = cube->cubies[1][0];
      newCubies[2][3] = cube->cubies[1][3];
      newCubies[2][6] = cube->cubies[1][6];

      //Red->White
      newCubies[0][0] = cube->cubies[2][0];
      newCubies[0][3] = cube->cubies[2][3];
      newCubies[0][6] = cube->cubies[2][6];

      //Rotate Blue
      newCubies[5][0] = cube->cubies[5][6];
      newCubies[5][1] = cube->cubies[5][3];
      newCubies[5][2] = cube->cubies[5][0];
      newCubies[5][3] = cube->cubies[5][7];
      newCubies[5][5] = cube->cubies[5][1];
      newCubies[5][6] = cube->cubies[5][8];
      newCubies[5][7] = cube->cubies[5][5];
      newCubies[5][8] = cube->cubies[5][2];

      setCube(cube, newCubies);
    }
  }
}

void u(Cube* cube, int n){
  int newCubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Orange->Green
      newCubies[3][0] = cube->cubies[4][0];
      newCubies[3][1] = cube->cubies[4][1];
      newCubies[3][2] = cube->cubies[4][2];

      //Green->Red
      newCubies[2][0] = cube->cubies[3][0];
      newCubies[2][1] = cube->cubies[3][1];
      newCubies[2][2] = cube->cubies[3][2];

      //Red->Blue
      newCubies[5][0] = cube->cubies[2][0];
      newCubies[5][1] = cube->cubies[2][1];
      newCubies[5][2] = cube->cubies[2][2];

      //Blue->Orange
      newCubies[4][0] = cube->cubies[5][0];
      newCubies[4][1] = cube->cubies[5][1];
      newCubies[4][2] = cube->cubies[5][2];

      //Rotate Yellow
      newCubies[1][0] = cube->cubies[1][6];
      newCubies[1][1] = cube->cubies[1][3];
      newCubies[1][2] = cube->cubies[1][0];
      newCubies[1][3] = cube->cubies[1][7];
      newCubies[1][5] = cube->cubies[1][1];
      newCubies[1][6] = cube->cubies[1][8];
      newCubies[1][7] = cube->cubies[1][5];
      newCubies[1][8] = cube->cubies[1][2];

      setCube(cube, newCubies);
    }
  }
}

void d(Cube* cube, int n){
  int newCubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Orange->Blue
      newCubies[5][6] = cube->cubies[4][6];
      newCubies[5][7] = cube->cubies[4][7];
      newCubies[5][8] = cube->cubies[4][8];

      //Blue->Red
      newCubies[2][6] = cube->cubies[5][6];
      newCubies[2][7] = cube->cubies[5][7];
      newCubies[2][8] = cube->cubies[5][8];

      //Red->Green
      newCubies[3][6] = cube->cubies[2][6];
      newCubies[3][7] = cube->cubies[2][7];
      newCubies[3][8] = cube->cubies[2][8];

      //Green->Orange
      newCubies[4][6] = cube->cubies[3][6];
      newCubies[4][7] = cube->cubies[3][7];
      newCubies[4][8] = cube->cubies[3][8];

      //Rotate White
      newCubies[0][0] = cube->cubies[0][6];
      newCubies[0][1] = cube->cubies[0][3];
      newCubies[0][2] = cube->cubies[0][0];
      newCubies[0][3] = cube->cubies[0][7];
      newCubies[0][5] = cube->cubies[0][1];
      newCubies[0][6] = cube->cubies[0][8];
      newCubies[0][7] = cube->cubies[0][5];
      newCubies[0][8] = cube->cubies[0][2];

      setCube(cube, newCubies);
    }
  }
}

void f(Cube* cube, int n){
  int newCubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Blue->Yellow
      newCubies[1][8] = cube->cubies[5][2];
      newCubies[1][7] = cube->cubies[5][5];
      newCubies[1][6] = cube->cubies[5][8];

      //Yellow->Green
      newCubies[3][0] = cube->cubies[1][6];
      newCubies[3][3] = cube->cubies[1][7];
      newCubies[3][6] = cube->cubies[1][8];

      //Green->White
      newCubies[0][2] = cube->cubies[3][0];
      newCubies[0][1] = cube->cubies[3][3];
      newCubies[0][0] = cube->cubies[3][6];

      //White->Blue
      newCubies[5][2] = cube->cubies[0][0];
      newCubies[5][5] = cube->cubies[0][1];
      newCubies[5][8] = cube->cubies[0][2];

      //Rotate Red
      newCubies[2][0] = cube->cubies[2][6];
      newCubies[2][1] = cube->cubies[2][3];
      newCubies[2][2] = cube->cubies[2][0];
      newCubies[2][3] = cube->cubies[2][7];
      newCubies[2][5] = cube->cubies[2][1];
      newCubies[2][6] = cube->cubies[2][8];
      newCubies[2][7] = cube->cubies[2][5];
      newCubies[2][8] = cube->cubies[2][2];

      setCube(cube, newCubies);
    }
  }
}

void b(Cube* cube, int n){
  int newCubies[NUM_CUBE_FACE][NUM_CUBE_STICKER_PER_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Yellow->Blue
      newCubies[5][6] = cube->cubies[1][0];
      newCubies[5][3] = cube->cubies[1][1];
      newCubies[5][0] = cube->cubies[1][2];

      //Blue->White
      newCubies[0][6] = cube->cubies[5][0];
      newCubies[0][7] = cube->cubies[5][3];
      newCubies[0][8] = cube->cubies[5][6];

      //White->Green
      newCubies[3][8] = cube->cubies[0][6];
      newCubies[3][5] = cube->cubies[0][7];
      newCubies[3][2] = cube->cubies[0][8];

      //Green->Yellow
      newCubies[1][0] = cube->cubies[3][2];
      newCubies[1][1] = cube->cubies[3][5];
      newCubies[1][2] = cube->cubies[3][8];

      //Rotate Orange
      newCubies[4][0] = cube->cubies[4][6];
      newCubies[4][1] = cube->cubies[4][3];
      newCubies[4][2] = cube->cubies[4][0];
      newCubies[4][3] = cube->cubies[4][7];
      newCubies[4][5] = cube->cubies[4][1];
      newCubies[4][6] = cube->cubies[4][8];
      newCubies[4][7] = cube->cubies[4][5];
      newCubies[4][8] = cube->cubies[4][2];

      setCube(cube, newCubies);
    }
  }
}

void output(Cube* cube, char cube_state[TOTAL_STICKER]){
  int idx = 0;

      // U face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[U][j]);

      // R face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[R][j]);

      // F face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[F][j]);

      // D face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[D][j]);

      // L face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[L][j]);

      // B face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[B][j]);
}

int sticketToInt(char sticker){
  switch(sticker){
  case 'W':
    return 0;
  case 'Y':
    return 1;
  case 'R':
    return 2;
  case 'G':
    return 3;
  case 'O':
    return 4;
  case 'B':
    return 5;
  default:
    return -1;
  }
}

char intToSticker(int color){
  switch(color){
  case 0:
    return 'W';
  case 1:
    return 'Y';
  case 2:
    return 'R';
  case 3:
    return 'G';
  case 4:
    return 'O';
  case 5:
    return 'B';
  default:
    return '-';
  }
}
