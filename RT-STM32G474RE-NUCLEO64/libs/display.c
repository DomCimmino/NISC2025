#include "display.h"
#include "ch.h"
#include "hal.h"
#include "gfx.h"

#define COLOR_WHITE     GFX_COLOR_WHITE
#define COLOR_YELLOW    GFX_COLOR_YELLOW
#define COLOR_RED       GFX_COLOR_RED
#define COLOR_ORANGE    GFX_COLOR(255,165,0)
#define COLOR_BLUE      GFX_COLOR_BLUE
#define COLOR_GREEN     GFX_COLOR_GREEN

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
static const char *face_names[] = {"UP", "DOWN", "FRONT", "BACK", "LEFT", "RIGHT"};
static const char *face_letters[] = {"U", "D", "F", "B", "L", "R"};

static uint16_t get_color_from_value(uint8_t color_value) {
    switch(color_value) {
        case 0: return COLOR_WHITE;
        case 1: return COLOR_YELLOW;
        case 2: return COLOR_RED;
        case 3: return COLOR_ORANGE;
        case 4: return COLOR_BLUE;
        case 5: return COLOR_GREEN;
        default: return GFX_COLOR_BLACK;
    }
}

static void get_face_data(uint8_t cube_state[54], CubeFace face, uint8_t face_data[9]) {
    uint8_t start_index = face * 9;
    for (int i = 0; i < 9; i++) {
        face_data[i] = cube_state[start_index + i];
    }
}

void draw_current_face(uint8_t cube_state[54]) {
    gfxFillScreen(GFX_COLOR_BLACK);

    uint8_t face_data[9];
    get_face_data(cube_state, current_face, face_data);

    char title[20];
    snprintf(title, sizeof(title), "FACE: %s (%s)", face_names[current_face], face_letters[current_face]);
    gfxDrawString(10, 10, title, GFX_COLOR_WHITE, 2);

    gfxDrawString(10, 200, "Use joystick to navigate", GFX_COLOR_GRAY, 1);
    gfxDrawString(10, 215, "Center: Next face", GFX_COLOR_GRAY, 1);

    uint16_t face_x = (gfxGetDisplayWidth() - (3 * STICKER_SIZE + 2 * STICKER_SPACING)) / 2;
    uint16_t face_y = 50;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            uint8_t color_value = face_data[row * 3 + col];
            uint16_t color = get_color_from_value(color_value);

            uint16_t sticker_x = face_x + col * (STICKER_SIZE + STICKER_SPACING);
            uint16_t sticker_y = face_y + row * (STICKER_SIZE + STICKER_SPACING);

            gfxFillRoundRect(sticker_x, sticker_y, STICKER_SIZE, STICKER_SIZE, 5, color);
            gfxDrawRoundRect(sticker_x, sticker_y, STICKER_SIZE, STICKER_SIZE, 5, GFX_COLOR_BLACK);

            char value_str[4];
            snprintf(value_str, sizeof(value_str), "%d", color_value);
            gfxDrawString(sticker_x + 15, sticker_y + 12, value_str, GFX_COLOR_BLACK, 1);
        }
    }

    show_face_number(current_face);
}

void show_face_number(uint8_t face_num) {
    uint16_t y_pos = 180;
    uint16_t x_start = (gfxGetDisplayWidth() - (NUM_FACES * 20 + (NUM_FACES-1) * 5)) / 2;

    for (int i = 0; i < NUM_FACES; i++) {
        uint16_t x_pos = x_start + i * 25;

        if (i == face_num) {
            gfxFillCircle(x_pos, y_pos, 8, GFX_COLOR_WHITE);
            gfxDrawCircle(x_pos, y_pos, 8, GFX_COLOR_BLACK);
        } else {
            gfxFillCircle(x_pos, y_pos, 6, GFX_COLOR_GRAY);
            gfxDrawCircle(x_pos, y_pos, 6, GFX_COLOR_BLACK);
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

void handle_navigation(void) {
    static uint32_t last_press_time = 0;
    uint32_t current_time = chVTGetSystemTime();

    if (current_time - last_press_time < TIME_MS2I(200)) {
        return;
    }

    bool navigation_changed = false;

    if (is_button_pressed(JOY_CENTER_PIN)) {
        current_face = (current_face + 1) % NUM_FACES;
        navigation_changed = true;
        last_press_time = current_time;
    }
}

void display_init(void) {
    gfxInit();

    joystick_init();

    gfxFillScreen(GFX_COLOR_BLACK);

    gfxDrawString(40, 60, "RUBIK'S CUBE", GFX_COLOR_WHITE, 2);
    gfxDrawString(50, 90, "VIEWER", GFX_COLOR_WHITE, 2);
    gfxDrawString(30, 130, "Press center button", GFX_COLOR_WHITE, 1);
    gfxDrawString(40, 150, "to start", GFX_COLOR_WHITE, 1);

    while (!is_button_pressed(JOY_CENTER_PIN)) {
        chThdSleepMilliseconds(100);
    }

    gfxFillScreen(GFX_COLOR_BLACK);
}
