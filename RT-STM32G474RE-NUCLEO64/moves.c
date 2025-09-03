#include "moves.h"
#include <string.h>

// Indici delle facce
// U=0, R=1, F=2, D=3, L=4, B=5
#define U 0
#define R 1
#define F 2
#define D 3
#define L 4
#define B 5

// Helper: ruota una faccia 3x3 in senso orario
static void rotate_face_cw(char face[9]) {
    char tmp[9];
    memcpy(tmp, face, 9);
    face[0] = tmp[6];
    face[1] = tmp[3];
    face[2] = tmp[0];
    face[3] = tmp[7];
    face[4] = tmp[4];
    face[5] = tmp[1];
    face[6] = tmp[8];
    face[7] = tmp[5];
    face[8] = tmp[2];
}

static void rotate_face_ccw(char face[9]) {
    rotate_face_cw(face);
    rotate_face_cw(face);
    rotate_face_cw(face);
}

// Accesso semplificato a facce
static char* face_ptr(char cube[54], int f) {
    return &cube[f*9];
}

// ----- MOSSE -----

// U
void move_U(char cube[54]) {
    char* fU = face_ptr(cube, U);
    char tmp[3];

    // ruota faccia U
    rotate_face_cw(fU);

    // salva strato superiore laterali
    tmp[0] = cube[F*9 + 0]; tmp[1] = cube[F*9 + 1]; tmp[2] = cube[F*9 + 2];

    // ruota strati superiori: F->R->B->L->F
    cube[F*9+0] = cube[R*9+0]; cube[F*9+1] = cube[R*9+1]; cube[F*9+2] = cube[R*9+2];
    cube[R*9+0] = cube[B*9+0]; cube[R*9+1] = cube[B*9+1]; cube[R*9+2] = cube[B*9+2];
    cube[B*9+0] = cube[L*9+0]; cube[B*9+1] = cube[L*9+1]; cube[B*9+2] = cube[L*9+2];
    cube[L*9+0] = tmp[0]; cube[L*9+1] = tmp[1]; cube[L*9+2] = tmp[2];
}

void move_Ui(char cube[54]) { move_U(cube); move_U(cube); move_U(cube); }

// D
void move_D(char cube[54]) {
    char* fD = face_ptr(cube, D);
    char tmp[3];

    rotate_face_cw(fD);

    tmp[0] = cube[F*9+6]; tmp[1] = cube[F*9+7]; tmp[2] = cube[F*9+8];

    cube[F*9+6] = cube[L*9+6]; cube[F*9+7] = cube[L*9+7]; cube[F*9+8] = cube[L*9+8];
    cube[L*9+6] = cube[B*9+6]; cube[L*9+7] = cube[B*9+7]; cube[L*9+8] = cube[B*9+8];
    cube[B*9+6] = cube[R*9+6]; cube[B*9+7] = cube[R*9+7]; cube[B*9+8] = cube[R*9+8];
    cube[R*9+6] = tmp[0]; cube[R*9+7] = tmp[1]; cube[R*9+8] = tmp[2];
}

void move_Di(char cube[54]) { move_D(cube); move_D(cube); move_D(cube); }

// L
void move_L(char cube[54]) {
    char* fL = face_ptr(cube, L);
    char tmp[3];

    rotate_face_cw(fL);

    tmp[0] = cube[U*9+0]; tmp[1] = cube[U*9+3]; tmp[2] = cube[U*9+6];

    cube[U*9+0] = cube[B*9+8]; cube[U*9+3] = cube[B*9+5]; cube[U*9+6] = cube[B*9+2];
    cube[B*9+8] = cube[D*9+8]; cube[B*9+5] = cube[D*9+5]; cube[B*9+2] = cube[D*9+2];
    cube[D*9+2] = cube[F*9+0]; cube[D*9+5] = cube[F*9+3]; cube[D*9+8] = cube[F*9+6];
    cube[F*9+0] = tmp[0]; cube[F*9+3] = tmp[1]; cube[F*9+6] = tmp[2];
}

void move_Li(char cube[54]) { move_L(cube); move_L(cube); move_L(cube); }

// R
void move_R(char cube[54]) {
    char* fR = face_ptr(cube, R);
    char tmp[3];

    rotate_face_cw(fR);

    tmp[0] = cube[U*9+2]; tmp[1] = cube[U*9+5]; tmp[2] = cube[U*9+8];

    cube[U*9+2] = cube[F*9+2]; cube[U*9+5] = cube[F*9+5]; cube[U*9+8] = cube[F*9+8];
    cube[F*9+2] = cube[D*9+2]; cube[F*9+5] = cube[D*9+5]; cube[F*9+8] = cube[D*9+8];
    cube[D*9+2] = cube[B*9+6]; cube[D*9+5] = cube[B*9+3]; cube[D*9+8] = cube[B*9+0];
    cube[B*9+0] = tmp[2]; cube[B*9+3] = tmp[1]; cube[B*9+6] = tmp[0];
}

void move_Ri(char cube[54]) { move_R(cube); move_R(cube); move_R(cube); }

// F
void move_F(char cube[54]) {
    char* fF = face_ptr(cube, F);
    char tmp[3];

    rotate_face_cw(fF);

    tmp[0] = cube[U*9+6]; tmp[1] = cube[U*9+7]; tmp[2] = cube[U*9+8];

    cube[U*9+6] = cube[L*9+8]; cube[U*9+7] = cube[L*9+5]; cube[U*9+8] = cube[L*9+2];
    cube[L*9+2] = cube[D*9+0]; cube[L*9+5] = cube[D*9+1]; cube[L*9+8] = cube[D*9+2];
    cube[D*9+0] = cube[R*9+6]; cube[D*9+1] = cube[R*9+3]; cube[D*9+2] = cube[R*9+0];
    cube[R*9+0] = tmp[0]; cube[R*9+3] = tmp[1]; cube[R*9+6] = tmp[2];
}

void move_Fi(char cube[54]) { move_F(cube); move_F(cube); move_F(cube); }

// B
void move_B(char cube[54]) {
    char* fB = face_ptr(cube, B);
    char tmp[3];

    rotate_face_cw(fB);

    tmp[0] = cube[U*9+0]; tmp[1] = cube[U*9+1]; tmp[2] = cube[U*9+2];

    cube[U*9+0] = cube[R*9+2]; cube[U*9+1] = cube[R*9+5]; cube[U*9+2] = cube[R*9+8];
    cube[R*9+2] = cube[D*9+8]; cube[R*9+5] = cube[D*9+7]; cube[R*9+8] = cube[D*9+6];
    cube[D*9+6] = cube[L*9+0]; cube[D*9+7] = cube[L*9+3]; cube[D*9+8] = cube[L*9+6];
    cube[L*9+0] = tmp[2]; cube[L*9+3] = tmp[1]; cube[L*9+6] = tmp[0];
}

void move_Bi(char cube[54]) { move_B(cube); move_B(cube); move_B(cube); }
