#ifndef _CHESS_H_
#define _CHESS_H_

#define WHITE 0
#define BLACK 1

#define INVALID '?'
#define PAWN 'p'
#define KNIGHT 'n'
#define BISHOP 'b'
#define ROOK 'r'
#define QUEEN 'q'
#define KING 'k'


typedef struct {
    int type, colour, column, row, moves, counter;
} ChessPiece;

typedef struct {
    ChessPiece pieces[32];
    ChessPiece *board[8 * 8];
    int moves;
} ChessGame;


ChessGame *chess_newgame();
void chess_free(ChessGame *game);
bool chess_test_move (ChessGame *game, ChessPiece *piece, int column, int row);
bool chess_turn (ChessGame *game, int sx, int sy, int dx, int dy);

#endif