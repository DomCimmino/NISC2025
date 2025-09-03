#include "display.h"
#include "gfx.h"
#include <stdlib.h>
#include <string.h>

#define R_SIZE          3
#define CELL            18
#define GAP             2
#define FACE_BORDER     2
#define NET_GAP         8

static gColor kPalette[COL_CNT];
static unsigned char stickers[FACE_CNT][R_SIZE][R_SIZE];

static void initPalette(void) {
    kPalette[COL_W] = GFX_WHITE;
    kPalette[COL_Y] = GFX_YELLOW;
    kPalette[COL_R] = GFX_RED;
    kPalette[COL_O] = GFX_ORANGE;
    kPalette[COL_B] = GFX_BLUE;
    kPalette[COL_G] = HTML2COLOR(0x009000);
}

void rubikInit(void) {
    initPalette();
    srand(gfxSystemTicks());
    for (int f=0; f<FACE_CNT; ++f)
      for (int r=0; r<R_SIZE; ++r)
        for (int c=0; c<R_SIZE; ++c)
          stickers[f][r][c] = (unsigned char)(f % COL_CNT);
}

void display_init(void) {
    gfxInit();
    initPalette();
    gdispClear(GFX_BLACK);
}

void rubikUpdateFromCubeState(char cube_state[54]) {
    for (int f = 0; f < FACE_CNT; f++) {
        for (int r = 0; r < R_SIZE; r++) {
            for (int c = 0; c < R_SIZE; c++) {
                char ch = cube_state[f*R_SIZE*R_SIZE + r*R_SIZE + c];
                switch(ch) {
                    case 'W': stickers[f][r][c] = COL_W; break;
                    case 'Y': stickers[f][r][c] = COL_Y; break;
                    case 'R': stickers[f][r][c] = COL_R; break;
                    case 'O': stickers[f][r][c] = COL_O; break;
                    case 'B': stickers[f][r][c] = COL_B; break;
                    case 'G': stickers[f][r][c] = COL_G; break;
                    default:  stickers[f][r][c] = COL_W; break;
                }
            }
        }
    }
}

typedef struct { int fx, fy; } FacePos;

static FacePos facePos(RubikFace f) {
    switch (f) {
        case FACE_U: return (FacePos){ 1,0 };
        case FACE_L: return (FacePos){ 0,1 };
        case FACE_F: return (FacePos){ 1,1 };
        case FACE_R: return (FacePos){ 2,1 };
        case FACE_B: return (FacePos){ 3,1 };
        case FACE_D: return (FacePos){ 1,2 };
        default:     return (FacePos){ 1,1 };
    }
}

static inline int facePixelSize(void) {
    return FACE_BORDER*2 + (R_SIZE*CELL) + ((R_SIZE-1)*GAP);
}

static void drawSticker(gCoord x, gCoord y, gColor col) {
    gdispFillArea(x, y, CELL, CELL, col);
    gdispDrawBox(x, y, CELL, CELL, GFX_BLACK);
}

static void drawFace(gCoord x0, gCoord y0, RubikFace f) {
    const int FP = facePixelSize();
    gdispFillArea(x0, y0, FP, FP, GFX_BLACK); // bordo esterno
    gdispFillArea(x0+FACE_BORDER, y0+FACE_BORDER, FP-2*FACE_BORDER, FP-2*FACE_BORDER, GFX_GRAY);
    gCoord sx = x0 + FACE_BORDER, sy = y0 + FACE_BORDER;
    for (int r=0; r<R_SIZE; ++r)
        for (int c=0; c<R_SIZE; ++c)
            drawSticker(sx + c*(CELL+GAP), sy + r*(CELL+GAP), kPalette[stickers[f][r][c]]);
}

void rubikDrawNetFromCube(char cube_state[54], gCoord ox, gCoord oy) {
    rubikUpdateFromCubeState(cube_state);
    const int FP = facePixelSize(); (void)FP;
    for (int f=0; f<FACE_CNT; ++f) {
        FacePos p = facePos((RubikFace)f);
        gCoord x = ox + p.fx*(facePixelSize() + NET_GAP);
        gCoord y = oy + p.fy*(facePixelSize() + NET_GAP);
        drawFace(x, y, (RubikFace)f);
    }
}
