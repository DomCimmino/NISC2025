#include "edges.h"
#include "cube.h"

static void solveEdges(Cube* cube) {
  while (!checkEdges(cube)) {
    solveEdge(cube);
  }
}

void solveEdge(Cube* cube) {
  if (cube->cubies[1][1] != 1 && cube->cubies[1][4] != 1) {
    if (cube->cubies[1][1] == 2) {
      if (cube->cubies[1][4] == 3) {
        moves(cube, "URRRUUURRRUUURRRURUR");
      }
      else if (cube->cubies[1][4] == 5) {
        moves(cube, "BUBUBUUUBBBUUUBBB");
      }
    }
    else if (cube->cubies[1][1] == 3) {
      if (cube->cubies[1][4] == 4) {
        moves(cube, "BBBUUUBBBUUUBBBUBUB");
      }
      else if (cube->cubies[1][4] == 2) {
        moves(cube, "UUFUFUFUUUFFFUUUFFF");
      }
    }
    else if (cube->cubies[1][1] == 4) {
      if (cube->cubies[1][4] == 5) {
        moves(cube, "UUULLLUUULLLUUULLLULUL");
      }
      else if (cube->cubies[1][4] == 3) {
        moves(cube, "BBBUUUBBBUUUBBBUBUB");
      }
    }
    else if (cube->cubies[1][1] == 5) {
      if (cube->cubies[1][4] == 2) {
        moves(cube, "UUFFFUUUFFFUUUFFFUFUF");
      }
      else if (cube->cubies[1][4] == 4) {
        moves(cube, "LLLUUULLLUUULLLULUL");
      }
    }
  } else if (cube->cubies[5][1] != 1 && cube->cubies[1][3] != 1) {
    if (cube->cubies[5][1] == 2) {
      if (cube->cubies[1][3] == 3) {
        moves(cube, "RRRUUURRRUUURRRURUR");
      }
      else if (cube->cubies[1][3] == 5) {
        moves(cube, "UULULULUUULLLUUULLL");
      }
    }
    else if (cube->cubies[5][1] == 3) {
      if (cube->cubies[1][3] == 4) {
        moves(cube, "UUUBBBUUUBBBUUUBBBUBUB");
      }
      else if (cube->cubies[1][3] == 2) {
        moves(cube, "UFUFUFUUUFFFUUUFFF");
      }
    }
    else if (cube->cubies[5][1] == 4) {
      if (cube->cubies[1][3] == 5) {
        moves(cube, "UULLLUUULLLUUULLLULUL");
      }
      else if (cube->cubies[1][3] == 3) {
        moves(cube, "RURURUUURRRUUURRR");
      }
    }
    else if (cube->cubies[5][1] == 5) {
      if (cube->cubies[1][3] == 2) {
        moves(cube, "UFFFUUUFFFUUUFFFUFUF");
      }
      else if (cube->cubies[1][3] == 4) {
        moves(cube, "UUUBUBUBUUUBBBUUUBBB");
      }
    }
  }  else if (cube->cubies[7][1] != 1 && cube->cubies[1][2] != 1) {
    if (cube->cubies[7][1] == 2) {
      if (cube->cubies[1][2] == 3) {
        moves(cube, "UUURRRUUURRRUUURRRURUR");
      }
      else if (cube->cubies[1][2] == 5) {
        moves(cube, "ULULULUUULLLUUULLL");
      }
    }
    else if (cube->cubies[7][1] == 3) {
      if (cube->cubies[1][2] == 4) {
        moves(cube, "UUBBBUUUBBBUUUBBBUBUB");
      }
      else if (cube->cubies[1][2] == 2) {
        moves(cube, "FUFUFUUUFFFUUUFFF");
      }
    }
    else if (cube->cubies[7][1] == 4) {
      if (cube->cubies[1][2] == 5) {
        moves(cube, "ULLLUUULLLUUULLLULUL");
      }
      else if (cube->cubies[1][2] == 3) {
        moves(cube, "UUURURURUUURRRUUURRR");
      }
    }
    else if (cube->cubies[7][1] == 5) {
      if (cube->cubies[1][2] == 2) {
        moves(cube, "FFFUUUFFFUUUFFFUFUF");
      }
      else if (cube->cubies[1][2] == 4) {
        moves(cube, "UUBUBUBUUUBBBUUUBBB");
      }
    }
  } else if (cube->cubies[3][1] != 1 && cube->cubies[1][5] != 1) {
    if (cube->cubies[3][1] == 2) {
      if (cube->cubies[1][5] == 3) {
        moves(cube, "UURRRUUURRRUUURRRURUR");
      }
      else if (cube->cubies[1][5] == 5) {
        moves(cube, "LULULUUULLLUUULLL");
      }
    }
    else if (cube->cubies[3][1] == 3) {
      if (cube->cubies[1][5] == 4) {
        moves(cube, "UBBBUUUBBBUUUBBBUBUB");
      }
      else if (cube->cubies[1][5] == 2) {
        moves(cube, "UUUFUFUFUUUFFFUUUFFF");
      }
    }
    else if (cube->cubies[3][1] == 4) {
      if (cube->cubies[1][5] == 5) {
        moves(cube, "LLLUUULLLUUULLLULUL");
      }
      else if (cube->cubies[1][5] == 3) {
        moves(cube, "UURURURUUURRRUUURRR");
      }
    }
    else if (cube->cubies[3][1] == 5) {
      if (cube->cubies[1][5] == 2) {
        moves(cube, "UUUFFFUUUFFFUUUFFFUFUF");
      }
      else if (cube->cubies[1][5] == 4) {
        moves(cube, "UBUBUBUUUBBBUUUBBB");
      }
    }
  } else if (cube->cubies[3][2] != 2 || cube->cubies[5][5] != 5) {
    moves(cube, "LULULUUULLLUUULLL");
  } else if (cube->cubies[5][2] != 2 || cube->cubies[3][3] != 3) {
    moves(cube, "FUFUFUUUFFFUUUFFF");
  } else if (cube->cubies[5][3] != 3 || cube->cubies[3][4] != 4) {
    moves(cube, "RURURUUURRRUUURRR");
  } else if (cube->cubies[5][4] != 4 || cube->cubies[3][5] != 5) {
    moves(cube, "BUBUBUUUBBBUUUBBB");
  }
}

int checkEdges(Cube* cube) {
  if (cube->cubies[3][2] != 2 || cube->cubies[5][2] != 2)
    return 0;
  else if (cube->cubies[3][3] != 3 || cube->cubies[5][3] != 3)
    return 0;
  else if (cube->cubies[3][4] != 4 || cube->cubies[5][4] != 4)
    return 0;
  else if (cube->cubies[3][5] != 5 || cube->cubies[5][5] != 5)
    return 0;
  else
    return 1;
}

