#include <stdio.h>
#include "player.h"

struct ChessPlayer *player_create (ChessGame *game, int colour) {
    struct ChessPlayer *pl = (struct ChessPlayer *)malloc(sizeof(struct ChessPlayer));
    pl->colour = colour;
    for (int i = 0, j = 0; i < 32; i++) {
        if (game->pieces[i].colour != colour) continue;
        pl->pieces[j++] = &game->pieces[i];
    }
    return pl;
}


ChessMove *player_turn_random (ChessGame *game, struct ChessPlayer *player) {
    ChessPiece *p = NULL;
    while (p == NULL) {
        p = player->pieces[rand() % 16];
        if (p->status == TAKEN) p = NULL;
    }
    ChessMove *move = (ChessMove *)malloc(sizeof(ChessMove));
    move->piece = p;
    move->action = 0;
    move->column = rand() % 8;
    move->row = rand() % 8;
    //printf("%s %d %d\n", chess_piece_names[move->piece->type], move->column, move->row);
    return move;
}