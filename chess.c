#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "chess.h"
#include "list.h"


ChessGame *chess_newgame () {
    ChessGame *game = (ChessGame *)malloc(sizeof(ChessGame));
    memset(game, 0, sizeof(ChessGame));

    for (int i = 0; i < 8; i++) {
        game->pieces[i].colour = BLACK;
        game->pieces[i].row = 1;
        game->pieces[i].column = i;
        game->pieces[i].type = PAWN;
        int board_idx = game->pieces[i].row * 8 + game->pieces[i].column;
        game->board[board_idx] = &game->pieces[i];
    }
    for (int i = 8; i < 16; i++) {
        game->pieces[i].colour = WHITE;
        game->pieces[i].row = 6;
        game->pieces[i].column = i-8;
        game->pieces[i].type = PAWN;
        int board_idx = game->pieces[i].row * 8 + game->pieces[i].column;
        game->board[board_idx] = &game->pieces[i];
    }

    game->pieces[16] = (ChessPiece){ ROOK, BLACK , 0, 0 };
    game->pieces[17] = (ChessPiece){ ROOK, WHITE , 0, 7 };
    game->pieces[18] = (ChessPiece){ KNIGHT, BLACK , 1, 0 };
    game->pieces[19] = (ChessPiece){ KNIGHT, WHITE , 1, 7 };
    game->pieces[20] = (ChessPiece){ BISHOP, BLACK , 2, 0 };
    game->pieces[21] = (ChessPiece){ BISHOP, WHITE , 2, 7 };
    game->pieces[22] = (ChessPiece){ KING, BLACK , 3, 0 };
    game->pieces[23] = (ChessPiece){ KING, WHITE , 3, 7 };
    game->pieces[24] = (ChessPiece){ QUEEN, BLACK , 4, 0 };
    game->pieces[25] = (ChessPiece){ QUEEN, WHITE , 4, 7 };
    game->pieces[26] = (ChessPiece){ BISHOP, BLACK , 5, 0 };
    game->pieces[27] = (ChessPiece){ BISHOP, WHITE , 5, 7 };
    game->pieces[28] = (ChessPiece){ KNIGHT, BLACK , 6, 0 };
    game->pieces[29] = (ChessPiece){ KNIGHT, WHITE , 6, 7 };
    game->pieces[30] = (ChessPiece){ ROOK, BLACK , 7, 0 };
    game->pieces[31] = (ChessPiece){ ROOK, WHITE , 7, 7 };

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

//tests if one position is valid
bool chess_test_position_pawn (ChessGame *game, ChessPiece *piece, int column, int row) {
    int col_diff = abs(column - piece->column);
    int row_diff = row - piece->row;
    //cant move backwards or sideways or more than two tiles
    if (piece->colour == WHITE) {
        if (piece->moves == 0 && row_diff > 2) return false;
        if (row_diff <= 0 || row_diff > 1) return false;
    } else if (piece->colour == BLACK) {
        if (piece->moves == 0 && row_diff < -2) return false;
        if (row_diff >= 0 || row_diff < -1) return false;
    }

    //cant move more than one tile left/right
    if (col_diff > 1) return false;
    
    //only allow diagonal move if they are taking a peice.. of a diff colour..
    int idx = row * 8 + column;
    if (col_diff == 1 && (game->board[idx] == NULL || game->board[idx]->colour == piece->colour)) return false;
    return true;
}

//tests if all moves toward destination are valid
bool chess_test_pawn (ChessGame *game, ChessPiece *piece, int column, int row) {
    int col_diff = column - piece->column;
    int col_diff_abs = abs(col_diff);
    int row_diff = row - piece->row;
    int row_diff_abs = abs(row_diff);
    if (col_diff_abs != 0) return false;
    if (row_diff_abs != 1) {
        return false;
    }
    return true;
}

bool chess_test_move (ChessGame *game, ChessPiece *piece, int column, int row) {
    if (game == NULL || piece == NULL || piece->type == INVALID) return false;
    if (row < 0 || row > 7 || column < 0 || column > 7) return false;
    switch(piece->type) {
        case PAWN:
            return chess_test_pawn(game, piece, column, row);
    }

    return false;
}

bool chess_turn (ChessGame *game, int sx, int sy, int dx, int dy) {
    if (dx < 0 || dx > 7 || dy < 0 || dy > 7) return false;
    ChessPiece * p = game->board[sy * 8 + sx];
    if (p == NULL) return false;

    if ((p->colour == WHITE && game->moves % 2 == 1)
     || (p->colour == BLACK && game->moves % 2 == 0)) {
        return false;
    }

    ChessPiece *taken = game->board[dy * 8 + dx];
    bool taken_enemy = taken != NULL && taken->type != INVALID && taken->colour != p->colour;
    bool taken_friend = taken != NULL && taken->type != INVALID && taken->colour == p->colour;

    //insert rules here
    if (p->type == PAWN) {
        int yDir = dy - p->row;
        if (p->colour == WHITE && yDir >= 0) return false; 
        else {
            if (yDir < -2) return false;
            if (p->moves > 0 && yDir == -2) return false;
        }

        if (p->colour == BLACK && yDir <= 0) return false;
        else {
            if (yDir > 2) return false;
            if (p->moves > 0 && yDir == 2) return false;
        }

        //only move sideways when taking
        int xDir = dx - p->column;
        if (!taken_enemy && xDir != 0) return false;
        if ((taken_enemy || taken_friend) && xDir == 0) return false;

    } else if (p->type == KNIGHT) {
        return false;
    } else if (p->type == BISHOP) {
        return false;
    } else if (p->type == ROOK) {
        return false;
    } else if (p->type == QUEEN) {
    } else if (p->type == KING) {
        //ez        
        int xDiff = abs(dx - p->column);
        if (xDiff > 1) return false;
        int yDiff = abs(dy - p->row);
        if (yDiff > 1) return false;
    }

    if (taken_enemy) {
        // printf("%s %c took %s %c\n", 
        //     p->colour ? "black" : "white", 
        //     p->type,
        //     taken->colour ? "black" : "white",
        //     taken->type
        // );
        taken->type = INVALID;
    } else if (taken_friend) return false;

    p->column = dx;
    p->row = dy;
    game->board[sy * 8 + sx] = NULL;
    game->board[dy * 8 + dx] = p; //here
    game->moves++;
    p->moves++;

    return true;
}