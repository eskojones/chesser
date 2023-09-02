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

    game->pieces[ 0] = (ChessPiece){ PAWN, BLACK , 0, 1, 0, 0 };
    game->pieces[ 1] = (ChessPiece){ PAWN, BLACK , 1, 1, 0, 0 };
    game->pieces[ 2] = (ChessPiece){ PAWN, BLACK , 2, 1, 0, 0 };
    game->pieces[ 3] = (ChessPiece){ PAWN, BLACK , 3, 1, 0, 0 };
    game->pieces[ 4] = (ChessPiece){ PAWN, BLACK , 4, 1, 0, 0 };
    game->pieces[ 5] = (ChessPiece){ PAWN, BLACK , 5, 1, 0, 0 };
    game->pieces[ 6] = (ChessPiece){ PAWN, BLACK , 6, 1, 0, 0 };
    game->pieces[ 7] = (ChessPiece){ PAWN, BLACK , 7, 1, 0, 0 };
    game->pieces[ 8] = (ChessPiece){ PAWN, WHITE , 0, 6, 0, 0 };
    game->pieces[ 9] = (ChessPiece){ PAWN, WHITE , 1, 6, 0, 0 };
    game->pieces[10] = (ChessPiece){ PAWN, WHITE , 2, 6, 0, 0 };
    game->pieces[11] = (ChessPiece){ PAWN, WHITE , 3, 6, 0, 0 };
    game->pieces[12] = (ChessPiece){ PAWN, WHITE , 4, 6, 0, 0 };
    game->pieces[13] = (ChessPiece){ PAWN, WHITE , 5, 6, 0, 0 };
    game->pieces[14] = (ChessPiece){ PAWN, WHITE , 6, 6, 0, 0 };
    game->pieces[15] = (ChessPiece){ PAWN, WHITE , 7, 6, 0, 0 };
    game->pieces[16] = (ChessPiece){ ROOK, BLACK , 0, 0, 0, 0 };
    game->pieces[17] = (ChessPiece){ ROOK, WHITE , 0, 7, 0, 0 };
    game->pieces[18] = (ChessPiece){ KNIGHT, BLACK , 1, 0, 0, 0 };
    game->pieces[19] = (ChessPiece){ KNIGHT, WHITE , 1, 7, 0, 0 };
    game->pieces[20] = (ChessPiece){ BISHOP, BLACK , 2, 0, 0, 0 };
    game->pieces[21] = (ChessPiece){ BISHOP, WHITE , 2, 7, 0, 0 };
    game->pieces[22] = (ChessPiece){ KING, BLACK , 3, 0, 0, 0 };
    game->pieces[23] = (ChessPiece){ KING, WHITE , 3, 7, 0, 0 };
    game->pieces[24] = (ChessPiece){ QUEEN, BLACK , 4, 0, 0, 0 };
    game->pieces[25] = (ChessPiece){ QUEEN, WHITE , 4, 7, 0, 0 };
    game->pieces[26] = (ChessPiece){ BISHOP, BLACK , 5, 0 , 0, 0};
    game->pieces[27] = (ChessPiece){ BISHOP, WHITE , 5, 7, 0, 0 };
    game->pieces[28] = (ChessPiece){ KNIGHT, BLACK , 6, 0, 0, 0 };
    game->pieces[29] = (ChessPiece){ KNIGHT, WHITE , 6, 7, 0, 0 };
    game->pieces[30] = (ChessPiece){ ROOK, BLACK , 7, 0, 0, 0 };
    game->pieces[31] = (ChessPiece){ ROOK, WHITE , 7, 7, 0, 0 };

    for (int i = 0; i < 32; i++) {
        int x = game->pieces[i].column;
        int y = game->pieces[i].row;
        game->board[y * 8 + x] = &game->pieces[i];
    }
    return game;
}

void chess_free (ChessGame *game) {
    free(game);
}

int chess_test_position (
    ChessGame *game, 
    int piece_colour, int piece_type, bool first_move, 
    int piece_column, int piece_row, int column, int row
) {
    if (piece_type == INVALID) return 0;
    if (piece_column < 0 || piece_column > 7 || piece_row < 0 || piece_row > 7) return 0;
    if (column < 0 || column > 7 || row < 0 || row > 7) return 0;
    ChessPiece *target = game->board[row * 8 + column];
    //there's a piece at the destination
    bool target_exists = target != NULL;
    //the piece is a different colour
    bool target_enemy = target_exists && target->colour != piece_colour;
    //the piece is the same colour
    bool target_friend = target_exists && target->colour == piece_colour;

    //direction on column and row
    int xDir = column - piece_column;
    int yDir = row - piece_row;
    //distance on column and row
    int xDiff = abs(xDir);
    int yDiff = abs(yDir);
    //must be moving somewhere
    if (xDiff == 0 && yDiff == 0) return 0;

    //piece specific conditions
    if (piece_type == PAWN) {
        // if (piece_colour == WHITE) {
        //     if (yDir >= 0) return 0; //never move backwards
        //     if (yDir < -2) return 0; //never move more than 2
        //     //only allow moving 2 tiles when first move and not taking
        //     if (yDir == -2 && xDir == 0 && !first_move) return 0; 
        // }
        // if (piece_colour == BLACK) {
        //     if (yDir <= 0) return 0; //never move backwards
        //     if (yDir > 2) return 0; //never move more than 2
        //     //only allow moving 2 tiles when first move and not taking
        //     if (yDir == 2 && xDir == 0 && !first_move) return 0; 
        // }
        // //never move more than one column across
        // if (xDiff > 1) return 0;
        // //only move sideways when taking
        // if (target_friend && !target_enemy && xDiff != 0) return 0;
        // if (xDiff == 0 && yDiff == 1 && target_exists) return 0; 
        if (xDiff != 0) return false;
        if (yDiff > 2 && !first_move) return false;

    } else if (piece_type == KNIGHT) {
        int valids[][2] = {
            {  1, -2 },
            {  2, -1 },
            {  2,  1 },
            {  1,  2 },
            { -1,  2 },
            { -2,  1 },
            { -2, -1 },
            { -1, -2 }
        };
        bool valid = false;
        for (int i = 0; i < 8; i++) {
            if (valids[i][0] == xDir && valids[i][1] == yDir) {
                valid = true;
                break;
            }
        }
        if (!valid) return 0;

    } else if (piece_type == BISHOP) {
        //must be moving on both axis'
        if (xDiff == 0 || yDiff == 0) return 0;
        //must move diagonal
        if (xDiff != yDiff) return 0;

    } else if (piece_type == ROOK) {
        //cannot move diagonal
        if (xDiff != 0 && yDiff != 0) return 0;

    } else if (piece_type == QUEEN) {
        //allow anything except non-diagonal/cardinal moves
        if (xDiff != 0 && yDiff != 0) {
            if (xDiff != yDiff) return 0;
        }
    } else if (piece_type == KING) {
        //ez        
        if (xDiff > 1) return 0;
        if (yDiff > 1) return 0;
    }

    //there's a piece at our destination already
    if (target_exists) {
        //take the piece
        if (target_enemy) return 2;
        //target is friendly, cant move here
        else if (target_friend) return 0;
    }

    return 1;
}

float clamp (float v, float min, float max) {
    return v < min ? min : (v > max ? max : v);
}

bool chess_turn (ChessGame *game, int sx, int sy, int dx, int dy) {
    //destination tile is out of bounds
    if (sx < 0 || sx > 7 || sy < 0 || sy > 7) return false;
    if (dx < 0 || dx > 7 || dy < 0 || dy > 7) return false;
    int sidx = sy * 8 + sx;
    int didx = dy * 8 + dx;
    if (sidx == didx) return false;
    ChessPiece *p = game->board[sidx];
    //no piece at the source tile
    if (p == NULL) return false;
    if (p->type == INVALID) return false;

    //not this colour's turn
    if ((p->colour == WHITE && game->moves % 2 == 1)
     || (p->colour == BLACK && game->moves % 2 == 0)) {
        return false;
    }

    int result = 0;
    if (p->type == KNIGHT) {
        return false;
        result = chess_test_position(
            game, 
            p->colour, p->type, p->moves == 0, 
            p->column, p->row, 
            dx, dy
        );

    } else {
        int xDiff = abs(dx - p->column);
        int yDiff = abs(dy - p->row);
        int xDir = clamp(dx - p->column, -1, 1);
        int yDir = clamp(dy - p->row, -1, 1);
        if (p->type == KING) {
            if (xDiff > 1 || yDiff > 1) return false;
        }
        if (p->type == PAWN) {
            if (yDiff > 1 && p->moves > 0) return false;
        }
        int start_column = p->column;
        int start_row = p->row;
        int current_column = start_column;
        int current_row = start_row;
        while (current_column != dx || current_row != dy) {
            result = chess_test_position(
                game, 
                p->colour, p->type, p->moves == 0, 
                current_column, current_row, 
                current_column + xDir, current_row + yDir
            );
            current_column += xDir;
            current_row += yDir;
            if (result == 0) return false;
            //enemy piece is in the way
            if (result == 2) {
                if (current_column != dx || current_row != dy) return false;
            }
        }
    }

    if (result == 2) {
        //take the piece at dx,dy
        if (game->board[didx] == NULL) {
            printf("??\n");
            return false;
        }
        game->board[didx]->type = INVALID;
        game->board[didx]->column = -1;
        game->board[didx]->row = -1;
    }

    //move the piece
    p->column = dx;
    p->row = dy;
    game->board[sidx] = NULL;
    game->board[didx] = p;
    game->moves++;
    p->moves++;

    return true;
}