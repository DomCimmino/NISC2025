#include "cube.h"
#include <string.h>

void initCube(Cube* cube, char cube_state[TOTAL_STICKER]){

  for(int j=0; j<TOTAL_STICKER; j++){
      if(j>=0 && j<=8){
        cube->cubies[j][0] = sticketToInt(cube_state[j]);
      }
      if(j>=9 && j<=17){
        cube->cubies[j-9][3] = sticketToInt(cube_state[j]);
      }
      if(j>=18 && j<=26){
        cube->cubies[j-18][2] = sticketToInt(cube_state[j]);
      }
      if(j>=27 && j<=35){
        cube->cubies[j-27][1] = sticketToInt(cube_state[j]);
      }
      if(j>=36 && j<=44){
        cube->cubies[j-36][5] = sticketToInt(cube_state[j]);
      }
      if(j>=45 && j<=53){
        cube->cubies[j-45][4] = sticketToInt(cube_state[j]);
      }
  }
}

void createTempCubies(Cube* cube, int cubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE]){
  for (int i=0;i<NUM_CUBE_FACE;i++) {
    for (int j=0;j<NUM_CUBE_STICKER_PER_FACE;j++) {
      cubies[j][i] = cube->cubies[j][i];
    }
  }
}

void setCube(Cube* cube, int cubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE]){
  for (int i=0;i<NUM_CUBE_FACE;i++) {
    for (int j=0;j<NUM_CUBE_STICKER_PER_FACE;j++) {
      cube->cubies[j][i] = cubies[j][i];
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
  int newCubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE];
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
  int newCubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //White->Orange
      newCubies[2][4] = cube->cubies[6][0];
      newCubies[5][4] = cube->cubies[3][0];
      newCubies[8][4] = cube->cubies[0][0];

      //Orange->Yellow
      newCubies[6][1] = cube->cubies[2][4];
      newCubies[3][1] = cube->cubies[5][4];
      newCubies[0][1] = cube->cubies[8][4];

      //Yellow->Red
      newCubies[0][2] = cube->cubies[0][1];
      newCubies[3][2] = cube->cubies[3][1];
      newCubies[6][2] = cube->cubies[6][1];

      //Red->White
      newCubies[0][0] = cube->cubies[0][2];
      newCubies[3][0] = cube->cubies[3][2];
      newCubies[6][0] = cube->cubies[6][2];

      //Rotate Blue
      newCubies[0][5] = cube->cubies[6][5];
      newCubies[1][5] = cube->cubies[3][5];
      newCubies[2][5] = cube->cubies[0][5];
      newCubies[3][5] = cube->cubies[7][5];
      newCubies[5][5] = cube->cubies[1][5];
      newCubies[6][5] = cube->cubies[8][5];
      newCubies[7][5] = cube->cubies[5][5];
      newCubies[8][5] = cube->cubies[2][5];

      setCube(cube, newCubies);
    }
  }
}

void u(Cube* cube, int n){
  int newCubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Orange->Green
      newCubies[0][3] = cube->cubies[0][4];
      newCubies[1][3] = cube->cubies[1][4];
      newCubies[2][3] = cube->cubies[2][4];

      //Green->Red
      newCubies[0][2] = cube->cubies[0][3];
      newCubies[1][2] = cube->cubies[1][3];
      newCubies[2][2] = cube->cubies[2][3];

      //Red->Blue
      newCubies[0][5] = cube->cubies[0][2];
      newCubies[1][5] = cube->cubies[1][2];
      newCubies[2][5] = cube->cubies[2][2];

      //Blue->Orange
      newCubies[0][4] = cube->cubies[0][5];
      newCubies[1][4] = cube->cubies[1][5];
      newCubies[2][4] = cube->cubies[2][5];

      //Rotate Yellow
      newCubies[0][1] = cube->cubies[6][1];
      newCubies[1][1] = cube->cubies[3][1];
      newCubies[2][1] = cube->cubies[0][1];
      newCubies[3][1] = cube->cubies[7][1];
      newCubies[5][1] = cube->cubies[1][1];
      newCubies[6][1] = cube->cubies[8][1];
      newCubies[7][1] = cube->cubies[5][1];
      newCubies[8][1] = cube->cubies[2][1];

      setCube(cube, newCubies);
    }
  }
}

void d(Cube* cube, int n){
  int newCubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Orange->Blue
      newCubies[6][5] = cube->cubies[6][4];
      newCubies[7][5] = cube->cubies[7][4];
      newCubies[8][5] = cube->cubies[8][4];

      //Blue->Red
      newCubies[6][2] = cube->cubies[6][5];
      newCubies[7][2] = cube->cubies[7][5];
      newCubies[8][2] = cube->cubies[8][5];

      //Red->Green
      newCubies[6][3] = cube->cubies[6][2];
      newCubies[7][3] = cube->cubies[7][2];
      newCubies[8][3] = cube->cubies[8][2];

      //Green->Orange
      newCubies[6][4] = cube->cubies[6][3];
      newCubies[7][4] = cube->cubies[7][3];
      newCubies[8][4] = cube->cubies[8][3];

      //Rotate White
      newCubies[0][0] = cube->cubies[6][0];
      newCubies[1][0] = cube->cubies[3][0];
      newCubies[2][0] = cube->cubies[0][0];
      newCubies[3][0] = cube->cubies[7][0];
      newCubies[5][0] = cube->cubies[1][0];
      newCubies[6][0] = cube->cubies[8][0];
      newCubies[7][0] = cube->cubies[5][0];
      newCubies[8][0] = cube->cubies[2][0];

      setCube(cube, newCubies);
    }
  }
}

void f(Cube* cube, int n){
  int newCubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Blue->Yellow
      newCubies[8][1] = cube->cubies[2][5];
      newCubies[7][1] = cube->cubies[5][5];
      newCubies[6][1] = cube->cubies[8][5];

      //Yellow->Green
      newCubies[0][3] = cube->cubies[6][1];
      newCubies[3][3] = cube->cubies[7][1];
      newCubies[6][3] = cube->cubies[8][1];

      //Green->White
      newCubies[2][0] = cube->cubies[0][3];
      newCubies[1][0] = cube->cubies[3][3];
      newCubies[0][0] = cube->cubies[6][3];

      //White->Blue
      newCubies[2][5] = cube->cubies[0][0];
      newCubies[5][5] = cube->cubies[1][0];
      newCubies[8][5] = cube->cubies[2][0];

      //Rotate Red
      newCubies[0][2] = cube->cubies[6][2];
      newCubies[1][2] = cube->cubies[3][2];
      newCubies[2][2] = cube->cubies[0][2];
      newCubies[3][2] = cube->cubies[7][2];
      newCubies[5][2] = cube->cubies[1][2];
      newCubies[6][2] = cube->cubies[8][2];
      newCubies[7][2] = cube->cubies[5][2];
      newCubies[8][2] = cube->cubies[2][2];

      setCube(cube, newCubies);
    }
  }
}

void b(Cube* cube, int n){
  int newCubies[NUM_CUBE_STICKER_PER_FACE][NUM_CUBE_FACE];
  createTempCubies(cube, newCubies);

  if(n>=0){
    for(int i=0; i<n; i++){

      //Yellow->Blue
      newCubies[6][5] = cube->cubies[0][1];
      newCubies[3][5] = cube->cubies[1][1];
      newCubies[0][5] = cube->cubies[2][1];

      //Blue->White
      newCubies[6][0] = cube->cubies[0][5];
      newCubies[7][0] = cube->cubies[3][5];
      newCubies[8][0] = cube->cubies[6][5];

      //White->Green
      newCubies[8][3] = cube->cubies[6][0];
      newCubies[5][3] = cube->cubies[7][0];
      newCubies[2][3] = cube->cubies[8][0];

      //Green->Yellow
      newCubies[0][1] = cube->cubies[2][3];
      newCubies[1][1] = cube->cubies[5][3];
      newCubies[2][1] = cube->cubies[8][3];

      //Rotate Orange
      newCubies[0][4] = cube->cubies[6][4];
      newCubies[1][4] = cube->cubies[3][4];
      newCubies[2][4] = cube->cubies[0][4];
      newCubies[3][4] = cube->cubies[7][4];
      newCubies[5][4] = cube->cubies[1][4];
      newCubies[6][4] = cube->cubies[8][4];
      newCubies[7][4] = cube->cubies[5][4];
      newCubies[8][4] = cube->cubies[2][4];

      setCube(cube, newCubies);
    }
  }
}

void output(Cube* cube, char cube_state[TOTAL_STICKER]){
  int idx = 0;

      // U face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[j][0]);

      // R face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[j][3]);

      // F face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[j][2]);

      // D face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[j][1]);

      // L face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[j][5]);

      // B face
      for (int j = 0; j < 9; j++)
          cube_state[idx++] = intToSticker(cube->cubies[j][4]);
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
