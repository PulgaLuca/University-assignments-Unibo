#include "game.h"
#include <stdio.h>

// Movimento della palla
#define LEFT -1 // orizzontale
#define	RIGHT 1
#define UP -1   // verticale
#define DOWN 1

#define SHIFT_COL(b,p,n) ((b).x == ((p).x + (n))) // controlla se la palla tocca pad1.
#define SHIFT_COL2(b,p,n) ((b).x+1 == ((p).x + (n))) // controlla se la palla tocca pad2.
#define SAME_ROW(b,p,len) ((b).y >= (p).y && (b).y <= ((p).y+(len))) // full touch della palla sui pad.
#define TOP_ROW(b,p) ((b).y == ((p).y-1)) // tocco bordo superiore del pad.
#define BOT_ROW(b,p,len) ((b).y == ((p).y+(len))) // tocco bordo inferiore del pad.

#define PAD1_TOP_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p1,-1) || SHIFT_COL(G.b,G.p1,0) || SHIFT_COL(G.b,G.p1,1)) && TOP_ROW(G.b,G.p1)) // tocco bordo sup pad1.
#define PAD1_BOT_BORDER_TOUCH(G) ((SHIFT_COL(G.b,G.p1,-1) || SHIFT_COL(G.b,G.p1,0) || SHIFT_COL(G.b,G.p1,1)) && BOT_ROW(G.b,G.p1,G.pad_len))  // tocco bordo inf pad1.

#define PAD2_TOP_BORDER_TOUCH(G) ((SHIFT_COL2(G.b,G.p2,-1) || SHIFT_COL2(G.b,G.p2,1) || SHIFT_COL2(G.b,G.p2,0)) && TOP_ROW(G.b,G.p2)) // tocco bordo sup pad2.
#define PAD2_BOT_BORDER_TOUCH(G) ((SHIFT_COL2(G.b,G.p2,-1) || SHIFT_COL2(G.b,G.p2,1) || SHIFT_COL2(G.b,G.p2,0)) && BOT_ROW(G.b,G.p2,G.pad_len)) // tocco bordo inf pad2.

#define PAD1_FULL_TOUCH(G) (SHIFT_COL(G.b,G.p1,1) && SAME_ROW(G.b,G.p1,G.pad_len)) // tocco pieno pad1.
#define PAD2_FULL_TOUCH(G) (SHIFT_COL2(G.b,G.p2,-1) && SAME_ROW(G.b,G.p2,G.pad_len)) // tocco pieno pad2.

#define PAD1_TOUCH(G) (PAD1_TOP_BORDER_TOUCH(G) || PAD1_BOT_BORDER_TOUCH(G) || PAD1_FULL_TOUCH(G)) // tocco (generale) pad1.
#define PAD2_TOUCH(G) (PAD2_TOP_BORDER_TOUCH(G) || PAD2_BOT_BORDER_TOUCH(G) || PAD2_FULL_TOUCH(G)) // tocco (generale) pad2.

#define PAD_TOP_BORDER_TOUCH(G) (PAD1_TOP_BORDER_TOUCH(G) || PAD2_TOP_BORDER_TOUCH(G)) // tocco bordo sup di un pad.
#define PAD_BOT_BORDER_TOUCH(G) (PAD1_BOT_BORDER_TOUCH(G) || PAD2_BOT_BORDER_TOUCH(G)) // tocco bordo inf di un pad.

#define BALL_TOP_BORDER_TOUCH(G) ((G).b.y == 0) // tocco palla bordo sup.
#define BALL_BOT_BORDER_TOUCH(G) ((G).b.y == (G).height) // tocco palla bordo inf.

#define BALL_LEFT_BORDER_TOUCH(G) ((G).b.x == 0) // tocco palla bordo sx.
#define BALL_RIGHT_BORDER_TOUCH(G) ((G).b.x == (G).width-1) // tocco palla bordo dx.
#define BALL_BORDER_TOUCH(G) (BALL_LEFT_BORDER_TOUCH(G) || BALL_RIGHT_BORDER_TOUCH(G)) // tocco palla bordo (generale).

// struttura pad
struct paddle {
	int x;
	int y;
};

// struttura palla
struct ball {
	int x;
	int y;
	int v; // vertical direction
	int h; // horizontal direction
};

// struttura gioco
struct game {
	unsigned int score1;
	unsigned int score2;
	int pad_len; // lunghezza pad
	int width;
	int height;
	int starting_ball_x;
	int starting_ball_y;
	struct ball b;
	struct paddle p1;
	struct paddle p2;
};

static struct game G;

// setup del gioco ad ogni partita.
void setup_game(int height, int width, struct position ball_pos, struct position ball_dir, struct position pad1_pos, struct position pad2_pos, int pad_len) {
	G.starting_ball_x = ball_pos.x;
	G.starting_ball_y = ball_pos.y;

	G.score1 = G.score2 = 0;
	G.width = width;
	G.height = height;
	G.pad_len = pad_len;

	G.p1.x = pad1_pos.x;
	G.p1.y = pad1_pos.y;
	G.p2.x = pad2_pos.x;
	G.p2.y = pad2_pos.y;
	G.b.x = ball_pos.x;
	G.b.y = ball_pos.y;
	G.b.h = ball_dir.x;
	G.b.v = ball_dir.y;
}

// movimento e rimbalzi palla
void move_ball() {
	if (BALL_BORDER_TOUCH(G)) {
		if (BALL_LEFT_BORDER_TOUCH(G)) G.score2++;
		else                           G.score1++;
		G.b.x = G.starting_ball_x;
		G.b.y = G.starting_ball_y;
	}
	else {
		// PAD1 TOUCH
		if (PAD1_TOUCH(G)) G.b.h = RIGHT;

		// PAD2 TOUCH
		if (PAD2_TOUCH(G)) G.b.h = LEFT;

		// PAD BORDER TOUCH
		if (PAD_BOT_BORDER_TOUCH(G)) G.b.v = DOWN;
		else if (PAD_TOP_BORDER_TOUCH(G)) G.b.v = UP;

		// TOP BORDER TOUCH
		if (BALL_TOP_BORDER_TOUCH(G)) G.b.v = DOWN;

		// BOTTOM BORDER TOUCH
		if (BALL_BOT_BORDER_TOUCH(G)) G.b.v = UP;

		// UPDATE POSITION
		G.b.x += G.b.h;
		G.b.y += G.b.v;
	}
}

// movimento pad1 up.
void move_pad1_up(void) {
	if (G.p1.y > 0 && !PAD1_TOP_BORDER_TOUCH(G)) G.p1.y--;
}

// movimento pad2 up.
void move_pad2_up(void) {
	if (G.p2.y > 0 && !PAD2_TOP_BORDER_TOUCH(G)) G.p2.y--;
}

// movimento pad1 down.
void move_pad1_down(void) {
	if ((G.p1.y + G.pad_len) < G.height - 1 && !PAD1_BOT_BORDER_TOUCH(G)) G.p1.y++;
}

// movimento pad2 down.
void move_pad2_down(void) {
	if ((G.p2.y + G.pad_len) < G.height - 1 && !PAD2_BOT_BORDER_TOUCH(G)) G.p2.y++;
}

// punteggio pad1.
unsigned int get_pad1_score(void) {
	return G.score1;
}

// punteggio pad2.
unsigned int get_pad2_score(void) {
	return G.score2;
}

// posizione palla.
struct position get_ball_pos(void) {
	struct position p;
	p.x = G.b.x;
	p.y = G.b.y;
	return p;
}

// posizione pad1.
struct position get_pad1_pos(void) {
	struct position p;
	p.x = G.p1.x;
	p.y = G.p1.y;
	return p;
}

// posizione pad1.
struct position get_pad2_pos(void) {
	struct position p;
	p.x = G.p2.x;
	p.y = G.p2.y;
	return p;
}

// lunghezza pad.
unsigned int get_pad_len(void) {
	return G.pad_len;
}