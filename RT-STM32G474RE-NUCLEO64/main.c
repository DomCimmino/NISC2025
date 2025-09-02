#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#define CUBE_DATA_SIZE 54
#define BAUD_RATE 115200

uint8_t cube_state[CUBE_DATA_SIZE];
bool new_data_received = false;

void process_cube_data(void);
void send_acknowledgment(void);

static SerialConfig serial_cfg = {
    .speed = BAUD_RATE,
    .cr1 = 0,
    .cr2 = 0,
    .cr3 = 0
};

static THD_WORKING_AREA(waSerialThread, 256);
static THD_FUNCTION(SerialThread, arg) {
    (void)arg;
    uint8_t data_index = 0;
    bool receiving_data = false;
    bool start_marker_received = false;

    chRegSetThreadName("Serial");

    while (true) {
        msg_t char_received = sdGetTimeout(&SD2, TIME_MS2I(100));

        if (char_received == MSG_TIMEOUT) {
            continue;
        }

        uint8_t received_char = (uint8_t)char_received;

        if (!start_marker_received) {
            static const uint8_t start_marker[] = "START";
            static uint8_t start_index = 0;

            if (received_char == start_marker[start_index]) {
                start_index++;
                if (start_index == sizeof(start_marker) - 1) {
                    start_marker_received = true;
                    start_index = 0;
                    receiving_data = true;
                    data_index = 0;
                    chprintf((BaseSequentialStream *)&SD2, "START received\r\n");
                }
            } else {
                start_index = 0;
            }
        }
        else if (receiving_data) {
            static const uint8_t end_marker[] = "END";
            static uint8_t end_index = 0;

            if (received_char == end_marker[end_index]) {
                end_index++;
                if (end_index == sizeof(end_marker) - 1) {
                    receiving_data = false;
                    start_marker_received = false;
                    end_index = 0;
                    new_data_received = true;
                    chprintf((BaseSequentialStream *)&SD2, "END received\r\n");
                    send_acknowledgment();
                }
            } else {
                if (end_index > 0) {
                    for (uint8_t i = 0; i < end_index; i++) {
                        if (data_index < CUBE_DATA_SIZE) {
                            cube_state[data_index++] = end_marker[i];
                        }
                    }
                    end_index = 0;
                }

                if (data_index < CUBE_DATA_SIZE) {
                    cube_state[data_index++] = received_char;
                } else {
                    receiving_data = false;
                    start_marker_received = false;
                    chprintf((BaseSequentialStream *)&SD2, "Buffer overflow!\r\n");
                }
            }
        }
    }
}

void send_acknowledgment(void) {
    chprintf((BaseSequentialStream *)&SD2, "ACK\r\n");
}

void process_cube_data(void) {
    if (new_data_received) {
        chprintf((BaseSequentialStream *)&SD2, "Received cube data:\r\n");

        for (int i = 0; i < CUBE_DATA_SIZE; i++) {
            chprintf((BaseSequentialStream *)&SD2, "%d ", cube_state[i]);
            if ((i + 1) % 9 == 0) {
                chprintf((BaseSequentialStream *)&SD2, "\r\n");
            }
        }
        chprintf((BaseSequentialStream *)&SD2, "\r\n");

        new_data_received = false;
    }
}


int main(void) {

  halInit();
  chSysInit();

  palSetLineMode(PAL_LINE(GPIOA, 2), PAL_MODE_ALTERNATE(7));
  palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_ALTERNATE(7));

  sdStart(&SD2, &serial_cfg);

  chprintf((BaseSequentialStream *)&SD2, "Rubik's Cube Receiver Ready\r\n");
  chprintf((BaseSequentialStream *)&SD2, "Waiting for data...\r\n");

  chThdCreateStatic(waSerialThread, sizeof(waSerialThread), NORMALPRIO, SerialThread, NULL);

  while (true) {
      process_cube_data();
      chThdSleepMilliseconds(100);
  }
}

