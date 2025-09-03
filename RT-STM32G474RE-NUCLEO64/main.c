#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "display.h"

#define CUBE_DATA_SIZE 54
#define BAUD_RATE 115200

// Cube state storage - 54 raw chars
uint8_t cube_state[CUBE_DATA_SIZE];
bool new_data_received = false;
bool display_initialized = false;

// Serial config
static SerialConfig serial_cfg = {
    .speed = BAUD_RATE,
    .cr1 = 0,
    .cr2 = 0,
    .cr3 = 0
};

// Thread for receiving raw cube data
static THD_WORKING_AREA(waSerialThread, 512);
static THD_FUNCTION(SerialThread, arg) {
    (void)arg;
    chRegSetThreadName("Serial");

    int idx = 0;

    while (true) {
        msg_t c = sdGetTimeout(&SD2, TIME_MS2I(5000));

        if (c == MSG_TIMEOUT) {
            continue;
        }

        cube_state[idx++] = (char)c;

        if (idx == CUBE_DATA_SIZE) {
            new_data_received = true;
            idx = 0;  // ricomincia per il prossimo pacchetto
        }
    }
}

// Print and display cube data
void process_cube_data(void) {
    if (new_data_received) {
        chprintf((BaseSequentialStream *)&SD2, "Cube state received:\r\n");

        for (int i = 0; i < CUBE_DATA_SIZE; i++) {
            chprintf((BaseSequentialStream *)&SD2, "%c ", cube_state[i]);
            if ((i + 1) % 9 == 0) {
                chprintf((BaseSequentialStream *)&SD2, "\r\n");
            }
        }

        rubikDrawNetFromCube(cube_state, 10, 10);

        new_data_received = false;
    }
}

int main(void) {
    halInit();
    chSysInit();

    display_init();

    palSetLineMode(PAL_LINE(GPIOA, 2), PAL_MODE_ALTERNATE(7));
    palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_ALTERNATE(7));

    sdStart(&SD2, &serial_cfg);

    chprintf((BaseSequentialStream *)&SD2, "Rubik's Cube Raw Receiver Ready\r\n");

    chThdCreateStatic(waSerialThread, sizeof(waSerialThread),
                      NORMALPRIO, SerialThread, NULL);

    while (true) {
        process_cube_data();
        handle_navigation(cube_state);
        chThdSleepMilliseconds(100);
    }
}
