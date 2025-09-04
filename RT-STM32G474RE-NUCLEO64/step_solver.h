#ifndef STEP_SOLVER_H
#define STEP_SOLVER_H

#include <stdint.h>
#include <stdbool.h>

// Definizione colori
#define COLOR_W 'W'
#define COLOR_Y 'Y'
#define COLOR_R 'R'
#define COLOR_O 'O'
#define COLOR_G 'G'
#define COLOR_B 'B'

// Definizione facce
#define U_FACE 0
#define R_FACE 9
#define F_FACE 18
#define D_FACE 27
#define L_FACE 36
#define B_FACE 45

// Struttura per lo stato della risoluzione
typedef enum {
    SOLVER_IDLE,
    SOLVER_CROSS_WHITE,
    SOLVER_CORNERS_WHITE,
    SOLVER_MIDDLE_LAYER,
    SOLVER_YELLOW_CROSS,
    SOLVER_YELLOW_CORNERS,
    SOLVER_ORIENT_CORNERS,
    SOLVER_PERMUTE_CORNERS,
    SOLVER_COMPLETED
} SolverState;

// Struttura del solver
typedef struct {
    char cube_state[54];
    SolverState current_state;
    uint8_t step_counter;
    bool is_solving;
} StepSolver;

// Inizializzazione solver
void solver_init(StepSolver* solver, char initial_state[54]);

// Esegue un singolo passo della risoluzione
// Restituisce true se la risoluzione è completata
bool solver_execute_step(StepSolver* solver);

// Ottiene la descrizione della mossa corrente
const char* solver_get_current_move(void);

// Ottiene lo stato corrente della risoluzione
const char* solver_get_state_name(SolverState state);

// Funzioni di rotazione (esposte per testing)
void rotate_face_clockwise(char* cube, int face_start);
void rotate_face_counterclockwise(char* cube, int face_start);

// Mosse base
void move_U(char* cube);
void move_U_prime(char* cube);
void move_R(char* cube);
void move_R_prime(char* cube);
void move_F(char* cube);
void move_F_prime(char* cube);
void move_L(char* cube);
void move_L_prime(char* cube);
void move_B(char* cube);
void move_B_prime(char* cube);
void move_D(char* cube);
void move_D_prime(char* cube);

#endif // STEP_SOLVER_H
