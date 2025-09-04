#ifndef CUBE_H
#define CUBE_H

#define NUM_CUBE_FACE 6
#define NUM_CUBE_STICKER_PER_FACE 9
#define TOTAL_STICKER 54

#define U 0
#define D 1
#define R 2
#define F 3
#define L 4
#define B 5


typedef struct {
    int cubies[6][9];
} Cube;

void moves(Cube* cube, const char* moves);
void r(Cube* cube, int n);
void l(Cube* cube, int n);
void u(Cube* cube, int n);
void d(Cube* cube, int n);
void f(Cube* cube, int n);
void b(Cube* cube, int n);
void output(Cube* cube, char* cube_state[TOTAL_STICKER]);

#endif
