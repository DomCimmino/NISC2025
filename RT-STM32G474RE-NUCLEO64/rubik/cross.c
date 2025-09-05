#include "cross.h"
#include "cube.h"

void solveCross(Cube* cube){
  while (checkCross(cube)) {
    solveCrossEdge(cube);
    }
}

int checkCross(Cube* cube){
  if (!cube->cubies[1][0] &&
      !cube->cubies[3][0] &&
      !cube->cubies[5][0] &&
      !cube->cubies[7][0] &&
      cube->cubies[7][2] == 2 &&
      cube->cubies[7][3] == 3 &&
      cube->cubies[7][4] == 4 &&
      cube->cubies[7][5] == 5) {
    return 0;
  }
  else
    return 1;
}

void solveCrossEdge(Cube* cube){
  for (int side=0; side<6; ++side) {
    for (int edge = 1; edge<9; edge+=2) {
      if (cube->cubies[side][edge] == 0) {
        //Solve edge

        switch (side) {

          case 1: {//White edge on top
            if (edge == 1) { //orange side
              switch (cube->cubies[1][4]) {
                case 2://red
                  moves(cube, "UUFF");
                  break;
                case 3://green
                  moves(cube, "URR");
                  break;
                case 4:
                  moves(cube, "BB");
                  break;
                case 5:
                  moves(cube, "UUULL");
                  break;
              }
            }
            else if (edge == 3) {//Blue side
              switch (cube->cubies[1][5]) {
                case 2:
                  moves(cube, "UUUFF");
                  break;
                case 3://green
                  moves(cube, "UURR");
                  break;
                case 4:
                  moves(cube, "UBB");
                  break;
                case 5:
                  moves(cube, "LL");
                  break;
              }
            }
            else if (edge == 5) {//green side
              switch(cube->cubies[1][3]) {
                  case 2://red
                    moves(cube, "UFF");
                    break;
                  case 3:
                    moves(cube, "RR");
                    break;
                  case 4:
                    moves(cube, "UUUBB");
                    break;
                  case 5:
                    moves(cube, "UULL");
                    break;
              }
            }
            else if (edge == 7) {//red side
              switch (cube->cubies[1][2]) {
                  case 2:
                    moves(cube, "FF");
                    break;
                  case 3:
                    moves(cube, "UUURR");
                    break;
                  case 4:
                    moves(cube, "UUBB");
                    break;
                  case 5:
                    moves(cube, "ULL");
                    break;
              }
            }
            break;
          }
          case 2: {//White edge on red side

            if (edge == 1) {//Yellow side
              switch(cube->cubies[7][1]) {
                case 2:
                  moves(cube, "UUURRRFR");
                  break;
                case 3://green on top, white on red side
                  moves(cube, "FRRRFFF");
                  break;
                case 4:
                  moves(cube, "UUURBBBRRR");
                  break;
                case 5:
                  moves(cube, "FFFLF");
                  break;
              }
            }
            else if (edge == 3) { //Colored edge on blue side
              switch(cube->cubies[5][5]) {
                case 2:
                  moves(cube, "LDLLL");
                  break;
                case 3:
                  moves(cube, "UUFUU");
                  break;
                case 4:
                  moves(cube, "LLLULBB");
                  break;
                case 5:
                  moves(cube, "L");
                  break;

              }
            }
            else if (edge == 5) { //check color on green side
              switch(cube->cubies[3][3]) {
              case 2:
                moves(cube, "RRRDDDR");
                break;
              case 3:
                moves(cube, "RRR");
                break;
              case 4:
                moves(cube, "RUUURRRBB");
                break;
              case 5:
                moves(cube, "RUURRRLL");
                break;
              }
            }
            else if (edge == 7) { //check white side
              switch (cube->cubies[1][0]) {
              case 2:
                moves(cube, "FFFDRRRDDD");
                break;
              case 3:
                moves(cube, "FFFRRR");
                break;
              case 4:
                moves(cube, "FFUUURBBBRRR");
                break;
              case 5:
                moves(cube, "FL");
                break;
              }
            }

            break;
          }
          case 3: //White edge on green side
            if (edge == 1) {//Yellow side
              switch(cube->cubies[5][1]) {
                case 2:
                  moves(cube, "RRRFR");
                  break;
                case 3:
                  moves(cube, "UFRRRFFF");
                  break;
                case 4:
                  moves(cube, "RBBBRRR");
                  break;
                case 5:
                  moves(cube, "UFFFLF");
                  break;
                }
              }
              else if (edge == 3) { //Colored edge on red side
                switch(cube->cubies[5][2]) {
                  case 2:
                    moves(cube, "F");
                    break;
                  case 3:
                    moves(cube, "FFFUUUFRR");
                    break;
                  case 4:
                    moves(cube, "RRBBBRR");
                    break;
                  case 5:
                    moves(cube, "FFFUFLL");
                    break;

                }
              }
              else if (edge == 5) { //check color on orange side
                switch(cube->cubies[3][4]) {
                  case 2:
                    moves(cube, "RRFRR");
                    break;
                  case 3:
                    moves(cube, "BUBBBRR");
                    break;
                  case 4:
                    moves(cube, "BBB");
                    break;
                  case 5:
                    moves(cube, "RRRUFFFLF");
                    break;
                }
              }
              else if (edge == 7) { //check white side
                switch (cube->cubies[5][0]) {
                  case 2:
                    moves(cube, "RFRRR");
                    break;
                  case 3:
                    moves(cube, "RRUFRRRFFF");
                    break;
                  case 4:
                    moves(cube, "RRRBBBR");
                    break;
                  case 5:
                    moves(cube, "RRUFFFLF");
                    break;
                }
              }
            break;
          case 4: //White edge on orange side
            if (edge == 1) {//Yellow side
              switch(cube->cubies[1][1]) {
                case 2:
                    moves(cube, "URRRFR");
                    break;
                  case 3:
                    moves(cube, "BBBRB");
                    break;
                  case 4:
                    moves(cube, "URBBBRRR");
                    break;
                  case 5:
                    moves(cube, "BLLLBBB");
                    break;
                  }
                }
                else if (edge == 3) { //Colored edge on green side
                  switch(cube->cubies[5][3]) {
                    case 2:
                      moves(cube, "RRRURFF");
                      break;
                    case 3:
                        moves(cube, "R");
                        break;
                    case 4:
                        moves(cube, "RRRUUURBB");
                        break;
                    case 5:
                        moves(cube, "RRRUURLL");
                        break;

                        }
                      }
                else if (edge == 5) { //check color on blue side
                  switch(cube->cubies[3][5]) {
                    case 2:
                      moves(cube, "LUUULLLFF");
                      break;
                    case 3:
                      moves(cube, "LUULLLRR");
                      break;
                    case 4:
                      moves(cube, "LULLLBB");
                      break;
                    case 5:
                      moves(cube, "LLL");
                      break;
                    }
                  }
                  else if (edge == 7) { //check white side
                    switch (cube->cubies[7][0]) {
                      case 2:
                        moves(cube, "BBURRRFR");
                        break;
                      case 3:
                        moves(cube, "BRBBB");
                        break;
                      case 4:
                        moves(cube, "BBURBBBRRR");
                        break;
                      case 5:
                        moves(cube, "BBBLLLB");
                        break;
                      }
                    }
              break;
          case 5: //White edge on blue side

          if (edge == 1) {//Yellow side
            switch(cube->cubies[3][1]) {
              case 2:
                moves(cube, "LFLLL");
                break;
              case 3:
                moves(cube, "UUUFRRRFFF");
                break;
              case 4:
                moves(cube, "LLLBL");
                break;
              case 5:
                moves(cube, "UUUFFFLF");
                break;
            }
          }
          else if (edge == 3) { //Colored edge on orange side
            switch(cube->cubies[5][4]) {
              case 2:
                moves(cube, "LLFFFLL");
                break;
              case 3:
                moves(cube, "LUUUFRRRFFF");
                break;
              case 4:
                moves(cube, "B");
                break;
              case 5:
                moves(cube, "LUUUFFFLF");
                break;

            }
          }
          else if (edge == 5) { //check color on red side
            switch(cube->cubies[3][2]) {
            case 2:
              moves(cube, "FFF");
              break;
            case 3:
              moves(cube, "FUUUFFFRR");
              break;
            case 4:
              moves(cube, "FUUFFFBB");
              break;
            case 5:
              moves(cube, "FUFFFLL");
              break;
            }
          }
          else if (edge == 7) { //check white side
            switch (cube->cubies[3][0]) {
            case 2:
              moves(cube, "LLLFFFL");
              break;
            case 3:
              moves(cube, "LLUFRRRFFF");
              break;
            case 4:
              moves(cube, "LBLLL");
              break;
            case 5:
              moves(cube, "LLUFFFLF");
              break;
            }
          }

            break;

          case 0:
            if (edge == 1) {
              if (cube->cubies[7][2] != 2) {
                while (cube->cubies[1][2] == 0 || cube->cubies[7][1] == 0) {
                  moves(cube, "U");
                }
                moves(cube, "FF");
              }
            }
            else if (edge == 5) {
              if (cube->cubies[7][3] != 3) {
                while (cube->cubies[1][3] == 0 || cube->cubies[5][1] == 0) {
                  moves(cube, "U");
                }
                moves(cube, "RR");
              }
            }
            else if (edge == 7) {
              if (cube->cubies[7][4] != 4) {
                while (cube->cubies[1][3] == 0 || cube->cubies[1][1] == 0) {
                  moves(cube, "U");
                }
                moves(cube, "BB");
              }
            }
            else if (edge == 3) {
              if (cube->cubies[7][5] != 5) {
                while (cube->cubies[1][5] == 0 || cube->cubies[3][1] == 0) {
                  moves(cube, "U");
                }
                moves(cube, "LL");
              }
            }

            break;
        }
      }
    }
  }

}
