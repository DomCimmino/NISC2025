#include "corners.h"
#include "cube.h"

void solveCorners(Cube* cube) {
  while (checkBottomLayer(cube)) {

    //get a white corner into the top layer

    if (cube->cubies[0][0] == 0 || cube->cubies[6][2] == 0 || cube->cubies[8][5] == 0) {
      while (cube->cubies[6][1] == 0 || cube->cubies[0][2] == 0 || cube->cubies[2][5] == 0 ) {
        moves(cube, "U");
      }
      moves(cube, "FUFFF");

    }
    if (cube->cubies[2][0] == 0 || cube->cubies[8][2] == 0 || cube->cubies[6][3] == 0) {
      while (cube->cubies[8][1] == 0 || cube->cubies[2][2] == 0 || cube->cubies[0][3] == 0 ) {
        moves(cube, "U");
      }
      moves(cube, "RURRR");

    }
    if (cube->cubies[6][0] == 0 || cube->cubies[8][4] == 0 || cube->cubies[6][5] == 0) {
      while (cube->cubies[0][1] == 0 || cube->cubies[2][4] == 0 || cube->cubies[0][5] == 0 ) {
        moves(cube, "U");
      }
      moves(cube, "LULLL");

    }
    if (cube->cubies[8][0] == 0 || cube->cubies[8][3] == 0 || cube->cubies[6][4] == 0) {
      while (cube->cubies[2][1] == 0 || cube->cubies[2][3] == 0 || cube->cubies[0][4] == 0 ) {
        moves(cube, "U");
      }
      moves(cube, "BUBBB");

    }
  }


  while (checkCorners(cube)) {

    //then perform the alg for the case: RUURRRUUURURRR, RURRR, FFFUUUF

    if (cube->cubies[8][1] == 0) {
      if (cube->cubies[2][2] == 2) { //red and blue
        moves(cube, "UFUUFFFUUUFUFFF");
      }
      else if (cube->cubies[2][2] == 3) { //green and red
        moves(cube, "RUURRRUUURURRR");
      }
      else if (cube->cubies[2][2] == 4) { //orange and green
        moves(cube, "UUUBUUBBBUUUBUBBB");
      }
      else if (cube->cubies[2][2] == 5){ //orange and blue
        moves(cube, "UULUUULLLUULULLL");
      }


    }

    else if (cube->cubies[0][3] == 0) {
        if (cube->cubies[2][2] == 2) { //red green
          moves(cube, "RURRR");
        }
        else if (cube->cubies[2][2] == 3) { //green orange
          moves(cube, "UUUBUBBB");
        }
        else if (cube->cubies[2][2] == 4) { //orange blue
          moves(cube, "UULULLL");
        }
        else if (cube->cubies[2][2] == 5) { //red blue
          moves(cube, "UFUFFF");
        }
    }

    else if (cube->cubies[2][2] == 0){
      if (cube->cubies[0][3] == 2) { //blue red
        moves(cube, "ULLLUUUL");
      }
      else if (cube->cubies[0][3] == 3) { //red green
        moves(cube, "FFFUUUF");
      }
      else if (cube->cubies[0][3] == 4) { //orange green
        moves(cube, "URRRUUUR");
      }
      else if (cube->cubies[0][3] == 5) { //orange blue
        moves(cube, "UUBBBUUUB");
      }

    }
    else {
      moves(cube, "U");
    }

  }
}


int checkCorners(Cube* cube) {
  if (!cube->cubies[0][0] &&
      !cube->cubies[2][0] &&
      !cube->cubies[6][0] &&
      !cube->cubies[8][0]) {

    return 0;
  } else {
    return 1;
  }
}

int checkBottomLayer(Cube* cube) {
  if (cube->cubies[0][0] &&
      cube->cubies[2][0] &&
      cube->cubies[6][0] &&
      cube->cubies[8][0] &&
      cube->cubies[6][2] &&
      cube->cubies[8][2] &&
      cube->cubies[6][3] &&
      cube->cubies[8][3] &&
      cube->cubies[6][4] &&
      cube->cubies[8][4] &&
      cube->cubies[6][5] &&
      cube->cubies[8][5]) {

    return 0;
  } else if (!cube->cubies[0][0] &&
           !cube->cubies[2][0] &&
           !cube->cubies[6][0] &&
           !cube->cubies[8][0]) {
    return 0;
  } else {
    return 1;
  }
}
