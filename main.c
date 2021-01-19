#include <stdio.h>
#include <stdlib.h>
#include "laser-chess.h"
#include "drawing.h"
#include <SDL.h>
#include <stdbool.h>

#define WIDTH 1024
#define HEIGHT 768

#define MAXPIECES 36

void renderStatus();
void render(SDL_Renderer*);

bool init();

void setupGameState();
void placePieces();

void mainLoop();
void shutdown();

// Event Handlers
void mouseMoveHandler(SDL_MouseMotionEvent*);
void mouseUpHandler(SDL_MouseButtonEvent*);

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

Board board;

Player players[2];
GameState gameState;
Piece pieces[MAXPIECES];

int main() {
    if(!init()) {
        exit(1);
    }

    board.tileXCount = 9;
    board.tileYCount = 9;

    board.x = 10;
    board.y = 10;
    board.width = 720; // 495 is divisible by 9
    board.height = 720;

    board.renderer = renderer;

    players[0].color = (SDL_Color){255,0,0, SDL_ALPHA_OPAQUE};
    players[1].color = (SDL_Color){0,0,255, SDL_ALPHA_OPAQUE};

    placePieces();
    setupGameState();

    mainLoop();

    shutdown();
    return 0;
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xcc, 0xcc, 0xcc, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    drawBoard(&board);
    drawBoardHighlights(&board, &gameState);

    for(int i = 0; i < MAXPIECES; i++) {
        drawPiece(&board, &pieces[i]);
    }

    if(gameState.isLaserOn) {
        drawLaserBeam(&board, &gameState, renderer);
    }

    renderStatus();
    SDL_RenderPresent(renderer);
}

void renderStatus() {
    size_t sizeNeeded = snprintf(NULL, 0, "%s's Turn, Moves Left: %d", gameState.players[gameState.activePlayer].name, gameState.movesLeft)+1;
    char* playerStatus = malloc(sizeNeeded);
    sprintf(playerStatus, "%s's Turn, Moves Left: %d", gameState.players[gameState.activePlayer].name, gameState.movesLeft);
    stringRGBA(renderer, 750, 20, playerStatus, 0, 0, 0, SDL_ALPHA_OPAQUE);
    free(playerStatus);

    if(gameState.isPieceSelected) {
        sizeNeeded = snprintf(NULL, 0, "piece selected at: %d, %d", (int)trunc(gameState.selectedPiece->location.x), (int)trunc(gameState.selectedPiece->location.y)) + 1;
        char* textLine = malloc(sizeNeeded);
        sprintf(textLine, "piece selected at: %d, %d", (int)trunc(gameState.selectedPiece->location.x), (int)trunc(gameState.selectedPiece->location.y));
        stringRGBA(renderer, 750, 40, textLine, 0,0,0,SDL_ALPHA_OPAQUE);
        free(textLine);
    }
}

void mainLoop() {
    bool hasRendered = false;
    SDL_bool done = SDL_FALSE;

    SDL_Event event;
    while (!done) {

        render(renderer);
        hasRendered = true;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }

            if(event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case '`':
                        printf("player is skipping turn\n");
                        endTurn(&gameState);
                        break;
                    case 'r':
                        printf("rotating piece");
                        rotatePiece(&gameState);
                        break;
                    case SDLK_RETURN:
                        printf("return key pressed\n");
                        if(gameState.isPieceRotating) {
                            commitPieceRotation(&gameState);
                        }
                        break;
                    case SDLK_SPACE:
                        if(!gameState.isLaserOn) {
                        printf("space bar, let's fire a laser\n");
                        if(gameState.isPieceSelected && gameState.selectedPiece->type == LASER) {
                            fireLaser(&gameState, &board);
                        }
                        } else {
                            turnLaserOff(&gameState);
                        }
                        break;
                }
//                printf("key press is: %c\n", event.key.keysym.sym);
            }

            if(event.type == SDL_MOUSEMOTION) {
                mouseMoveHandler(&event.motion);
            }

            if(event.type == SDL_MOUSEBUTTONUP) {
                mouseUpHandler(&event.button);
            }
        }
    }

}

void placePieces() {
    Piece* p_ptr = pieces;

    *p_ptr++ = (Piece) {&players[0], {0, 0}, 90, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[0], {1, 0}, 90, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[0], {2, 0}, 0, DIAGONAL, true};
    *p_ptr++ = (Piece) {&players[0], {3, 0}, 180, LASER, true};
    *p_ptr++ = (Piece) {&players[0], {4, 0}, 0, KING, true};
    *p_ptr++ = (Piece) {&players[0], {5, 0}, 0, HYPERCUBE, true};
    *p_ptr++ = (Piece) {&players[0], {6, 0}, 90, DIAGONAL, true};
    *p_ptr++ = (Piece) {&players[0], {7, 0}, 180, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[0], {8, 0}, 180, TRIANGLE, true};

    *p_ptr++ = (Piece) {&players[0], {0, 1}, 180, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[0], {1, 1}, 180, BLOCK, true};
    *p_ptr++ = (Piece) {&players[0], {2, 1}, 180, BLOCK, true};
    *p_ptr++ = (Piece) {&players[0], {3, 1}, 0, SPLITTER, true};
    *p_ptr++ = (Piece) {&players[0], {4, 1}, 90, STRAIGHT, true};
    *p_ptr++ = (Piece) {&players[0], {5, 1}, 0, STRAIGHT, true};
    *p_ptr++ = (Piece) {&players[0], {6, 1}, 180, BLOCK, true};
    *p_ptr++ = (Piece) {&players[0], {7, 1}, 180, BLOCK, true};
    *p_ptr++ = (Piece) {&players[0], {8, 1}, 90, TRIANGLE, true};

    *p_ptr++ = (Piece) {&players[1], {0, 8}, 0, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[1], {1, 8}, 0, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[1], {2, 8}, 90, DIAGONAL, true};
    *p_ptr++ = (Piece) {&players[1], {3, 8}, 0, HYPERCUBE, true};
    *p_ptr++ = (Piece) {&players[1], {4, 8}, 0, KING, true};
    *p_ptr++ = (Piece) {&players[1], {5, 8}, 0, LASER, true};
    *p_ptr++ = (Piece) {&players[1], {6, 8}, 0, DIAGONAL, true};
    *p_ptr++ = (Piece) {&players[1], {7, 8}, -90, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[1], {8, 8}, -90, TRIANGLE, true};

    *p_ptr++ = (Piece) {&players[1], {0, 7}, -90, TRIANGLE, true};
    *p_ptr++ = (Piece) {&players[1], {1, 7}, 0, BLOCK, true};
    *p_ptr++ = (Piece) {&players[1], {2, 7}, 0, BLOCK, true};
    *p_ptr++ = (Piece) {&players[1], {3, 7}, 0, STRAIGHT, true};
    *p_ptr++ = (Piece) {&players[1], {4, 7}, 90, STRAIGHT, true};
    *p_ptr++ = (Piece) {&players[1], {5, 7}, 180, SPLITTER, true};
    *p_ptr++ = (Piece) {&players[1], {6, 7}, 0, BLOCK, true};
    *p_ptr++ = (Piece) {&players[1], {7, 7}, 0, BLOCK, true};
    *p_ptr++ = (Piece) {&players[1], {8, 7}, 0, TRIANGLE, true};
}

void setupGameState() {
    gameState.players = players;
    gameState.movesLeft = 2;
    gameState.activePlayer = 0;

    gameState.players[0].name = "Red";
    gameState.players[1].name = "Blue";

    gameState.pieces = pieces;
    gameState.pieceCount = MAXPIECES;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer) < 0) {
        return false;
    }
    return true;
}

void shutdown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

// Events
void mouseMoveHandler(SDL_MouseMotionEvent* evt) {
    Vector2 tile;
    if(getTileUnder(&board,(Vector2){evt->x, evt->y}, &tile)) {
        gameState.isTileHovered = true;
        gameState.hoveredTile = tile;
    } else {
        gameState.isTileHovered = false;
    }
}

void mouseUpHandler(SDL_MouseButtonEvent* evt) {
    Vector2 tile;
    Piece* piece;
    if(getTileUnder(&board, (Vector2){evt->x, evt->y}, &tile)) {
        if(getPieceOnTile(&gameState, tile, &piece)) {
            if(gameState.isPieceSelected) {
                if(gameState.selectedPiece == piece) {
                    cancelPieceRotation(&gameState);
                    gameState.isPieceSelected = false;
                } else { // We have a piece selected and we are clicking on a tile with another piece
                    Move possibleMoves[4];
                    int moveCount = getValidMoves(&gameState, gameState.selectedPiece, possibleMoves);
                    for(int i = 0; i < moveCount; i++) {
                        if(tile.x == possibleMoves[i].endPos.x && tile.y == possibleMoves[i].endPos.y) {
                            piece->isActive = false;
                            gameState.selectedPiece->location = (Vector2){tile.x, tile.y};
                            gameState.isPieceSelected = false;
                            endMove(&gameState);
                        }
                    }
                }
            } else {
                if(piece->player == &gameState.players[gameState.activePlayer]) {
                    gameState.selectedPiece = piece;
                    gameState.isPieceSelected = true;

                    Move possibleMoves[4];
                    int moveCount = getValidMoves(&gameState, piece, possibleMoves);
                    printf("Valid number of moves: %d\n", moveCount);
                    for(int i = 0; i < moveCount; i++) {
                        printf("Valid Move: x:%d y:%d\n", (int)possibleMoves[i].endPos.x, (int)possibleMoves[i].endPos.y);
                    }
                }
            }
        } else { // Clicked on an empty tile
            printf("clicking on empty tile (%d, %d)\n", (int)tile.x, (int)tile.y);
            if(gameState.isPieceSelected) {
                Move possibleMoves[4];
                int moveCount = getValidMoves(&gameState, gameState.selectedPiece, possibleMoves);
                for(int i = 0; i < moveCount; i++) {
                    if(tile.x == possibleMoves[i].endPos.x && tile.y == possibleMoves[i].endPos.y) {
                        gameState.selectedPiece->location = (Vector2){tile.x, tile.y};
//                        gameState.isPieceSelected = false;
                        endMove(&gameState);
                    }
                }
            }
        }
    }
}
