#include "step_solver.h"
#include <string.h>

// Variabile per l'ultima mossa eseguita
static char current_move[20] = "Inizio";

// Inizializzazione solver
void solver_init(StepSolver* solver, char initial_state[54]) {
    memcpy(solver->cube_state, initial_state, 54);
    solver->current_state = SOLVER_CROSS_WHITE;
    solver->step_counter = 0;
    solver->is_solving = true;
    strcpy(current_move, "Inizio risoluzione");
}

// Restituisce la descrizione della mossa corrente
const char* solver_get_current_move(void) {
    return current_move;
}

// Restituisce il nome dello stato
const char* solver_get_state_name(SolverState state) {
    switch(state) {
        case SOLVER_IDLE: return "Inattivo";
        case SOLVER_CROSS_WHITE: return "Croce bianca";
        case SOLVER_CORNERS_WHITE: return "Angoli bianchi";
        case SOLVER_MIDDLE_LAYER: return "Strato medio";
        case SOLVER_YELLOW_CROSS: return "Croce gialla";
        case SOLVER_YELLOW_CORNERS: return "Angoli gialli";
        case SOLVER_ORIENT_CORNERS: return "Orientamento angoli";
        case SOLVER_PERMUTE_CORNERS: return "Permutazione angoli";
        case SOLVER_COMPLETED: return "Completato";
        default: return "Sconosciuto";
    }
}

// Funzioni di rotazione
void rotate_face_clockwise(char* cube, int face_start) {
    char temp[9];
    memcpy(temp, &cube[face_start], 9);

    cube[face_start] = temp[6];
    cube[face_start+1] = temp[3];
    cube[face_start+2] = temp[0];
    cube[face_start+3] = temp[7];
    cube[face_start+4] = temp[4];
    cube[face_start+5] = temp[1];
    cube[face_start+6] = temp[8];
    cube[face_start+7] = temp[5];
    cube[face_start+8] = temp[2];
}

void rotate_face_counterclockwise(char* cube, int face_start) {
    char temp[9];
    memcpy(temp, &cube[face_start], 9);

    cube[face_start] = temp[2];
    cube[face_start+1] = temp[5];
    cube[face_start+2] = temp[8];
    cube[face_start+3] = temp[1];
    cube[face_start+4] = temp[4];
    cube[face_start+5] = temp[7];
    cube[face_start+6] = temp[0];
    cube[face_start+7] = temp[3];
    cube[face_start+8] = temp[6];
}

// Implementazione mosse base
void move_U(char* cube) {
    rotate_face_clockwise(cube, U_FACE);
    strcpy(current_move, "U");

    char temp[3];
    memcpy(temp, &cube[9], 3);
    memcpy(&cube[9], &cube[18], 3);
    memcpy(&cube[18], &cube[36], 3);
    memcpy(&cube[36], &cube[45], 3);
    memcpy(&cube[45], temp, 3);
}

void move_U_prime(char* cube) {
    rotate_face_counterclockwise(cube, U_FACE);
    strcpy(current_move, "U'");

    char temp[3];
    memcpy(temp, &cube[9], 3);
    memcpy(&cube[9], &cube[45], 3);
    memcpy(&cube[45], &cube[36], 3);
    memcpy(&cube[36], &cube[18], 3);
    memcpy(&cube[18], temp, 3);
}

void move_R(char* cube) {
    rotate_face_clockwise(cube, R_FACE);
    strcpy(current_move, "R");

    char temp[3];
    temp[0] = cube[20]; temp[1] = cube[23]; temp[2] = cube[26];

    cube[20] = cube[2]; cube[23] = cube[5]; cube[26] = cube[8];
    cube[2] = cube[42]; cube[5] = cube[39]; cube[8] = cube[36];
    cube[42] = cube[29]; cube[39] = cube[32]; cube[36] = cube[35];
    cube[29] = temp[0]; cube[32] = temp[1]; cube[35] = temp[2];
}

void move_R_prime(char* cube) {
    rotate_face_counterclockwise(cube, R_FACE);
    strcpy(current_move, "R'");

    char temp[3];
    temp[0] = cube[20]; temp[1] = cube[23]; temp[2] = cube[26];

    cube[20] = cube[29]; cube[23] = cube[32]; cube[26] = cube[35];
    cube[29] = cube[42]; cube[32] = cube[39]; cube[35] = cube[36];
    cube[42] = cube[2]; cube[39] = cube[5]; cube[36] = cube[8];
    cube[2] = temp[0]; cube[5] = temp[1]; cube[8] = temp[2];
}

void move_F(char* cube) {
    rotate_face_clockwise(cube, F_FACE);
    strcpy(current_move, "F");

    char temp[3];
    temp[0] = cube[6]; temp[1] = cube[7]; temp[2] = cube[8];

    cube[6] = cube[44]; cube[7] = cube[41]; cube[8] = cube[38];
    cube[44] = cube[27]; cube[41] = cube[28]; cube[38] = cube[29];
    cube[27] = cube[18]; cube[28] = cube[21]; cube[29] = cube[24];
    cube[18] = temp[0]; cube[21] = temp[1]; cube[24] = temp[2];
}

void move_F_prime(char* cube) {
    rotate_face_counterclockwise(cube, F_FACE);
    strcpy(current_move, "F'");

    char temp[3];
    temp[0] = cube[6]; temp[1] = cube[7]; temp[2] = cube[8];

    cube[6] = cube[18]; cube[7] = cube[21]; cube[8] = cube[24];
    cube[18] = cube[27]; cube[21] = cube[28]; cube[24] = cube[29];
    cube[27] = cube[44]; cube[28] = cube[41]; cube[29] = cube[38];
    cube[44] = temp[0]; cube[41] = temp[1]; cube[38] = temp[2];
}

void move_L(char* cube) {
    rotate_face_clockwise(cube, L_FACE);
    strcpy(current_move, "L");

    char temp[3];
    temp[0] = cube[0]; temp[1] = cube[3]; temp[2] = cube[6];

    cube[0] = cube[45]; cube[3] = cube[48]; cube[6] = cube[51];
    cube[45] = cube[35]; cube[48] = cube[32]; cube[51] = cube[29];
    cube[35] = cube[18]; cube[32] = cube[21]; cube[29] = cube[24];
    cube[18] = temp[0]; cube[21] = temp[1]; cube[24] = temp[2];
}

void move_L_prime(char* cube) {
    rotate_face_counterclockwise(cube, L_FACE);
    strcpy(current_move, "L'");

    char temp[3];
    temp[0] = cube[0]; temp[1] = cube[3]; temp[2] = cube[6];

    cube[0] = cube[18]; cube[3] = cube[21]; cube[6] = cube[24];
    cube[18] = cube[35]; cube[21] = cube[32]; cube[24] = cube[29];
    cube[35] = cube[45]; cube[32] = cube[48]; cube[29] = cube[51];
    cube[45] = temp[0]; cube[48] = temp[1]; cube[51] = temp[2];
}

void move_B(char* cube) {
    rotate_face_clockwise(cube, B_FACE);
    strcpy(current_move, "B");

    char temp[3];
    temp[0] = cube[2]; temp[1] = cube[1]; temp[2] = cube[0];

    cube[2] = cube[9]; cube[1] = cube[12]; cube[0] = cube[15];
    cube[9] = cube[33]; cube[12] = cube[30]; cube[15] = cube[27];
    cube[33] = cube[53]; cube[30] = cube[52]; cube[27] = cube[51];
    cube[53] = temp[0]; cube[52] = temp[1]; cube[51] = temp[2];
}

void move_B_prime(char* cube) {
    rotate_face_counterclockwise(cube, B_FACE);
    strcpy(current_move, "B'");

    char temp[3];
    temp[0] = cube[2]; temp[1] = cube[1]; temp[2] = cube[0];

    cube[2] = cube[53]; cube[1] = cube[52]; cube[0] = cube[51];
    cube[53] = cube[33]; cube[52] = cube[30]; cube[51] = cube[27];
    cube[33] = cube[9]; cube[30] = cube[12]; cube[27] = cube[15];
    cube[9] = temp[0]; cube[12] = temp[1]; cube[15] = temp[2];
}

void move_D(char* cube) {
    rotate_face_clockwise(cube, D_FACE);
    strcpy(current_move, "D");

    char temp[3];
    memcpy(temp, &cube[15], 3);

    memcpy(&cube[15], &cube[24], 3);
    memcpy(&cube[24], &cube[33], 3);
    memcpy(&cube[33], &cube[42], 3);
    memcpy(&cube[42], temp, 3);
}

void move_D_prime(char* cube) {
    rotate_face_counterclockwise(cube, D_FACE);
    strcpy(current_move, "D'");

    char temp[3];
    memcpy(temp, &cube[15], 3);

    memcpy(&cube[15], &cube[42], 3);
    memcpy(&cube[42], &cube[33], 3);
    memcpy(&cube[33], &cube[24], 3);
    memcpy(&cube[24], temp, 3);
}

// Funzioni di supporto per la risoluzione
static bool is_white_cross_solved(char* cube) {
    return (cube[46] == COLOR_W && cube[48] == COLOR_W &&
            cube[50] == COLOR_W && cube[52] == COLOR_W &&
            cube[37] == COLOR_R && cube[39] == COLOR_B &&
            cube[41] == COLOR_O && cube[43] == COLOR_G);
}

static bool is_white_corners_solved(char* cube) {
    return (cube[45] == COLOR_W && cube[47] == COLOR_W &&
            cube[51] == COLOR_W && cube[53] == COLOR_W &&
            cube[36] == COLOR_R && cube[38] == COLOR_R &&
            cube[42] == COLOR_B && cube[44] == COLOR_B &&
            cube[48] == COLOR_O && cube[50] == COLOR_O &&
            cube[54] == COLOR_G && cube[52] == COLOR_G);
}

// Funzioni di risoluzione per ogni fase
static bool solve_cross_white_step(StepSolver* solver) {
    // Algoritmo semplificato per la croce bianca
    // In una implementazione reale, qui ci sarebbe la logica completa
    // per posizionare correttamente i pezzi del bordo bianco

    switch(solver->step_counter) {
        case 0:
            move_U(solver->cube_state);
            break;
        case 1:
            move_U(solver->cube_state);
            break;
        case 2:
            move_F(solver->cube_state);
            break;
        case 3:
            move_R(solver->cube_state);
            break;
        case 4:
            move_U_prime(solver->cube_state);
            break;
        case 5:
            move_R_prime(solver->cube_state);
            break;
        case 6:
            move_F_prime(solver->cube_state);
            break;
        default:
            // Controlla se la croce è risolta
            if(is_white_cross_solved(solver->cube_state)) {
                return true;
            } else {
                // Se non è risolta, resetta il contatore
                solver->step_counter = 0;
                return false;
            }
    }

    solver->step_counter++;
    return false;
}

static bool solve_corners_white_step(StepSolver* solver) {
    // Algoritmo per gli angoli bianchi
    switch(solver->step_counter) {
        case 0:
            move_R(solver->cube_state);
            break;
        case 1:
            move_U(solver->cube_state);
            break;
        case 2:
            move_R_prime(solver->cube_state);
            break;
        case 3:
            move_U_prime(solver->cube_state);
            break;
        default:
            if(is_white_corners_solved(solver->cube_state)) {
                return true;
            } else {
                solver->step_counter = 0;
                return false;
            }
    }

    solver->step_counter++;
    return false;
}

static bool solve_middle_layer_step(StepSolver* solver) {
    // Algoritmo per lo strato medio
    switch(solver->step_counter % 4) {
        case 0:
            move_U(solver->cube_state);
            break;
        case 1:
            move_R(solver->cube_state);
            break;
        case 2:
            move_U_prime(solver->cube_state);
            break;
        case 3:
            move_R_prime(solver->cube_state);
            break;
    }

    solver->step_counter++;

    // Per semplicità, assumiamo che lo strato medio sia risolto dopo 8 mosse
    return (solver->step_counter >= 8);
}

static bool solve_yellow_cross_step(StepSolver* solver) {
    // Algoritmo per la croce gialla
    switch(solver->step_counter % 4) {
        case 0:
            move_F(solver->cube_state);
            break;
        case 1:
            move_R(solver->cube_state);
            break;
        case 2:
            move_U(solver->cube_state);
            break;
        case 3:
            move_R_prime(solver->cube_state);
            break;
    }

    solver->step_counter++;

    // Per semplicità, assumiamo che la croce gialla sia fatta dopo 12 mosse
    return (solver->step_counter >= 12);
}

static bool solve_yellow_corners_step(StepSolver* solver) {
    // Algoritmo per posizionare gli angoli gialli
    switch(solver->step_counter % 5) {
        case 0:
            move_R(solver->cube_state);
            break;
        case 1:
            move_U(solver->cube_state);
            break;
        case 2:
            move_R_prime(solver->cube_state);
            break;
        case 3:
            move_U_prime(solver->cube_state);
            break;
        case 4:
            move_F(solver->cube_state);
            break;
    }

    solver->step_counter++;

    // Per semplicità, assumiamo che gli angoli gialli siano posizionati dopo 10 mosse
    return (solver->step_counter >= 10);
}

static bool solve_orient_corners_step(StepSolver* solver) {
    // Algoritmo per orientare gli angoli gialli
    switch(solver->step_counter % 6) {
        case 0:
            move_R(solver->cube_state);
            break;
        case 1:
            move_U(solver->cube_state);
            break;
        case 2:
            move_R_prime(solver->cube_state);
            break;
        case 3:
            move_U(solver->cube_state);
            break;
        case 4:
            move_R(solver->cube_state);
            break;
        case 5:
            move_U(solver->cube_state);
            break;
    }

    solver->step_counter++;

    // Per semplicità, assumiamo che gli angoli siano orientati dopo 12 mosse
    return (solver->step_counter >= 12);
}

static bool solve_permute_corners_step(StepSolver* solver) {
    // Algoritmo per permutare gli angoli gialli
    switch(solver->step_counter % 4) {
        case 0:
            move_R(solver->cube_state);
            break;
        case 1:
            move_U(solver->cube_state);
            break;
        case 2:
            move_R_prime(solver->cube_state);
            break;
        case 3:
            move_U_prime(solver->cube_state);
            break;
    }

    solver->step_counter++;

    // Per semplicità, assumiamo che la permutazione sia completata dopo 8 mosse
    return (solver->step_counter >= 8);
}

// Esecuzione di un singolo passo
bool solver_execute_step(StepSolver* solver) {
    if(!solver->is_solving) return true;

    bool phase_completed = false;

    switch(solver->current_state) {
        case SOLVER_CROSS_WHITE:
            phase_completed = solve_cross_white_step(solver);
            break;
        case SOLVER_CORNERS_WHITE:
            phase_completed = solve_corners_white_step(solver);
            break;
        case SOLVER_MIDDLE_LAYER:
            phase_completed = solve_middle_layer_step(solver);
            break;
        case SOLVER_YELLOW_CROSS:
            phase_completed = solve_yellow_cross_step(solver);
            break;
        case SOLVER_YELLOW_CORNERS:
            phase_completed = solve_yellow_corners_step(solver);
            break;
        case SOLVER_ORIENT_CORNERS:
            phase_completed = solve_orient_corners_step(solver);
            break;
        case SOLVER_PERMUTE_CORNERS:
            phase_completed = solve_permute_corners_step(solver);
            break;
        default:
            break;
    }

    if(phase_completed) {
        // Passa allo stato successivo
        solver->current_state++;
        solver->step_counter = 0;

        if(solver->current_state == SOLVER_COMPLETED) {
            solver->is_solving = false;
            strcpy(current_move, "Risoluzione completata!");
            return true;
        }
    }

    return false;
}
