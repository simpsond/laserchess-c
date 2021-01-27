//
// Created by Dustin Simpson on 1/17/21.
//
#include "drawing.h"

void drawLaserBeam(Board* b, GameState* gs, SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    Vector2 p1, p2;
    Piece* tmpPiece;

    Beam* cur = gs->beam;
    while(cur) {
        printf("laser had direction at (%d, %d)\n", (int)cur->tile.x, (int)cur->tile.y);
        getBoxFromTile(b,cur->tile, &p1, &p2);
        getBoxRefPoints(p1, p2,0,refPoints,9);

        if(getPieceOnTile(gs,cur->tile, &tmpPiece)) {
            switch (tmpPiece->type) {
                case SPLITTER:
                    thickLineRGBA(renderer, round(refPoints[1].x), round(refPoints[1].y), round(refPoints[8].x),
                              round(refPoints[8].y), 3, 255, 0, 255, SDL_ALPHA_OPAQUE);
                    thickLineRGBA(renderer, round(refPoints[8].x), round(refPoints[8].y), round(refPoints[3].x),
                                  round(refPoints[3].y), 3, 255, 0, 255, SDL_ALPHA_OPAQUE);
                    break;
            }
        } else {
            if(cur->direction.x != 0) {
                thickLineRGBA(renderer, round(refPoints[7].x), round(refPoints[7].y), round(refPoints[3].x),
                              round(refPoints[3].y), 3, 255, 0, 255, SDL_ALPHA_OPAQUE);
            } else {
                thickLineRGBA(renderer, round(refPoints[1].x), round(refPoints[1].y), round(refPoints[5].x),
                              round(refPoints[5].y), 3, 255, 0, 255, SDL_ALPHA_OPAQUE);
            }
        }


        if(cur->nextCount > 0) {
            cur = cur->next[0];
        } else {
            cur = NULL;
        }
    }
}

void drawPiece(Board* b, Piece* piece) {
    if(!piece->isActive) {
        return;
    }

    void (*drawFunc[8])(Vector2, Vector2 , SDL_Color, int, SDL_Renderer*) = {
            &drawTriangle,
            &drawStraight,
            &drawDiagonal,
            &drawBlock,
            &drawSplitter,
            &drawHypercube,
            &drawLaser,
            &drawKing
    };

    Vector2 p1, p2;
    getBoxFromTile(b,piece->location,&p1,&p2);
    drawFunc[piece->type](p1,p2,piece->player->color, piece->rotation,b->renderer);
}

void drawDiagonal(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[2] = {
            refPoints[0],
            refPoints[4]
    };

    Vector2 mirror1Points[2] = {
            {refPoints[0].x+4, refPoints[0].y},
            {refPoints[4].x, refPoints[4].y-4}
    };
    Vector2 mirror2Points[2] = {
            {refPoints[0].x, refPoints[0].y+5},
            {refPoints[4].x-5, refPoints[4].y}
    };

    rotatePoints(points, refPoints[8], 2, rotation);
    rotatePoints(mirror1Points, refPoints[8], 2, rotation);
    rotatePoints(mirror2Points, refPoints[8], 2, rotation);

    thickLineRGBA(renderer, round(points[0].x), round(points[0].y), round(points[1].x), round(points[1].y), 8, 255,0,0, SDL_ALPHA_OPAQUE);
    SDL_Color mirrorColor = adjustColorBrightness(color, 0.5f);
    thickLineRGBA(renderer, round(mirror1Points[0].x), round(mirror1Points[0].y), round(mirror1Points[1].x), round(mirror1Points[1].y), 3, mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
    thickLineRGBA(renderer, round(mirror2Points[0].x), round(mirror2Points[0].y), round(mirror2Points[1].x), round(mirror2Points[1].y), 3, mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
}

void drawBlock(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[2] = {
            refPoints[0],
            refPoints[4]
    };

    Vector2 mirrorPoints[2] = {
            refPoints[0],
            refPoints[2]
    };

    rotatePoints(points, refPoints[8], 2, rotation);
    rotatePoints(mirrorPoints, refPoints[8], 2, rotation);

    boxRGBA(renderer, round(points[0].x), round(points[0].y), round(points[1].x), round(points[1].y),color.r,color.g,color.b,color.a);
    SDL_Color mirrorColor = adjustColorBrightness(color, 0.5f);
    thickLineRGBA(renderer, mirrorPoints[0].x, mirrorPoints[0].y, mirrorPoints[1].x, mirrorPoints[1].y, 3, mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
}

void drawTriangle(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[3] = {
            refPoints[0],
            refPoints[6],
            refPoints[4]
    };

    Vector2 mirrorPoints[2] = {
            refPoints[0],
            refPoints[4]
    };

    rotatePoints(points, refPoints[8], 3, rotation);
    rotatePoints(mirrorPoints, refPoints[8], 2, rotation);

    filledTrigonRGBA(renderer, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x,points[2].y,color.r, color.g, color.b, color.a);
    SDL_Color mirrorColor = adjustColorBrightness(color, 0.5f);
    thickLineRGBA(renderer, round(mirrorPoints[0].x), round(mirrorPoints[0].y), round(mirrorPoints[1].x), round(mirrorPoints[1].y), 3, mirrorColor.r,mirrorColor.g,mirrorColor.b, mirrorColor.a);
}

void drawStraight(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[2] = {
            refPoints[1],
            refPoints[5]
    };

    Vector2 mirror1Points[2] = {
            {refPoints[1].x-4, refPoints[1].y},
            {refPoints[5].x-4, refPoints[5].y}
    };
    Vector2 mirror2Points[2] = {
            {refPoints[1].x+4, refPoints[1].y},
            {refPoints[5].x+4, refPoints[5].y}
    };

    rotatePoints(points, refPoints[8], 2, rotation);
    rotatePoints(mirror1Points, refPoints[8], 2, rotation);
    rotatePoints(mirror2Points, refPoints[8], 2, rotation);

    thickLineRGBA(renderer, round(points[0].x), round(points[0].y), round(points[1].x), round(points[1].y), 8, color.r, color.g, color.b, color.a);

    SDL_Color mirrorColor = adjustColorBrightness(color, 0.5f);
    lineRGBA(renderer, round(mirror1Points[0].x), round(mirror1Points[0].y), round(mirror1Points[1].x), round(mirror1Points[1].y), mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
    lineRGBA(renderer, round(mirror2Points[0].x), round(mirror2Points[0].y), round(mirror2Points[1].x), round(mirror2Points[1].y), mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
}

void drawSplitter(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[3] = {
            refPoints[8],
            refPoints[4],
            refPoints[6]
    };

    Vector2 mirror1Points[2] = {
            refPoints[6],
            {refPoints[8].x + 1, refPoints[8].y - 1}
    };

    Vector2 mirror2Points[2] = {
            refPoints[4],
            {refPoints[8].x - 1, refPoints[8].y - 1}
    };
    rotatePoints(points, refPoints[8], 3, rotation);
    rotatePoints(mirror1Points, refPoints[8], 2, rotation);
    rotatePoints(mirror2Points, refPoints[8], 2, rotation);
    filledTrigonRGBA(renderer, points[0].x, points[0].y, points[1].x, points[1].y, points[2].x,points[2].y, color.r, color.g, color.b, color.a);

    SDL_Color mirrorColor = adjustColorBrightness(color, 0.5f);
    thickLineRGBA(renderer, mirror1Points[0].x, mirror1Points[0].y, mirror1Points[1].x, mirror1Points[1].y, 3,mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
    thickLineRGBA(renderer, mirror2Points[0].x, mirror2Points[0].y, mirror2Points[1].x, mirror2Points[1].y, 3,mirrorColor.r, mirrorColor.g, mirrorColor.b, mirrorColor.a);
}

void drawHypercube(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[8] = {
            {refPoints[0].x, refPoints[0].y+4},
            {refPoints[2].x, refPoints[2].y+4},
            {refPoints[2].x-3, refPoints[2].y},
            {refPoints[4].x-3, refPoints[4].y},
            {refPoints[4].x, refPoints[4].y-3},
            {refPoints[6].x, refPoints[6].y-3},
            {refPoints[6].x+3, refPoints[6].y},
            {refPoints[0].x+3, refPoints[0].y},
    };
    rotatePoints(points, refPoints[8], 8, rotation);
    thickLineRGBA(renderer, points[0].x, points[0].y, points[1].x, points[1].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[2].x, points[2].y, points[3].x, points[3].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[4].x, points[4].y, points[5].x, points[5].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[6].x, points[6].y, points[7].x, points[7].y, 8, color.r, color.g, color.b, color.a);
}

void drawLaser(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[10] = {
            {refPoints[1].x-10, refPoints[1].y+5},
            {refPoints[1].x+10, refPoints[1].y+5},
            {refPoints[1].x+10, refPoints[1].y+10},
            {refPoints[1].x+7, refPoints[1].y+12},
            {refPoints[1].x+7, refPoints[1].y+42},
            refPoints[4],
            refPoints[6],
            {refPoints[1].x-7, refPoints[1].y+42},
            {refPoints[1].x-7, refPoints[1].y+12},
            {refPoints[1].x-10, refPoints[1].y+10},
    };
    rotatePoints(points, refPoints[8], 10, rotation);

    Sint16 xVals[10];
    Sint16 yVals[10];
    for(int i = 0; i < 10; i++) {
        xVals[i] = round(points[i].x);
        yVals[i] = round(points[i].y);
    }

    filledPolygonRGBA(renderer,xVals,yVals,10,color.r, color.g, color.b, color.a);
}

void drawKing(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer) {
    Vector2 refPoints[9];
    getBoxRefPoints(p1, p2,5,refPoints,9);

    Vector2 points[4] = {
            refPoints[1],
            refPoints[3],
            refPoints[5],
            refPoints[7]
    };
    rotatePoints(points, refPoints[8], 4, rotation);
    thickLineRGBA(renderer, points[0].x, points[0].y, points[1].x, points[1].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[1].x, points[1].y, points[2].x, points[2].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[2].x, points[2].y, points[3].x, points[3].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[3].x, points[3].y, points[0].x, points[0].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[0].x, points[0].y, points[2].x, points[2].y, 8, color.r, color.g, color.b, color.a);
    thickLineRGBA(renderer, points[3].x, points[3].y, points[1].x, points[1].y, 8, color.r, color.g, color.b, color.a);
}

void drawBoard(Board* b) {
    b->blockWidth = b->width / 9;
    b->blockHeight = b->height / 9;
    for(int y = 0; y < 9; y++) {
        for(int x = 0; x < 9; x++) {
            int xCoord = x*b->blockWidth + b->x;
            int yCoord = y*b->blockWidth + b->y;
            if((y*9+x)%2) {
                boxRGBA(b->renderer, xCoord, yCoord, xCoord+b->blockWidth, yCoord+b->blockHeight,0,0,0,255);
            } else {
                boxRGBA(b->renderer, xCoord, yCoord, xCoord+b->blockWidth, yCoord+b->blockHeight,255,255,255,255);
            }
        }
    }
}

void drawBoardHighlights(Board* b, GameState* gs) {
    Vector2 p1, p2;

    // Highlight board of selected piece
    if(gs->isPieceSelected) {
        getBoxFromTile(b,(Vector2){gs->selectedPiece->location.x, gs->selectedPiece->location.y}, &p1, &p2);
        boxRGBA(b->renderer, p1.x, p1.y, p2.x, p2.y, 255, 255, 0, SDL_ALPHA_OPAQUE);
    }

    // Highlight board hover
    for(int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            getBoxFromTile(b, (Vector2){x, y}, &p1, &p2);

            if (gs && gs->isTileHovered && gs->hoveredTile.x == x && gs->hoveredTile.y == y) {
                SDL_Color highlightColor = adjustColorBrightness(gs->players[gs->activePlayer].color, .50);
                boxRGBA(b->renderer, p1.x, p1.y, p2.x, p2.y, highlightColor.r,
                        highlightColor.g, highlightColor.b, 100);
            }
        }
    }

}