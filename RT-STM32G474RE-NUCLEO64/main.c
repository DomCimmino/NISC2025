#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "rubik/cube.h"
#include "display/display.h"
#include "rubik/solver.h"
#include "music/music.h"

#define CUBE_DATA_SIZE 54
#define BAUD_RATE 115200

#define JOY_CENTER_LINE PAL_LINE(GPIOC, 7)

void joystickInit(void);
void serialInit(void);

char cube_state[CUBE_DATA_SIZE];

static SerialConfig serial_cfg = {
    .speed = BAUD_RATE,
    .cr1 = 0,
    .cr2 = 0,
    .cr3 = 0
};

static THD_WORKING_AREA(waSerialThread, 2048);
static THD_FUNCTION(SerialThread, arg) {
    (void)arg;
    chRegSetThreadName("Serial");

    int idx = 0;

    while (true) {
        msg_t c = sdGetTimeout(&SD2, TIME_MS2I(5000));
        if (c == MSG_TIMEOUT) {
            idx = 0;
            continue;
        }

        cube_state[idx++] = (char)c;

        if (idx == CUBE_DATA_SIZE) {
            rubikDrawNetFromCube(cube_state, 10, 25);
            idx = 0;
        }
    }
}

int main(void) {
    halInit();
    chSysInit();

    displayInit();
    joystickInit();
    serialInit();
    musicInit();

    chThdCreateStatic(waSerialThread, sizeof(waSerialThread),
                      NORMALPRIO, SerialThread, NULL);

    while (true) {
      if(palReadLine(JOY_CENTER_LINE) == PAL_LOW){
        Cube cube;
        initCube(&cube, cube_state);
        solveCube(&cube);
        playMusic();
      }
        chThdSleepMilliseconds(100);
    }
}

void joystickInit(void) {
  palSetLineMode(JOY_CENTER_LINE, PAL_MODE_INPUT_PULLUP);
}

void serialInit(void){
  palSetLineMode(PAL_LINE(GPIOA, 2), PAL_MODE_ALTERNATE(7));
  palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_ALTERNATE(7));

  sdStart(&SD2, &serial_cfg);
}
