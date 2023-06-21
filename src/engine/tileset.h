#ifndef TILESET_H
#define TILESET_H
#include "./input.h"

typedef struct
{
    int numRows;
    int numCols;
    float tileSize;
    float tileHalf;
    float startY;
    char currIdx;
    Rectangle currTile;
    Texture2D texture;
    Rectangle* pSrcRects;
} Tileset;

Tileset createTileset(const int NUM_ROWS, const int NUM_COLS, const float TILE_SIZE, const char* FILEPATH);
void drawTileset(Tileset* pTileset, Mouse* pMouse, const float W);
void destroyTileset(Tileset* pTileset);
void printSrcRects(Tileset* pTileset);

#endif