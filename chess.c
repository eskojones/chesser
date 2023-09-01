#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chess.h"


ChessGame *chess_newgame () {
    ChessGame *game = (ChessGame *)malloc(sizeof(ChessGame));
    memset(game, 0, sizeof(ChessGame));

    for (int i = 0; i < 16; i++) {
        int imod2 = i % 2;
        game->pieces[i].colour = imod2 == 0 ? WHITE : BLACK;
        game->pieces[i].row = imod2 == 0 ? 1 : 6;
        game->pieces[i].column = floor((float)i / 2.0f);
        game->pieces[i].type = PAWN;
        int board_idx = game->pieces[i].row * 8 + game->pieces[i].column;
        game->board[board_idx] = &game->pieces[i];
    }

    game->pieces[16] = (ChessPiece){ ROOK, WHITE, 0, 0 };
    game->pieces[17] = (ChessPiece){ ROOK, BLACK, 0, 7 };
    game->pieces[18] = (ChessPiece){ KNIGHT, WHITE, 1, 0 };
    game->pieces[19] = (ChessPiece){ KNIGHT, BLACK, 1, 7 };
    game->pieces[20] = (ChessPiece){ BISHOP, WHITE, 2, 0 };
    game->pieces[21] = (ChessPiece){ BISHOP, BLACK, 2, 7 };
    game->pieces[22] = (ChessPiece){ KING, WHITE, 3, 0 };
    game->pieces[23] = (ChessPiece){ KING, BLACK, 3, 7 };
    game->pieces[24] = (ChessPiece){ QUEEN, WHITE, 4, 0 };
    game->pieces[25] = (ChessPiece){ QUEEN, BLACK, 4, 7 };
    game->pieces[26] = (ChessPiece){ BISHOP, WHITE, 5, 0 };
    game->pieces[27] = (ChessPiece){ BISHOP, BLACK, 5, 7 };
    game->pieces[28] = (ChessPiece){ KNIGHT, WHITE, 6, 0 };
    game->pieces[29] = (ChessPiece){ KNIGHT, BLACK, 6, 7 };
    game->pieces[30] = (ChessPiece){ ROOK, WHITE, 7, 0 };
    game->pieces[31] = (ChessPiece){ ROOK, BLACK, 7, 7 };

    for (int i = 16; i < 32; i++) {
        int x = game->pieces[i].column;
        int y = game->pieces[i].row;
        game->board[y * 8 + x] = &game->pieces[i];
    }
    return game;
}

void chess_free (ChessGame *game) {
    free(game);
}