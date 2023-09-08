#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "chess.h"


const char chess_piece_names[7][8] = {
    "Invalid",
    "Pawn",
    "Knight",
    "Bishop",
    "Rook",
    "Queen",
    "King"
};
const char chess_piece_chars[8] = "?PNBRQK";
const char chess_board_columns[8] = "abcdefgh";
const char chess_board_rows[8] = "87654321";

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


void chess_log_free (void *data, void *index, void *meta) {
    free((ChessTurnResult *)data);
}

void chess_free (ChessGame *game) {
    free(game->players[0]);
    free(game->players[1]);
    ll_foreach(game->log, chess_log_free, NULL);
    ll_free(game->log);
    free(game);
}


int chess_test_position (
    ChessGame *game, 
    int piece_colour, int piece_type, bool first_move, 
    int piece_column, int piece_row, int column, int row
) {
    if (piece_column < 0 || piece_column > 7 || piece_row < 0 || piece_row > 7) return CH_MOVE_INVALID;
    if (column < 0 || column > 7 || row < 0 || row > 7) return CH_MOVE_INVALID;
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
    if (xDiff == 0 && yDiff == 0) return CH_MOVE_INVALID;

    //piece specific conditions
    if (piece_type == PAWN) {
        if (piece_colour == WHITE && yDir >= 0) return CH_MOVE_INVALID;
        if (piece_colour == BLACK && yDir <= 0) return CH_MOVE_INVALID;
        if (first_move && xDiff != 0) return CH_MOVE_INVALID;
        if (first_move && yDiff > 2) return CH_MOVE_INVALID;
        if (!first_move && yDiff > 1) return CH_MOVE_INVALID;
        if (xDiff == 0 && target_exists) return CH_MOVE_INVALID;
        if (xDiff != 0 && !target_exists) return CH_MOVE_INVALID;

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
        if (!valid) return CH_MOVE_INVALID;

    } else if (piece_type == BISHOP) {
        //must be moving on both axis'
        if (xDiff == 0 || yDiff == 0) return CH_MOVE_INVALID;
        //must move diagonal
        if (xDiff != yDiff) return CH_MOVE_INVALID;

    } else if (piece_type == ROOK) {
        //cannot move diagonal
        if (xDiff != 0 && yDiff != 0) return CH_MOVE_INVALID;

    } else if (piece_type == QUEEN) {
        //allow anything except non-diagonal/cardinal moves
        if (xDiff != 0 && yDiff != 0) {
            if (xDiff != yDiff) return CH_MOVE_INVALID;
        }
    } else if (piece_type == KING) {
        //ez        
        if (xDiff > 1) return CH_MOVE_INVALID;
        if (yDiff > 1) return CH_MOVE_INVALID;
    }

    //there's a piece at our destination already
    if (target_exists) {
        //take the piece
        if (target_enemy) {
            return CH_MOVE_TAKE;
        }
        //target is friendly, cant move here
        else if (target_friend) return CH_MOVE_BLOCKED;
    }

    //piece could move to (col,row)
    return CH_MOVE_SUCCESS;
}


void chess_do_move (ChessGame *game, ChessPiece *p, int column, int row) {
    int idx = p->row * 8 + p->column;
    game->board[idx] = NULL;
    p->column = column;
    p->row = row;
    idx = p->row * 8 + p->column;
    game->board[idx] = p;
    p->moves++;
}


bool chess_do_take (ChessGame *game, ChessPiece *p, int column, int row) {
    int idx = row * 8 + column;
    ChessPiece *target = game->board[idx];
    if (target == NULL) return false;
    target->status = TAKEN;
    target->column = -1;
    target->row = -1;
    return true;
}


bool chess_test_pawn_promote (ChessTurnResult *result) {
    return result->piece_type == PAWN
        && (result->code == CH_MOVE_SUCCESS || result->code == CH_MOVE_TAKE)
        && (result->target_row == 0 || result->target_row == 7);
}


ChessTurnResult *chess_result_create (ChessPiece *p, int column, int row) {
    ChessTurnResult *result = (ChessTurnResult *)malloc(sizeof(ChessTurnResult));
    memset(result, 0, sizeof(ChessTurnResult));
    result->piece_colour = p->colour;
    result->piece_type = p->type;
    result->piece_column = p->column;
    result->piece_row = p->row;
    result->target_column = column;
    result->target_row = row;
    return result;
}

int chess_test_turn (ChessGame *game) {
    return game->moves % 2 == 1 ? WHITE : BLACK;
}

//float clamp (float v, float min, float max) {
//    return v < min ? min : (v > max ? max : v);
//}

ChessTurnResult *chess_turn (ChessGame *game, int sx, int sy, int dx, int dy) {
    //destination tile is out of bounds
    if (sx < 0 || sx > 7 || sy < 0 || sy > 7) return NULL;
    if (dx < 0 || dx > 7 || dy < 0 || dy > 7) return NULL;
    int sidx = sy * 8 + sx;
    int didx = dy * 8 + dx;
    if (sidx == didx) return NULL;
    ChessPiece *p = game->board[sidx];
    //no piece at the source tile
    if (p == NULL) return NULL;
    if (p->status == TAKEN) return NULL;

    //not this colour's turn
    if (chess_test_turn(game) != p->colour) {
        return NULL;
    }

    ChessTurnResult *result = chess_result_create(p, dx, dy);

    if (p->type == KNIGHT) {
        result->code = chess_test_position(
            game, 
            p->colour, p->type, p->moves == 0, 
            p->column, p->row, 
            dx, dy
        );
        if (result->code < CH_MOVE_SUCCESS) {
            free(result);
            return NULL;
        }

    } else {
        int xDir = (int)clamp((float)dx - (float)p->column, -1, 1);
        int yDir = (int)clamp((float)dy - (float)p->row, -1, 1);
        int start_column = p->column;
        int start_row = p->row;
        int current_column = start_column;
        int current_row = start_row;
        while (current_column != dx || current_row != dy) {
            result->code = chess_test_position(
                game, 
                p->colour, p->type, p->moves == 0, 
                start_column, start_row, 
                current_column + xDir, current_row + yDir
            );
            current_column += xDir;
            current_row += yDir;
            //enemy piece is in the way
            if (result->code == CH_MOVE_BLOCKED || result->code == CH_MOVE_TAKE) {
                if (current_column != dx || current_row != dy) {
                    free(result);
                    return NULL;
                }
            }
            if (result->code < CH_MOVE_SUCCESS) {
                free(result);
                return NULL;
            }
        }
    }

    if (result->code >= CH_MOVE_TAKE) {
        //take the piece at dx,dy
        if (game->board[didx] == NULL) {
            //printf("... but there's no piece to take ???\n");
            free(result);
            return NULL;
        }
        result->target_type = game->board[didx]->type;
        result->target_colour = game->board[didx]->colour;
        result->took = true;
        chess_do_take(game, p, dx, dy);
    }

    if (chess_test_pawn_promote(result)) result->code = CH_MOVE_PROMOTE;
    result->first = p->moves == 0;
    chess_do_move(game, p, dx, dy);
    game->moves++;
    result->piece = p;

    return result;
}

bool chess_do_turn (ChessGame *game) {
    int idx = WHITE;
    if (chess_test_turn(game) == BLACK) idx = BLACK;
    ChessMove *move = game->players[idx]->turn(game, game->players[idx]);
    ChessTurnResult *result = chess_turn(game, move->piece->column, move->piece->row, move->column, move->row);
    if (result != NULL) {
        if (game->log == NULL) {
            game->log = ll_create(result);
        } else {
            ll_append(game->log, result);
        }
    }
    free(move);
    return true;
}