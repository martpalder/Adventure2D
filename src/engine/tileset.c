#include <stdio.h>
#include <raylib.h>
#include "./rect.h"
#include "./input.h"
#include "./tileset.h"

Tileset createTileset(const int NUM_ROWS, const int NUM_COLS, const float TILE_SIZE, const char* FILEPATH)
{
    Tileset tileset;
    tileset.numRows = NUM_ROWS;
    tileset.numCols = NUM_COLS;
    tileset.tileSize = TILE_SIZE;
    tileset.tileHalf = TILE_SIZE / 2;
    tileset.startY = 0.0f;
    tileset.currIdx = 0;
    tileset.currTile = (Rectangle){ 0, 0, TILE_SIZE, TILE_SIZE };
    tileset.texture = LoadTexture(FILEPATH);
    tileset.pSrcRects = createRects(NUM_ROWS, NUM_COLS, TILE_SIZE);
    return tileset;
}

void drawTileset(Tileset* pTileset, Mouse* pMouse, const float W)
{
    // Get tileset attributes
    int numIndices = pTileset->numCols * pTileset->numCols;
    float tileSize = pTileset->tileSize;
    int tileIdx = 0;
    Vector2 dstPos = { W - tileSize, pTileset->startY };
    Rectangle dstRect;
    
    // For each tileset tile
    for(int i = 0; i < numIndices; ++i){
        dstRect = (Rectangle){ dstPos.x, dstPos.y, tileSize, tileSize };
        // Draw the tile
        DrawTextureRec(
            pTileset->texture,
            *(pTileset->pSrcRects + i),
            dstPos,
            WHITE
        );
        
        // If mouse over the tile
        if(pMouse->screenPos.x > 640 - tileSize && CheckCollisionPointRec(pMouse->screenPos, dstRect))
        {
            // If clicked on it
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // Get its tile index
                pTileset->currIdx = i;
                
                // Get pointer to its source rectangle
                pTileset->currTile = *(pTileset->pSrcRects + i);
            }

            // Draw a red outline
            DrawRectangleLinesEx(dstRect, 2.0f, YELLOW);
        }

        // Change vertical position
        dstPos.y += tileSize;

        // Increase tile index
        ++tileIdx;
    }

    // Add mouse scrolling
    pTileset->startY += GetMouseWheelMove() * 32;
    if(pTileset->startY > 0.0f){ pTileset->startY = 0.0f; }

    // Draw a tile to mouse position
    DrawTextureRec(
        pTileset->texture,
        pTileset->currTile,
        (Vector2){ pMouse->screenPos.x - tileSize / 2, pMouse->screenPos.y - tileSize / 2 },
        WHITE
    );
}

void destroyTileset(Tileset* pTileset)
{
    destroyRects(pTileset->pSrcRects);
    pTileset->pSrcRects = NULL;
    UnloadTexture(pTileset->texture);
}

void printSrcRects(Tileset* pTileset)
{
    int numRects = pTileset->numCols * pTileset->numRows;
    Rectangle srcRect;
    for(int i = 0; i < numRects; ++i)
    {
        srcRect = *(pTileset->pSrcRects + i);
        printf("Source rect %d X: %f Y: %f W: %f H: %f\n", i, srcRect.x, srcRect.y, srcRect.width, srcRect.height);
    }
}