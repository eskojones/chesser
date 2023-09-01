#include <stdio.h>
#include <string.h>

#include "chess.h"


int main (int argc, char **argv) {
    ChessGame *game = chess_newgame();

    for (int i = 0; i < 64; i++) {
        if (game->board[i] == NULL) printf(".");
        else {
            printf("%c", (game->board[i]->colour == WHITE ? game->board[i]->type - 32 : game->board[i]->type));
        }
        if (i % 8 == 7) printf("\n");
    }
    chess_free(game);
    return 0;
}
