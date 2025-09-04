#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "gfx.h"

#define R_SIZE          3
#define FACE_CNT 6
#define CELL            18
#define GAP             2
#define FACE_BORDER     2
#define NET_GAP         8

void displayInit(void);

void rubikDrawNetFromCube(char cube_state[54], gCoord ox, gCoord oy);

void displayString(const char* message);

typedef enum { FACE_U=0, FACE_R, FACE_F, FACE_D, FACE_L, FACE_B } RubikFace;
typedef enum { COL_W=0, COL_R, COL_G, COL_Y, COL_O, COL_B, COL_CNT } RubikColor;

#endif
