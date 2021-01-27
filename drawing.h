//
// Created by Dustin Simpson on 1/17/21.
//

#ifndef LASER_CHESS_C_DRAWING_H
#define LASER_CHESS_C_DRAWING_H
#include "GameState.h"
#include "Piece.h"
#include "Player.h"
#include "laser-chess.h"

void drawBoard(Board* b);
void drawBoardHighlights(Board* b, GameState* gs);
void drawLaserBeam(Board* b, GameState* gs, SDL_Renderer* renderer);
void drawPiece(Board* b, Piece* piece);
void drawTriangle(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawDiagonal(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawBlock(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawStraight(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawSplitter(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawHypercube(Vector2 p1, Vector2 p2, struct SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawLaser(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawKing(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);

#endif //LASER_CHESS_C_DRAWING_H
