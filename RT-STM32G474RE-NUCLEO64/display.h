#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

#define R_SIZE 3
#define FACE_CNT 6

void display_init(void);

void drawCubeNet(uint8_t cube_state[54], int ox, int oy);

void handle_navigation(char cube_state[54]);

typedef enum { FACE_U=0, FACE_R, FACE_F, FACE_D, FACE_L, FACE_B } RubikFace;
typedef enum { COL_W=0, COL_R, COL_G, COL_Y, COL_O, COL_B, COL_CNT } RubikColor;

#endif
