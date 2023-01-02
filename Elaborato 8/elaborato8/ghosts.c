#define GHOSTS_STUD
#ifdef GHOSTS_STUD

/*#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ghosts.h"
#include "pacman.h"
#include "matrix.h"
#include "global.h"

static const struct position UNK_POSITION = { -1,-1 }; // Set unknown position to (UINT_MAX,UINT_MAX)

struct ghost {
    int id; 
    int status;
    int dir; // ?? enum direction dir ??
    struct position pos;
};

struct ghosts {
    char** arena;
    unsigned int rows;
    unsigned int cols;
    unsigned int nghosts;
    struct ghost* ghost;
};

static struct position ghost_move_normal(struct ghosts* G, struct pacman* P, unsigned int id);
static struct position ghost_move_scared(struct ghosts* G, struct pacman* P, unsigned int id);
static struct position ghost_move_eyes(struct ghosts* G, struct pacman* P, unsigned int id);

/* Create the ghosts data structure */
struct ghosts* ghosts_setup(unsigned int num_ghosts) {
    struct ghosts* G = (struct ghosts*)malloc(sizeof(struct ghosts));
    unsigned int i;
    srand(time(NULL));
    if (G != NULL) {
        // al campo n di G assegnamo num_ghosts
        G->nghosts = num_ghosts;

        //al campo ghost di G assegniamo dinamicamente la memoria
        G->ghost = (struct ghost*)calloc(num_ghosts, sizeof(struct ghost));
        // g->ghost = (struct ghost*)malloc(num_ghosts * sizeof(struct ghost))

        //per ogni ghost presente assegnamo una UNK_POSITION e una direzione casuale
        for (i = 0; i < G->nghosts; i++) {
            G->ghost[i].pos = UNK_POSITION;
            G->ghost[i].dir = rand() % 4; // 0 to 6
        }
    }
    return G;
}

/* Return the number of ghosts */
unsigned int ghosts_get_number(struct ghosts* G) {
    return G->nghosts;
}

/* Destroy the ghost data structure */
void ghosts_destroy(struct ghosts* G) {
    if (G != NULL) {
        size_t i;
        free(G->ghost);
        /*for (i = 0; i < G->nghosts; i++)
            free(G[i].ghost);*/
        free(G);
    }
}

/* Set the arena (A) matrix */
void ghosts_set_arena(struct ghosts* G, char** A, unsigned int nrow, unsigned int ncol) {
    // se G è NON nullo
    if (G != NULL) {
        // assegnamo al campo A di G la matrice di gioco
        G->arena = A;
        // assegnamo al campo numero di righe il numero di righe e cosi per le colonne
        G->rows = nrow;
        G->cols = ncol;
    }
}

/* Set the position of the ghost id. */
void ghosts_set_position(struct ghosts* G, unsigned int id, struct position pos) {
    // se G è NON nullo e il suo identificativo è valido assegnamo al fantasma con quell’id la posizione
    if (G != NULL && id < G->nghosts) {
        G->ghost[id].pos = pos;
    }
}

/* Set the status of the ghost id. */
void ghosts_set_status(struct ghosts* G, unsigned int id, enum ghost_status status) {
    //se G è NON nullo e l’id è valido, assegnamo al ghost con quell’id uno stato
    if (G != NULL && id < G->nghosts) {
        G->ghost[id].status = status;
    }
}

/* Return the position of the ghost id. */
struct position ghosts_get_position(struct ghosts* G, unsigned int id) {
    struct position p = UNK_POSITION;
    // se G è NON nullo e il suo identificativo è valido assegnamo al fantasma con quell’id la posizione
    if (G != NULL && id < G->nghosts) {
        p = G->ghost[id].pos;
    }
    return p;
}

/* Return the status of the ghost id. */
enum ghost_status ghosts_get_status(struct ghosts* G, unsigned int id) {
    //se G è NON nullo e l’id è valido, assegnamo al ghost con quell’id uno stato
    if (G != NULL && id < G->nghosts) {
        return G->ghost[id].status;
    }
}

static int legal_position(struct ghosts* G, struct pacman* P, struct position pos, enum ghost_status status) {
    if (IS_WALL(G->arena, pos)) {
        return 0;
    }
    else {
        unsigned int i;
        struct position p = pacman_get_position(P);
        // check pacman intersect

        if (status != NORMAL) {
            if (pos.i == p.i && pos.j == p.j) {
                return 0;
            }
        }
        
        // check ghost intersect
        for (i = 0; i < G->nghosts; i++) {
            if (G->ghost[i].pos.i == pos.i && G->ghost[i].pos.j == pos.j) {
                return 0;
            }
        }
        return 1;
    }
}

// enum direction {LEFT, UP, RIGHT, DOWN, UNK_DIRECTION};
static struct position new_position(struct position pos, enum direction dir, unsigned int nrow, unsigned int ncol) {
    struct position new = pos;
    switch (dir) {
    case LEFT:  new.j = (pos.j + (ncol - 1)) % ncol; break;
    case RIGHT: new.j = (pos.j + 1) % ncol; break;
    case UP:    new.i = (pos.i + (nrow - 1)) % nrow; break;
    case DOWN:  new.i = (pos.i + 1) % nrow; break;
    case UNK_DIRECTION: break;
    }
    return new;
}

// QUI MODIFICA LA DIREZIONE PIU' VOLTE, BISOGNA CERCARE DI DARE UN PESO ALLA DIREZIONE DA PRENDERE
// enum direction {LEFT, UP, RIGHT, DOWN, UNK_DIRECTION};
static void setup_remaining_dir(struct ghosts* G, struct pacman* P,unsigned int id, enum direction dir[]) {
    unsigned int d = 4, i;
    dir[1] = dir[2] = dir[3] = UNK_DIRECTION;
    
    srand(time(NULL));
    
    switch (dir[0]) {
    case LEFT:  d = 2; break;
    case RIGHT: d = 0; break;
    case UP:    d = 3; break;
    case DOWN:  d = 1; break;
    case UNK_DIRECTION: d = 4; break;
    }
    /*
    unsigned int oPiccolo = 0;
    for (; oPiccolo != 2 && oPiccolo != 0;) {
        oPiccolo = rand() % 3;
        if (oPiccolo == 2) {
            dir[1] = RIGHT;
            dir[2] = LEFT;
        }
        if (oPiccolo == 0) {
            dir[2] = RIGHT;
            dir[1] = LEFT;
        }
    }

    for (; oPiccolo != 1 && oPiccolo != 3;) {
        oPiccolo = rand() % 4;
        if (oPiccolo == 1) {
            dir[1] = UP;
            dir[2] = DOWN;
        }
        if (oPiccolo == 3) {
            dir[2] = UP;
            dir[1] = DOWN;
        }
    }*/

    //dir[3] = d;

    if (dir[0] == UP || dir[0] == DOWN) {
        dir[1] = RIGHT;
        dir[2] = LEFT;
    }
    else if(dir[0] != UNK_DIRECTION) {
        dir[1] = DOWN;
        dir[2] = UP;
    }
    else {
        dir[0] = UP;
        dir[2] = LEFT;
        dir[1] = RIGHT;
        dir[3] = DOWN;
    }

    // se la new_position è legale, mantieni la direzione, altrimenti vai all'oppposto, altrimenti scegli random fra le due rimanenti.
    if (legal_position(G, P, new_position(G->ghost[id].pos, dir[0], G->rows, G->cols), G->ghost[id].status)) {
        // Higher priority to the current direction
        G->ghost[id].dir = dir[0];
        return;
    }
    else if(legal_position(G, P, new_position(G->ghost[id].pos, dir[1], G->rows, G->cols), G->ghost[id].status) || legal_position(G, P, new_position(G->ghost[id].pos, dir[2], G->rows, G->cols), G->ghost[id].status)) {
        // Random selection of the remaining directions
        if (legal_position(G, P, new_position(G->ghost[id].pos, dir[1], G->rows, G->cols), G->ghost[id].status)) {
            G->ghost[id].dir = dir[1];
            return;
        }
        else if (legal_position(G, P, new_position(G->ghost[id].pos, dir[2], G->rows, G->cols), G->ghost[id].status)) {
            G->ghost[id].dir = dir[2];
            return;
        }
    }
    else {
        // Lower priority to the direction opposite to the current one
        G->ghost[id].dir = d;
        return;
    }
}

static int nearby_home(char** A, unsigned int nrow, unsigned int ncol, struct position pos) {
    int n = 2, a, b, i = pos.i, j = pos.j;
    for (a = -n; a <= n; a++)
        if (i + a >= 0 && i + a < nrow)
            for (b = -n; b <= n; b++)
                if (j + b >= 0 && j + b < ncol)
                    if (A[i + a][j + b] == HOME_SYM)
                        return 1;
    return 0;
}

static void select_dir_towards(struct ghosts* G, struct pacman* P, unsigned int id, enum direction dir[]) {
    struct position g = ghosts_get_position(G, id);
    struct position p = pacman_get_position(P);
    if (nearby_home(G->arena, G->rows, G->cols, g))
        G->ghost[id].dir = UP;
    else if (g.i == p.i)
        G->ghost[id].dir = g.j < p.j ? RIGHT : LEFT; 
    else if (g.j == p.j)
        G->ghost[id].dir = g.i < p.i ? DOWN : UP; // se j ghost < j pacman allora il ghost è sopra pacman, quindi vado su, opposite a pacman
    else 
        setup_remaining_dir(G, P, id, dir);
}

static void select_dir_away(struct ghosts* G, struct pacman* P, unsigned int id, enum direction dir[]) {
    struct position g = ghosts_get_position(G, id);
    struct position p = pacman_get_position(P);
    if (nearby_home(G->arena, G->rows, G->cols, g)) // il ghost deve rincorrere da pacman, quindi se è nei pressi della casa, sale
        G->ghost[id].dir = DOWN;
    else if (g.i == p.i)
        G->ghost[id].dir = g.j < p.j ? LEFT : RIGHT;
    else if (g.j == p.j)
        G->ghost[id].dir = g.i < p.i ? UP : DOWN;
    else
        setup_remaining_dir(G, P, id, dir);
}

static void select_dir_home(struct ghosts* G, struct pacman* P, unsigned int id, enum direction dir[]) {
    int i = G->ghost[id].pos.i;
    int j = G->ghost[id].pos.j;
    char c = G->arena[i][j]; // carattere per ritornare velocemente nella casa dei fantasmi in path.txt
    dir[0] = c == 'L' ? LEFT : c == 'R' ? RIGHT : c == 'U' ? UP : DOWN; // setto la direzione a seconda del carattere
    setup_remaining_dir(G, P, id, dir); // ghosts, id del ghost in causa, direzione[0] setuppata con la nuova faster direzione per tornare nella casa dei fantasmi
}

static struct position ghost_move_normal(struct ghosts* G, struct pacman* P, unsigned int id) {
    enum direction dir[4] = {UNK_DIRECTION, UNK_DIRECTION, UNK_DIRECTION, UNK_DIRECTION};
    select_dir_towards(G, P, id, dir);
    struct position pos = G->ghost[id].pos;

    struct position new = new_position(pos, G->ghost[id].dir, G->rows, G->cols); // suggerisce la nuova posizione in cui direzionare il fantasma
    if (legal_position(G, P, new, G->ghost[id].status)) { // controllo se la posizione è possibile sia occupata
        G->ghost[id].pos = new; // nuova posizione ottenuta
        // assegno la nuova direzione provata
        return new; // ritorna la nuova posizione
    }
    return pos; // se non può aggiornare la posizione vuol dire che non può effettuare movimenti in nessuna direzione
}
static struct position ghost_move_scared(struct ghosts* G, struct pacman* P, unsigned int id) {
    enum direction dir[4] = { UNK_DIRECTION, UNK_DIRECTION, UNK_DIRECTION, UNK_DIRECTION };
    select_dir_away(G, P, id, dir);
    struct position pos = G->ghost[id].pos;

    struct position new = new_position(pos, G->ghost[id].dir, G->rows, G->cols); // suggerisce la nuova posizione in cui direzionare il fantasma

    if (legal_position(G, P, new, G->ghost[id].status)) { // controllo se la posizione è possibile sia occupata
        G->ghost[id].pos = new; // nuova posizione ottenuta
        return new; // ritorna la nuova posizione
    }
    return pos; // se non può aggiornare la posizione vuol dire che non può effettuare movimenti in nessuna direzione
}
static struct position ghost_move_eyes(struct ghosts* G, struct pacman* P, unsigned int id) {
    enum direction dir[4] = { UNK_DIRECTION, UNK_DIRECTION, UNK_DIRECTION, UNK_DIRECTION };
    select_dir_home(G, P, id, dir);
    struct position pos = G->ghost[id].pos;

    struct position new = new_position(pos, G->ghost[id].dir, G->rows, G->cols); // suggerisce la nuova posizione in cui direzionare il fantasma

    if (legal_position(G, P, new, G->ghost[id].status)) { // controllo se la posizione è possibile sia occupata
        G->ghost[id].pos = new; // nuova posizione ottenuta
        return new; // ritorna la nuova posizione
    }
    return pos; // se non può aggiornare la posizione vuol dire che non può effettuare movimenti in nessuna direzione
}

/* Move the ghost id (according to its status). Returns the new position */
struct position ghosts_move(struct ghosts* G, struct pacman* P, unsigned int id) {
    struct position p = UNK_POSITION;
    switch (ghosts_get_status(G, id)) {
    case NORMAL:
        return ghost_move_normal(G, P, id);
        break;
    case SCARED_NORMAL:
        return ghost_move_scared(G, P, id);
        break;
    case SCARED_BLINKING:
        return ghost_move_scared(G, P, id);
        break;
    case EYES:
        return ghost_move_eyes(G, P, id);
        break;
    case UNK_GHOST_STATUS:
        break;
    }
    return p;
}

#endif


