#ifndef MOVES_H
#define MOVES_H

// Mosse Upper
void move_U(char cube_state[54]);
void move_Ui(char cube_state[54]);

// Mosse Down
void move_D(char cube_state[54]);
void move_Di(char cube_state[54]);

// Mosse Left
void move_L(char cube_state[54]);
void move_Li(char cube_state[54]);

// Mosse Right
void move_R(char cube_state[54]);
void move_Ri(char cube_state[54]);

// Mosse Front
void move_F(char cube_state[54]);
void move_Fi(char cube_state[54]);

// Mosse Back
void move_B(char cube_state[54]);
void move_Bi(char cube_state[54]);

#endif
