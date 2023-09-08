#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "chess.h"


struct ChessPlayer *player_create (ChessGame *game, int colour);
ChessMove *player_turn_random (ChessGame *game, struct ChessPlayer *player);

#endif
