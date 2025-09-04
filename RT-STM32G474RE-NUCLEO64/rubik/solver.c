#include "solver.h"
#include "cube.h"
#include "cross.h"
#include "corners.h"
#include "edges.h"
#include "oll.h"
#include "pll.h"
#include "display/display.h"

void solveCube(Cube* cube){
  char cube_state[TOTAL_STICKER];

  displayString("Solve cross");
  solveCross(cube);
  output(cube, cube_state);
  rubikDrawNetFromCube(cube_state, 10, 25);

  displayString("Solve corners");
  solveCorners(cube);
  output(cube, cube_state);
  rubikDrawNetFromCube(cube_state, 10, 25);

  displayString("Solve edges");
  solveEdges(cube);
  output(cube, cube_state);
  rubikDrawNetFromCube(cube_state, 10, 25);

  displayString("Solve oll");
  solveOLL(cube);
  output(cube, cube_state);
  rubikDrawNetFromCube(cube_state, 10, 25);

  displayString("Solve pll");
  solvePLL(cube);
  output(cube, cube_state);
  rubikDrawNetFromCube(cube_state, 10, 25);

}
