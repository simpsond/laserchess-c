#include <stdio.h>
#include <stdlib.h>
#include "laser-chess.h"
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

    drawBoard(&board, &gameState);

    for(int i = 0; i < MAXPIECES; i++) {
        drawPiece(&board, &pieces[i]);
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

                }
                printf("key press is: %c\n", event.key.keysym.sym);
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

    *p_ptr++ = (Piece) {&players[0], {0, 0}, 90, TRIANGLE};
    *p_ptr++ = (Piece) {&players[0], {1, 0}, 90, TRIANGLE};
    *p_ptr++ = (Piece) {&players[0], {2, 0}, 0, DIAGONAL};
    *p_ptr++ = (Piece) {&players[0], {3, 0}, 180, LASER};
    *p_ptr++ = (Piece) {&players[0], {4, 0}, 0, KING};
    *p_ptr++ = (Piece) {&players[0], {5, 0}, 0, HYPERCUBE};
    *p_ptr++ = (Piece) {&players[0], {6, 0}, 90, DIAGONAL};
    *p_ptr++ = (Piece) {&players[0], {7, 0}, 180, TRIANGLE};
    *p_ptr++ = (Piece) {&players[0], {8, 0}, 180, TRIANGLE};

    *p_ptr++ = (Piece) {&players[0], {0, 1}, 180, TRIANGLE};
    *p_ptr++ = (Piece) {&players[0], {1, 1}, 180, BLOCK};
    *p_ptr++ = (Piece) {&players[0], {2, 1}, 180, BLOCK};
    *p_ptr++ = (Piece) {&players[0], {3, 1}, 0, SPLITTER};
    *p_ptr++ = (Piece) {&players[0], {4, 1}, 90, STRAIGHT};
    *p_ptr++ = (Piece) {&players[0], {5, 1}, 0, STRAIGHT};
    *p_ptr++ = (Piece) {&players[0], {6, 1}, 180, BLOCK};
    *p_ptr++ = (Piece) {&players[0], {7, 1}, 180, BLOCK};
    *p_ptr++ = (Piece) {&players[0], {8, 1}, 90, TRIANGLE};

    *p_ptr++ = (Piece) {&players[1], {0, 8}, 0, TRIANGLE};
    *p_ptr++ = (Piece) {&players[1], {1, 8}, 0, TRIANGLE};
    *p_ptr++ = (Piece) {&players[1], {2, 8}, 90, DIAGONAL};
    *p_ptr++ = (Piece) {&players[1], {3, 8}, 0, HYPERCUBE};
    *p_ptr++ = (Piece) {&players[1], {4, 8}, 0, KING};
    *p_ptr++ = (Piece) {&players[1], {5, 8}, 0, LASER};
    *p_ptr++ = (Piece) {&players[1], {6, 8}, 0, DIAGONAL};
    *p_ptr++ = (Piece) {&players[1], {7, 8}, -90, TRIANGLE};
    *p_ptr++ = (Piece) {&players[1], {8, 8}, -90, TRIANGLE};

    *p_ptr++ = (Piece) {&players[1], {0, 7}, -90, TRIANGLE};
    *p_ptr++ = (Piece) {&players[1], {1, 7}, 0, BLOCK};
    *p_ptr++ = (Piece) {&players[1], {2, 7}, 0, BLOCK};
    *p_ptr++ = (Piece) {&players[1], {3, 7}, 0, STRAIGHT};
    *p_ptr++ = (Piece) {&players[1], {4, 7}, 90, STRAIGHT};
    *p_ptr++ = (Piece) {&players[1], {5, 7}, 180, SPLITTER};
    *p_ptr++ = (Piece) {&players[1], {6, 7}, 0, BLOCK};
    *p_ptr++ = (Piece) {&players[1], {7, 7}, 0, BLOCK};
    *p_ptr++ = (Piece) {&players[1], {8, 7}, 0, TRIANGLE};
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
        if(getPieceOnTile(&gameState, tile, piece)) {
            if(gameState.isPieceSelected) {
                if(gameState.selectedPiece == piece) {
                    gameState.isPieceSelected = false;
                }
            } else {
                gameState.selectedPiece = piece;
                gameState.isPieceSelected = true;
            }
        }
    }
}
