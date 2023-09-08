#ifndef _CHESS_H_
#define _CHESS_H_

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"



#define clamp(v,min,max) (v < min ? min : (v > max ? max : v))

#define WHITE 0
#define BLACK 1

#define NORMAL 0
#define TAKEN 1

#define INVALID 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

extern const char chess_piece_names[7][8];  //Pawn, Knight, Bishop, etc etc...
extern const char chess_piece_chars[8];     //pnbrqk
extern const char chess_board_columns[8];   //abcdefgh
extern const char chess_board_rows[8];      //12345678


#define CH_MOVE_INVALID      (-1)
#define CH_MOVE_BLOCKED        0
#define CH_MOVE_SUCCESS      200
#define CH_MOVE_TAKE         201
#define CH_MOVE_PROMOTE      202

struct ChessPlayer;

typedef struct {
    int type, colour, column, row, moves, counter, status;
} ChessPiece;

typedef struct {
    struct ChessPlayer *players[2];
    ChessPiece pieces[32];
    ChessPiece *board[8 * 8];
    int moves;
    LLNode *log;
} ChessGame;

typedef struct {
    int code;
    ChessPiece *piece;
    int piece_colour, piece_type, piece_column, piece_row;
    int target_colour, target_type, target_column, target_row;
    bool took, first;
} ChessTurnResult;

typedef struct {
    ChessPiece *piece;
    int action;
    int column, row;
} ChessMove;


struct ChessPlayer {
    int colour;
    ChessPiece *pieces[16];
    ChessMove *(*turn)(ChessGame *game, struct ChessPlayer *player);
};

ChessGame *chess_newgame();
void chess_free(ChessGame *game);
ChessTurnResult *chess_turn (ChessGame *game, int sx, int sy, int dx, int dy);
bool chess_do_turn (ChessGame *game);

#endif