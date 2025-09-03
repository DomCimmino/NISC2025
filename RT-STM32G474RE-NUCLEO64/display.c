#include "display.h"
#include "ch.h"
#include "hal.h"
#include "gfx.h"


#define COLOR_WHITE     GFX_WHITE
#define COLOR_YELLOW    GFX_YELLOW
#define COLOR_RED       GFX_RED
#define COLOR_ORANGE    GFX_ORANGE
#define COLOR_BLUE      GFX_BLUE
#define COLOR_BLACK     GFX_BLACK
#define COLOR_GREEN     HTML2COLOR(0x009000)

#define STICKER_SIZE 40
#define STICKER_SPACING 4
#define FACE_MARGIN 20

#define JOY_PORT_1      GPIOB
#define JOY_PORT_2      GPIOC
#define JOY_UP_PIN          0
#define JOY_DOWN_PIN        4
#define JOY_LEFT_PIN        6
#define JOY_RIGHT_PIN       0
#define JOY_CENTRE_PIN      7

static CubeFace current_face = FACE_U;

static gColor get_color_from_value(uint8_t color_value) {
    switch(color_value) {
        case 0: return COLOR_WHITE;
        case 1: return COLOR_YELLOW;
        case 2: return COLOR_RED;
        case 3: return COLOR_ORANGE;
        case 4: return COLOR_BLUE;
        case 5: return COLOR_GREEN;
        default: return COLOR_BLACK;
    }
}

static void get_face_data(uint8_t cube_state[54], CubeFace face, uint8_t face_data[9]) {
    uint8_t start_index = face * 9;
    for (int i = 0; i < 9; i++) {
        face_data[i] = cube_state[start_index + i];
    }
}

void draw_current_face(uint8_t cube_state[54]) {
    gdispClear(COLOR_BLACK);

    uint8_t face_data[9];
    get_face_data(cube_state, current_face, face_data);

    uint16_t face_x = (gdispGetWidth() - (3 * STICKER_SIZE + 2 * STICKER_SPACING)) / 2;
    uint16_t face_y = 50;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            uint8_t color_value = face_data[row * 3 + col];
            gColor color = get_color_from_value(color_value);

            uint16_t sticker_x = face_x + col * (STICKER_SIZE + STICKER_SPACING);
            uint16_t sticker_y = face_y + row * (STICKER_SIZE + STICKER_SPACING);

            gdispFillArea(sticker_x, sticker_y, STICKER_SIZE, STICKER_SIZE, color);
            gdispDrawBox(sticker_x, sticker_y, STICKER_SIZE, STICKER_SIZE, COLOR_BLACK);
        }
    }

    show_face_number(current_face);
}

void show_face_number(uint8_t face_num) {
    uint16_t y_pos = 180;
    uint16_t x_start = (gdispGetWidth() - (NUM_FACES * 20 + (NUM_FACES-1) * 5)) / 2;

    for (int i = 0; i < NUM_FACES; i++) {
        uint16_t x_pos = x_start + i * 25;

        if (i == face_num) {
            gdispFillCircle(x_pos, y_pos, 8, COLOR_WHITE);
            gdispDrawCircle(x_pos, y_pos, 8, COLOR_BLACK);
        } else {
            gdispFillCircle(x_pos, y_pos, 6, GFX_GRAY);
            gdispDrawCircle(x_pos, y_pos, 6, COLOR_BLACK);
        }
    }
}

static void joystick_init(void) {
      palSetPadMode(JOY_PORT_2, JOY_UP_PIN, PAL_MODE_INPUT_PULLUP);            //UP
      palSetPadMode(JOY_PORT_1, JOY_DOWN_PIN, PAL_MODE_INPUT_PULLUP);          //DOWN
      palSetPadMode(JOY_PORT_1, JOY_LEFT_PIN, PAL_MODE_INPUT_PULLUP);          //LEFT
      palSetPadMode(JOY_PORT_1, JOY_RIGHT_PIN, PAL_MODE_INPUT_PULLUP);         //RIGHT
      palSetPadMode(JOY_PORT_2, JOY_CENTRE_PIN, PAL_MODE_INPUT_PULLUP);      //CENTRE
}

static bool is_button_pressed(ioportid_t port, uint16_t pad) {
    return palReadPad(port, pad) == PAL_LOW;
}

static bool is_center_pressed(void) {
    return is_button_pressed(JOY_PORT_2, JOY_CENTRE_PIN);
}


void handle_navigation(void) {
    static uint32_t last_press_time = 0;
    uint32_t current_time = chVTGetSystemTime();

    if (current_time - last_press_time < TIME_MS2I(200)) {
        return;
    }

    if (is_center_pressed()) {
        current_face = (current_face + 1) % NUM_FACES;
        last_press_time = current_time;
    }
}

void display_init(void) {
    gfxInit();

    joystick_init();

    gdispClear(COLOR_BLACK);

    while (!is_center_pressed()) {
        chThdSleepMilliseconds(100);
    }
}
