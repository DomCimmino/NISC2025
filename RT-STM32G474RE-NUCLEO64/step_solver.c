#include "step_solver.h"
#include "moves.h"
#include "display.h"
#include "ch.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
  Assunzioni:
  - cube_state[0..8]   = U face
  - cube_state[9..17]  = R face
  - cube_state[18..26] = F face
  - cube_state[27..35] = D face
  - cube_state[36..44] = L face
  - cube_state[45..53] = B face

  face center index = face*9 + 4
*/

static const char *ALL_MOVES_STR[] = {
    "U","U'","D","D'","L","L'","R","R'","F","F'","B","B'"
};
#define N_ALL_MOVES (sizeof(ALL_MOVES_STR)/sizeof(ALL_MOVES_STR[0]))

// mapping inverse index to avoid trivial backtracking
static int inverse_move_idx(int idx) {
    // pairs: 0<->1 (U U'), 2<->3 (D D'), 4<->5 (L L'), 6<->7 (R R'), 8<->9 (F F'), 10<->11 (B B')
    if (idx % 2 == 0) return idx + 1;
    return idx - 1;
}

/* ----------------- helpers to apply move by index/string ----------------- */

static void apply_move_by_index(char cube_state[54], int mv_idx) {
    const char *m = ALL_MOVES_STR[mv_idx];
    if (strcmp(m,"U")==0) move_U(cube_state);
    else if (strcmp(m,"U'")==0) move_Ui(cube_state);
    else if (strcmp(m,"D")==0) move_D(cube_state);
    else if (strcmp(m,"D'")==0) move_Di(cube_state);
    else if (strcmp(m,"L")==0) move_L(cube_state);
    else if (strcmp(m,"L'")==0) move_Li(cube_state);
    else if (strcmp(m,"R")==0) move_R(cube_state);
    else if (strcmp(m,"R'")==0) move_Ri(cube_state);
    else if (strcmp(m,"F")==0) move_F(cube_state);
    else if (strcmp(m,"F'")==0) move_Fi(cube_state);
    else if (strcmp(m,"B")==0) move_B(cube_state);
    else if (strcmp(m,"B'")==0) move_Bi(cube_state);
}

/* ----------------- global helpers ----------------- */

static inline int face_center_index(int face) { return face*9 + 4; }
static inline char face_center_color(char cube_state[54], int face) { return cube_state[face_center_index(face)]; }

/* find which face index contains a given color as center */
static int face_of_color(char cube_state[54], char color) {
    for (int f=0; f<6; ++f) {
        if (cube_state[face_center_index(f)] == color) return f;
    }
    return -1;
}

/* Edge and corner tables: global facelet indices */
static const int EDGE_POS[12][2] = {
    /* UF */ { 0*9 + 7, 2*9 + 1 },  // U7 - F1
    /* UR */ { 0*9 + 5, 1*9 + 1 },  // U5 - R1
    /* UB */ { 0*9 + 1, 5*9 + 1 },  // U1 - B1
    /* UL */ { 0*9 + 3, 4*9 + 1 },  // U3 - L1
    /* DF */ { 3*9 + 1, 2*9 + 7 },  // D1 - F7
    /* DR */ { 3*9 + 5, 1*9 + 7 },  // D5 - R7
    /* DB */ { 3*9 + 7, 5*9 + 7 },  // D7 - B7
    /* DL */ { 3*9 + 3, 4*9 + 7 },  // D3 - L7
    /* FR */ { 2*9 + 5, 1*9 + 3 },  // F5 - R3
    /* FL */ { 2*9 + 3, 4*9 + 5 },  // F3 - L5
    /* BR */ { 5*9 + 3, 1*9 + 5 },  // B3 - R5
    /* BL */ { 5*9 + 5, 4*9 + 3 }   // B5 - L3
};

static const int CORNER_POS[8][3] = {
    /* URF */ { 0*9 + 8, 1*9 + 0, 2*9 + 2 }, // U8 R0 F2
    /* UFL */ { 0*9 + 6, 2*9 + 0, 4*9 + 2 }, // U6 F0 L2
    /* ULB */ { 0*9 + 0, 4*9 + 0, 5*9 + 2 }, // U0 L0 B2
    /* UBR */ { 0*9 + 2, 5*9 + 0, 1*9 + 2 }, // U2 B0 R2
    /* DFR */ { 3*9 + 2, 2*9 + 8, 1*9 + 6 }, // D2 F8 R6
    /* DLF */ { 3*9 + 0, 4*9 + 8, 2*9 + 6 }, // D0 L8 F6
    /* DBL */ { 3*9 + 6, 5*9 + 8, 4*9 + 6 }, // D6 B8 L6
    /* DRB */ { 3*9 + 8, 1*9 + 8, 5*9 + 6 }  // D8 R8 B6
};

/* get pair/triple index of which edge/corner contains a given facelet index */
static int edge_index_containing_facelet(int facelet_index) {
    for (int e=0;e<12;e++) {
        if (EDGE_POS[e][0]==facelet_index || EDGE_POS[e][1]==facelet_index) return e;
    }
    return -1;
}
static int corner_index_containing_facelet(int facelet_index) {
    for (int c=0;c<8;c++) {
        if (CORNER_POS[c][0]==facelet_index || CORNER_POS[c][1]==facelet_index || CORNER_POS[c][2]==facelet_index) return c;
    }
    return -1;
}

/* ----------------- goal tests ----------------- */

/* Check if edge (global facelet slot `slot`) contains 'W' in that slot and the partner color matches the partner face center */
static bool is_edge_correct_at_slot(char cube_state[54], int slot) {
    // find the edge pair that has this slot
    int e = edge_index_containing_facelet(slot);
    if (e < 0) return false;
    int a = EDGE_POS[e][0];
    int b = EDGE_POS[e][1];
    int other = (a == slot) ? b : a;
    // the face (center) corresponding to 'slot' is the face index slot/9
    int face_slot_face = slot / 9;
    int other_face = other / 9;
    char desired_center = cube_state[face_center_index(face_slot_face)];
    char desired_other_center = cube_state[face_center_index(other_face)];
    return (cube_state[slot] == desired_center) && (cube_state[other] == desired_other_center);
}

/* check if corner at a corner slot (global index) is correct (all 3 colors match centers) */
static bool is_corner_correct_at_slot(char cube_state[54], int corner_slot_facelet) {
    int cidx = corner_index_containing_facelet(corner_slot_facelet);
    if (cidx < 0) return false;
    int a = CORNER_POS[cidx][0];
    int b = CORNER_POS[cidx][1];
    int c = CORNER_POS[cidx][2];
    int fa = a/9, fb = b/9, fc = c/9;
    char ca = cube_state[face_center_index(fa)];
    char cb = cube_state[face_center_index(fb)];
    char cc = cube_state[face_center_index(fc)];
    // corner is correct if the three facelets contain exactly the three center colors (order doesn't matter)
    bool foundA = (cube_state[a]==ca) || (cube_state[a]==cb) || (cube_state[a]==cc);
    bool foundB = (cube_state[b]==ca) || (cube_state[b]==cb) || (cube_state[b]==cc);
    bool foundC = (cube_state[c]==ca) || (cube_state[c]==cb) || (cube_state[c]==cc);
    return foundA && foundB && foundC;
}

/* ----------------- IDDFS search for placing a target condition -----------------
   We implement a generic IDDFS: try sequences of moves up to depth 'max_d' to
   satisfy 'predicate(cube_state, param)'. For performance we avoid immediate inverses.
   predicate must be a function that returns true when goal reached.
*/

typedef bool (*goal_pred_t)(char cube_state[54], void *param);

static bool iddfs_rec(char node[54], int depth, int max_d, int last_move_idx, goal_pred_t pred, void *param, int *out_seq, int *out_len) {
    if (pred(node, param)) {
        *out_len = 0;
        return true;
    }
    if (depth == max_d) return false;

    for (int mv = 0; mv < N_ALL_MOVES; ++mv) {
        if (last_move_idx >= 0 && inverse_move_idx(mv) == last_move_idx) continue; // avoid undo
        char nxt[54];
        memcpy(nxt, node, 54);
        apply_move_by_index(nxt, mv);

        // recurse
        if (iddfs_rec(nxt, depth+1, max_d, mv, pred, param, out_seq, out_len)) {
            // prepend this move
            // shift out_seq by +1
            for (int k = *out_len - 1; k >= 0; --k) out_seq[k+1] = out_seq[k];
            out_seq[0] = mv;
            (*out_len)++;
            return true;
        }
    }
    return false;
}

/* wrapper that runs IDDFS increasing depth from 0..max_depth.
   Returns true and fills out_seq/out_len (sequence of move indices) on success.
*/
static bool find_sequence_iddfs(char cube_state[54], int max_depth, goal_pred_t pred, void *param, int *out_seq, int *out_len) {
    for (int d = 0; d <= max_depth; ++d) {
        // call rec with copies
        char copy[54];
        memcpy(copy, cube_state, 54);
        if (iddfs_rec(copy, 0, d, -1, pred, param, out_seq, out_len)) {
            return true;
        }
    }
    return false;
}

/* ----------------- predicate implementations for common goals ----------------- */

/* parameter struct for edge goal: target_slot (global index) */
typedef struct { int target_slot; } edge_goal_param_t;

static bool edge_goal_pred(char cube_state[54], void *param) {
    edge_goal_param_t *p = (edge_goal_param_t*)param;
    return is_edge_correct_at_slot(cube_state, p->target_slot);
}

/* parameter struct for corner goal: target_corner_facelet (any of 3 facelets for that corner) */
typedef struct { int corner_facelet; } corner_goal_param_t;

static bool corner_goal_pred(char cube_state[54], void *param) {
    corner_goal_param_t *p = (corner_goal_param_t*)param;
    return is_corner_correct_at_slot(cube_state, p->corner_facelet);
}

/* ----------------- high-level placement routines ----------------- */

/* place all 4 edges of face 'white_face' (face index) into its edge slots */
static void place_cross_for_face(char cube_state[54], int face_idx, int delay_ms) {
    // target slots on this face are the edge facelets: 1,3,5,7 offset by face*9
    int base = face_idx * 9;
    int target_slots[4] = { base + 1, base + 3, base + 5, base + 7 };
    const int SEARCH_MAX_DEPTH = 6; // adjustable

    for (int i = 0; i < 4; ++i) {
        edge_goal_param_t param = { .target_slot = target_slots[i] };
        int seq[64];
        int seq_len = 0;
        // quick check: if already correct continue
        if (edge_goal_pred(cube_state, &param)) continue;

        bool found = find_sequence_iddfs(cube_state, SEARCH_MAX_DEPTH, edge_goal_pred, &param, seq, &seq_len);
        if (!found) {
            // try increasing depth
            found = find_sequence_iddfs(cube_state, SEARCH_MAX_DEPTH+2, edge_goal_pred, &param, seq, &seq_len);
        }
        if (!found) {
            // fallback: brute place with some naive sequence (try rotating U rows to bring edges)
            // Try rotate U up to 4 times and test again
            for (int r=0;r<4 && !found;r++) {
                apply_move_by_index(cube_state, 0); // U
                rubikDrawNetFromCube(cube_state, 10, 10);
                chThdSleepMilliseconds(delay_ms);
                found = edge_goal_pred(cube_state, &param);
            }
            if (!found) {
                // give up this edge (rare), continue
                continue;
            }
        }
        // apply found sequence
        for (int k=0;k<seq_len;k++) {
            apply_move_by_index(cube_state, seq[k]);
            rubikDrawNetFromCube(cube_state, 10, 10);
            chThdSleepMilliseconds(delay_ms);
        }
    }
}

/* place all 4 white corners on face 'white_face' */
static void place_corners_for_face(char cube_state[54], int face_idx, int delay_ms) {
    // corner facelets on a face are: top-left(0), top-right(2), bottom-right(8), bottom-left(6)
    int base = face_idx * 9;
    int corner_facelets[4] = { base+0, base+2, base+8, base+6 };
    const int SEARCH_MAX_DEPTH = 7;

    for (int i=0;i<4;i++) {
        corner_goal_param_t param = { .corner_facelet = corner_facelets[i] };
        if (corner_goal_pred(cube_state, &param)) continue;

        int seq[128]; int seq_len=0;
        bool found = find_sequence_iddfs(cube_state, SEARCH_MAX_DEPTH, corner_goal_pred, &param, seq, &seq_len);
        if (!found) found = find_sequence_iddfs(cube_state, SEARCH_MAX_DEPTH+2, corner_goal_pred, &param, seq, &seq_len);

        if (!found) {
            // naive attempt: rotate U until corner appears in some convenient place
            for (int r=0;r<4 && !found;r++) {
                apply_move_by_index(cube_state, 0); // U
                rubikDrawNetFromCube(cube_state, 10, 10);
                chThdSleepMilliseconds(delay_ms);
                found = corner_goal_pred(cube_state, &param);
            }
            if (!found) continue;
        }

        for (int k=0;k<seq_len;k++) {
            apply_move_by_index(cube_state, seq[k]);
            rubikDrawNetFromCube(cube_state, 10, 10);
            chThdSleepMilliseconds(delay_ms);
        }
    }
}

/* place second layer edges (non U/D edges) */
static void solve_second_layer(char cube_state[54], int delay_ms) {
    // target second-layer edge slots are the four edges between F/R, R/B, B/L, L/F:
    int target_slots[4] = {
        EDGE_POS[8][0], // FR facelet (F5 global index)
        EDGE_POS[10][0],// BR facelet (B3)
        EDGE_POS[11][0],// BL facelet (B5?) (we will just iterate via edge slots)
        EDGE_POS[9][0]  // FL facelet (F3)
    };
    const int SEARCH_MAX_DEPTH = 8;

    // For simplicity, iterate the 4 general mid-layer edge slots and try to position them
    for (int i=0;i<4;i++) {
        edge_goal_param_t param = { .target_slot = target_slots[i] };
        if (edge_goal_pred(cube_state, &param)) continue;

        int seq[128]; int seq_len=0;
        bool found = find_sequence_iddfs(cube_state, SEARCH_MAX_DEPTH, edge_goal_pred, &param, seq, &seq_len);
        if (!found) found = find_sequence_iddfs(cube_state, SEARCH_MAX_DEPTH+2, edge_goal_pred, &param, seq, &seq_len);
        if (!found) continue;

        for (int k=0;k<seq_len;k++) {
            apply_move_by_index(cube_state, seq[k]);
            rubikDrawNetFromCube(cube_state, 10, 10);
            chThdSleepMilliseconds(delay_ms);
        }
    }
}

/* Yellow cross and orientations: try to orient D-face edges to match center (use F R U R' U' F') */
static void solve_yellow_cross(char cube_state[54], int yellow_face_idx, int delay_ms) {
    // apply algorithm repeatedly until cross achieved on yellow_face
    const char *alg[] = { "F","R","U","R'","U'","F'" };
    int iter = 0;
    while (iter < 6) {
        // check if all edge facelets on yellow_face are yellow
        int base = yellow_face_idx*9;
        if (cube_state[base+1] == cube_state[face_center_index(yellow_face_idx)] &&
            cube_state[base+3] == cube_state[face_center_index(yellow_face_idx)] &&
            cube_state[base+5] == cube_state[face_center_index(yellow_face_idx)] &&
            cube_state[base+7] == cube_state[face_center_index(yellow_face_idx)]) {
            break;
        }

        // apply alg
        for (int i=0;i< (int)(sizeof(alg)/sizeof(alg[0])); ++i) {
            // apply single move by name
            if (strcmp(alg[i],"U")==0) move_U(cube_state);
            else if (strcmp(alg[i],"U'")==0) move_Ui(cube_state);
            else if (strcmp(alg[i],"D")==0) move_D(cube_state);
            else if (strcmp(alg[i],"D'")==0) move_Di(cube_state);
            else if (strcmp(alg[i],"L")==0) move_L(cube_state);
            else if (strcmp(alg[i],"L'")==0) move_Li(cube_state);
            else if (strcmp(alg[i],"R")==0) move_R(cube_state);
            else if (strcmp(alg[i],"R'")==0) move_Ri(cube_state);
            else if (strcmp(alg[i],"F")==0) move_F(cube_state);
            else if (strcmp(alg[i],"F'")==0) move_Fi(cube_state);
            else if (strcmp(alg[i],"B")==0) move_B(cube_state);
            else if (strcmp(alg[i],"B'")==0) move_Bi(cube_state);

            rubikDrawNetFromCube(cube_state, 10, 10);
            chThdSleepMilliseconds(delay_ms);
        }
        iter++;
    }
}

/* Position yellow corners (permute), we attempt standard corner permutation alg until corners in place */
static void position_yellow_corners(char cube_state[54], int yellow_face_idx, int delay_ms) {
    // corner permutation algorithm: U R U' L' U R' U' L
    const char *alg[] = {"U","R","U'","L'","U","R'","U'","L"};
    int attempts = 0;
    while (attempts < 10) {
        // check corners at yellow face
        int base = yellow_face_idx*9;
        bool all_corners_ok = true;
        int corner_facelets[4] = { base+0, base+2, base+8, base+6 };
        for (int i=0;i<4;i++) {
            if (!is_corner_correct_at_slot(cube_state, corner_facelets[i])) { all_corners_ok = false; break; }
        }
        if (all_corners_ok) break;

        for (int i=0;i<(int)(sizeof(alg)/sizeof(alg[0])); ++i) {
            // apply move
            if (strcmp(alg[i],"U")==0) move_U(cube_state);
            else if (strcmp(alg[i],"U'")==0) move_Ui(cube_state);
            else if (strcmp(alg[i],"D")==0) move_D(cube_state);
            else if (strcmp(alg[i],"D'")==0) move_Di(cube_state);
            else if (strcmp(alg[i],"L")==0) move_L(cube_state);
            else if (strcmp(alg[i],"L'")==0) move_Li(cube_state);
            else if (strcmp(alg[i],"R")==0) move_R(cube_state);
            else if (strcmp(alg[i],"R'")==0) move_Ri(cube_state);
            else if (strcmp(alg[i],"F")==0) move_F(cube_state);
            else if (strcmp(alg[i],"F'")==0) move_Fi(cube_state);
            else if (strcmp(alg[i],"B")==0) move_B(cube_state);
            else if (strcmp(alg[i],"B'")==0) move_Bi(cube_state);

            rubikDrawNetFromCube(cube_state, 10, 10);
            chThdSleepMilliseconds(delay_ms);
        }
        attempts++;
    }
}

/* Orient yellow corners (bring orientation correct) - standard: R' D' R D repeated */
static void orient_yellow_corners(char cube_state[54], int yellow_face_idx, int delay_ms) {
    // For each corner on yellow face, apply R' D' R D until yellow face color is on the face
    int base = yellow_face_idx*9;
    int corner_facelets[4] = { base+0, base+2, base+8, base+6 };
    for (int i=0;i<4;i++) {
        // rotate U to bring corner to URF (or some known corner) position for algorithm
        int tries = 0;
        while (tries < 4) {
            int cf = corner_facelets[i];
            if (cube_state[cf] == cube_state[face_center_index(yellow_face_idx)]) break;
            // apply the R' D' R D sequence until that corner is oriented
            // We'll attempt up to 4 cycles
            int cycles = 0;
            while (cube_state[cf] != cube_state[face_center_index(yellow_face_idx)] && cycles < 6) {
                // R' D' R D
                move_Ri(cube_state);
                rubikDrawNetFromCube(cube_state, 10, 10); chThdSleepMilliseconds(delay_ms);
                move_Di(cube_state);
                rubikDrawNetFromCube(cube_state, 10, 10); chThdSleepMilliseconds(delay_ms);
                move_R(cube_state);
                rubikDrawNetFromCube(cube_state, 10, 10); chThdSleepMilliseconds(delay_ms);
                move_D(cube_state);
                rubikDrawNetFromCube(cube_state, 10, 10); chThdSleepMilliseconds(delay_ms);
                cycles++;
            }
            // rotate U to go to next corner
            move_U(cube_state);
            rubikDrawNetFromCube(cube_state, 10, 10); chThdSleepMilliseconds(delay_ms);
            tries++;
        }
    }
}

/* ----------------- main solver orchestration ----------------- */

void solve_cube_step_by_step(char cube_state[54], int delay_ms) {

    int white_face = face_of_color(cube_state, 'W');
    int yellow_face = face_of_color(cube_state, 'Y');
    if (white_face < 0) white_face = 0;
    if (yellow_face < 0) yellow_face = 3;

    // 1) White cross (place the 4 edges on the white face)
    place_cross_for_face(cube_state, white_face, delay_ms);

    // 2) White corners
    place_corners_for_face(cube_state, white_face, delay_ms);

    // 3) Second layer (middle edges)
    solve_second_layer(cube_state, delay_ms);

    // 4) Yellow cross on D face (yellow_face)
    solve_yellow_cross(cube_state, yellow_face, delay_ms);

    // 5) Position yellow corners
    position_yellow_corners(cube_state, yellow_face, delay_ms);

    // 6) Orient yellow corners
    orient_yellow_corners(cube_state, yellow_face, delay_ms);

    // final: redraw to ensure done
    rubikDrawNetFromCube(cube_state, 10, 10);
}
