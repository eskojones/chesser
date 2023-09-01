#ifndef _CHESS_H_
#define _CHESS_H_

#define WHITE 0
#define BLACK 1

#define INVALID 0
#define PAWN 'p'
#define KNIGHT 'n'
#define BISHOP 'b'
#define ROOK 'r'
#define QUEEN 'q'
#define KING 'k'


typedef struct {
    int type, colour, column, row;
} ChessPiece;

typedef struct {
    ChessPiece pieces[32];
    ChessPiece *board[8 * 8];
} ChessGame;


ChessGame *chess_newgame();
void chess_free(ChessGame *game);
#endif