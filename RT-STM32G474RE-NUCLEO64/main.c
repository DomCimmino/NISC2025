#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "display.h"
#include "gfx.h"

#define CUBE_DATA_SIZE 54
#define BAUD_RATE 115200

uint8_t cube_state[CUBE_DATA_SIZE];
bool new_data_received = false;
bool display_initialized = false;

void process_cube_data(void);
void send_acknowledgment(void);

static SerialConfig serial_cfg = {
    .speed = BAUD_RATE,
    .cr1 = 0,
    .cr2 = 0,
    .cr3 = 0
};

static THD_WORKING_AREA(waSerialThread, 512);
static THD_FUNCTION(SerialThread, arg) {
    (void)arg;
    uint8_t data_index = 0;
    bool receiving_data = false;
    uint8_t start_marker_index = 0;
    uint8_t end_marker_index = 0;

    // Markers matching Python code
    static const uint8_t start_marker[] = "CUBE_START|";
    static const uint8_t end_marker[] = "|CUBE_END";

    chRegSetThreadName("Serial");

    while (true) {
        msg_t char_received = sdGetTimeout(&SD2, TIME_MS2I(50));

        if (char_received == MSG_TIMEOUT) {
            continue;
        }

        uint8_t received_char = (uint8_t)char_received;

        if (!receiving_data) {
            // Looking for start marker "CUBE_START|"
            if (received_char == start_marker[start_marker_index]) {
                start_marker_index++;
                if (start_marker_index == sizeof(start_marker) - 1) { // -1 for null terminator
                    // Start marker complete, begin receiving data
                    receiving_data = true;
                    start_marker_index = 0;
                    data_index = 0;
                    chprintf((BaseSequentialStream *)&SD2, "Start marker received, waiting for data...\r\n");
                }
            } else {
                // Reset start marker detection on mismatch
                start_marker_index = 0;
            }
        } else {
            // We are receiving data, check for end marker
            if (received_char == end_marker[end_marker_index]) {
                end_marker_index++;
                if (end_marker_index == sizeof(end_marker) - 1) { // -1 for null terminator
                    // End marker complete, data transmission finished
                    receiving_data = false;
                    end_marker_index = 0;

                    if (data_index == CUBE_DATA_SIZE) {
                        new_data_received = true;
                        chprintf((BaseSequentialStream *)&SD2, "End marker received, data complete (%d bytes)\r\n", data_index);
                        send_acknowledgment();
                    } else {
                        chprintf((BaseSequentialStream *)&SD2, "End marker received but wrong data size: %d/%d bytes\r\n",
                                data_index, CUBE_DATA_SIZE);
                    }
                }
            } else {
                if (end_marker_index > 0) {
                    // We were partially matching end marker but got wrong char
                    // Add the partially matched end marker bytes to data
                    for (uint8_t i = 0; i < end_marker_index; i++) {
                        if (data_index < CUBE_DATA_SIZE) {
                            cube_state[data_index++] = end_marker[i];
                        } else {
                            chprintf((BaseSequentialStream *)&SD2, "Buffer overflow during end marker recovery!\r\n");
                            receiving_data = false;
                            break;
                        }
                    }
                    end_marker_index = 0;

                    // Now add the current character
                    if (receiving_data && data_index < CUBE_DATA_SIZE) {
                        cube_state[data_index++] = received_char;
                    }
                } else {
                    // Normal data reception
                    if (data_index < CUBE_DATA_SIZE) {
                        cube_state[data_index++] = received_char;
                    } else {
                        // Buffer overflow
                        chprintf((BaseSequentialStream *)&SD2, "Buffer overflow at index %d!\r\n", data_index);
                        receiving_data = false;
                    }
                }
            }
        }
    }
}

static THD_WORKING_AREA(waDisplayThread, 512);
static THD_FUNCTION(DisplayThread, arg) {
    (void)arg;
    chRegSetThreadName("Display");

    while (true) {
        handle_navigation();

        if (display_initialized) {
            draw_current_face(cube_state);
        }

        chThdSleepMilliseconds(50);
    }
}

void send_acknowledgment(void) {
    chprintf((BaseSequentialStream *)&SD2, "ACK\r\n");
}

void process_cube_data(void) {
  if (new_data_received) {
    chprintf((BaseSequentialStream *)&SD2, "Drawing cube on display...\r\n");
    if (!display_initialized) {
      draw_current_face(cube_state);
      display_initialized = true;
    }

    chprintf((BaseSequentialStream *)&SD2, "Cube displayed successfully\r\n");

    new_data_received = false;
  }
}


int main(void) {

  halInit();
  chSysInit();

  palSetLineMode(PAL_LINE(GPIOA, 2), PAL_MODE_ALTERNATE(7));
  palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_ALTERNATE(7));

  display_init();

  sdStart(&SD2, &serial_cfg);

  chprintf((BaseSequentialStream *)&SD2, "Rubik's Cube Receiver Ready\r\n");
  chprintf((BaseSequentialStream *)&SD2, "Waiting for data...\r\n");

  chThdCreateStatic(waSerialThread, sizeof(waSerialThread), NORMALPRIO, SerialThread, NULL);
  chThdCreateStatic(waDisplayThread, sizeof(waDisplayThread), NORMALPRIO, DisplayThread, NULL);

  while (true) {
      process_cube_data();
      chThdSleepMilliseconds(100);
      gfxSleepMilliseconds(10000);
  }
}

