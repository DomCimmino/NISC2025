#include "display.h"
#include "gfx.h"
#include "step_solver.h"
#include <stdlib.h>
#include <string.h>

#define R_SIZE          3
#define CELL            18
#define GAP             2
#define FACE_BORDER     2
#define NET_GAP         8

#define JOY_PORT_1      GPIOB
#define JOY_PORT_2      GPIOC
#define JOY_UP_PIN          0
#define JOY_DOWN_PIN        4
#define JOY_LEFT_PIN        6
#define JOY_RIGHT_PIN       0
#define JOY_CENTRE_PIN      7

static gColor kPalette[COL_CNT];
static unsigned char stickers[FACE_CNT][R_SIZE][R_SIZE];

// Variabili per la risoluzione step-by-step
static StepSolver solver;
static bool solving = false;
static bool solution_complete = false;
static uint32_t last_press_time = 0;

static void initPalette(void) {
    kPalette[COL_W] = GFX_WHITE;
    kPalette[COL_Y] = GFX_YELLOW;
    kPalette[COL_R] = GFX_RED;
    kPalette[COL_O] = GFX_ORANGE;
    kPalette[COL_B] = GFX_BLUE;
    kPalette[COL_G] = HTML2COLOR(0x009000);
}

void rubikInit(void) {
    initPalette();
    palSetPadMode(JOY_PORT_2, JOY_UP_PIN, PAL_MODE_INPUT_PULLUP);            //UP
    palSetPadMode(JOY_PORT_1, JOY_DOWN_PIN, PAL_MODE_INPUT_PULLUP);          //DOWN
    palSetPadMode(JOY_PORT_1, JOY_LEFT_PIN, PAL_MODE_INPUT_PULLUP);          //LEFT
    palSetPadMode(JOY_PORT_1, JOY_RIGHT_PIN, PAL_MODE_INPUT_PULLUP);         //RIGHT
    palSetPadMode(JOY_PORT_2, JOY_CENTRE_PIN, PAL_MODE_INPUT_PULLUP);        //CENTRE
    srand(gfxSystemTicks());
    for (int f=0; f<FACE_CNT; ++f)
      for (int r=0; r<R_SIZE; ++r)
        for (int c=0; c<R_SIZE; ++c)
          stickers[f][r][c] = (unsigned char)(f % COL_CNT);
}

void display_init(void) {
    gfxInit();
    rubikInit();
    gdispClear(GFX_BLACK);
}

void rubikUpdateFromCubeState(char cube_state[54]) {
    for (int f = 0; f < FACE_CNT; f++) {
        for (int r = 0; r < R_SIZE; r++) {
            for (int c = 0; c < R_SIZE; c++) {
                char ch = cube_state[f*R_SIZE*R_SIZE + r*R_SIZE + c];
                switch(ch) {
                    case 'W': stickers[f][r][c] = COL_W; break;
                    case 'Y': stickers[f][r][c] = COL_Y; break;
                    case 'R': stickers[f][r][c] = COL_R; break;
                    case 'O': stickers[f][r][c] = COL_O; break;
                    case 'B': stickers[f][r][c] = COL_B; break;
                    case 'G': stickers[f][r][c] = COL_G; break;
                    default:  stickers[f][r][c] = COL_W; break;
                }
            }
        }
    }
}

typedef struct { int fx, fy; } FacePos;

static FacePos facePos(RubikFace f) {
    switch (f) {
        case FACE_U: return (FacePos){ 1,0 };
        case FACE_L: return (FacePos){ 0,1 };
        case FACE_F: return (FacePos){ 1,1 };
        case FACE_R: return (FacePos){ 2,1 };
        case FACE_B: return (FacePos){ 3,1 };
        case FACE_D: return (FacePos){ 1,2 };
        default:     return (FacePos){ 1,1 };
    }
}

static inline int facePixelSize(void) {
    return FACE_BORDER*2 + (R_SIZE*CELL) + ((R_SIZE-1)*GAP);
}

static void drawSticker(gCoord x, gCoord y, gColor col) {
    gdispFillArea(x, y, CELL, CELL, col);
    gdispDrawBox(x, y, CELL, CELL, GFX_BLACK);
}

static void drawFace(gCoord x0, gCoord y0, RubikFace f) {
    const int FP = facePixelSize();
    gdispFillArea(x0, y0, FP, FP, GFX_BLACK); // bordo esterno
    gdispFillArea(x0+FACE_BORDER, y0+FACE_BORDER, FP-2*FACE_BORDER, FP-2*FACE_BORDER, GFX_GRAY);
    gCoord sx = x0 + FACE_BORDER, sy = y0 + FACE_BORDER;
    for (int r=0; r<R_SIZE; ++r)
        for (int c=0; c<R_SIZE; ++c)
            drawSticker(sx + c*(CELL+GAP), sy + r*(CELL+GAP), kPalette[stickers[f][r][c]]);
}

void rubikDrawNetFromCube(char cube_state[54], gCoord ox, gCoord oy) {
    rubikUpdateFromCubeState(cube_state);
    const int FP = facePixelSize(); (void)FP;
    for (int f=0; f<FACE_CNT; ++f) {
        FacePos p = facePos((RubikFace)f);
        gCoord x = ox + p.fx*(facePixelSize() + NET_GAP);
        gCoord y = oy + p.fy*(facePixelSize() + NET_GAP);
        drawFace(x, y, (RubikFace)f);
    }
}

static bool is_button_pressed(ioportid_t port, uint16_t pad) {
    return palReadPad(port, pad) == PAL_LOW;
}

static bool is_center_pressed(void) {
    return is_button_pressed(JOY_PORT_2, JOY_CENTRE_PIN);
}

static bool is_right_pressed(void){
    return is_button_pressed(JOY_PORT_1, JOY_RIGHT_PIN);
}

// Funzione per visualizzare lo stato del solver
void display_solver_status(void) {
    // Pulisci l'area di testo
    gdispFillArea(0, 0, gdispGetWidth(), 20, GFX_BLACK);

    // Mostra lo stato corrente
    gdispDrawString(10, 5, solver_get_state_name(solver.current_state),
                   NULL, GFX_WHITE);

    // Mostra la mossa corrente
    gdispDrawString(150, 5, solver_get_current_move(),
                   NULL, GFX_YELLOW);

    if (solution_complete) {
        gdispDrawString(250, 5, "COMPLETATO!",
                       NULL, GFX_GREEN);
    }
}

// Inizia la risoluzione
void start_solution(char cube_state[54]) {
    solver_init(&solver, cube_state);
    solving = true;
    solution_complete = false;
}

// Esegue un passo della risoluzione
void execute_solution_step(char cube_state[54]) {
    if (!solving || solution_complete) return;

    // Esegui un passo
    solution_complete = solver_execute_step(&solver);

    // Aggiorna lo stato del cubo
    memcpy(cube_state, solver.cube_state, 54);

    // Aggiorna il display
    display_solver_status();
}

// Gestione della navigazione con risoluzione step-by-step
void handle_navigation(char cube_state[54]) {
    uint32_t current_time = chVTGetSystemTime();

    // Debounce
    if (current_time - last_press_time < TIME_MS2I(300)) {
        return;
    }

    // Tasto centrale: inizia la risoluzione
    if (is_center_pressed()) {
        last_press_time = current_time;
        if (!solving) {
            start_solution(cube_state);
            display_solver_status();
        }
    }

    // Tasto destro: esegui passo successivo
    if (is_right_pressed() && solving) {
        last_press_time = current_time;
        execute_solution_step(cube_state);
    }
}

// Funzione principale per la risoluzione step-by-step
void solve_cube_step_by_step(char cube_state[54], uint16_t delay_ms) {
    (void)delay_ms; // Non usiamo il delay in modalità step-by-step

    // Inizializza il solver
    start_solution(cube_state);

    // Loop principale (verrà gestito da handle_navigation)
    while (!solution_complete) {
        // La gestione dei passi avviene tramite handle_navigation
        chThdSleepMilliseconds(100);
    }
}
