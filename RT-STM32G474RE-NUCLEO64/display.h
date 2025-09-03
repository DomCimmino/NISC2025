#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

void display_init(void);
void draw_current_face(uint8_t cube_state[54]);
void handle_navigation(void);
void show_face_number(uint8_t face_num);

typedef enum {
    FACE_U = 0,
    FACE_D = 1,
    FACE_F = 2,
    FACE_B = 3,
    FACE_L = 4,
    FACE_R = 5,
    NUM_FACES = 6
} CubeFace;

#endif
