#include <stdio.h>
#include <raylib.h>
#include "./rect.h"

Rectangle* createRects(const int NUM_ROWS, const int NUM_COLS, const int TILE_SIZE)
{
    // Get tile size
    float tileSize = (float)TILE_SIZE;

    // Allocate memory for source rectangles
    Rectangle* srcRects = (Rectangle*)MemAlloc(NUM_ROWS * NUM_COLS * sizeof(Rectangle));
    if(srcRects == NULL){ fprintf(stderr, "MemAalloc failed!\n"); }

    // For each source tile
    int idx = 0;
    for(int i = 0; i < NUM_ROWS; ++i)
    {
        for(int j = 0; j < NUM_COLS; ++j)
        {
            // Set a source rectangle
            *(srcRects + idx) = (Rectangle){ j * tileSize, i * tileSize, tileSize, tileSize };
            ++idx;
        }
    }
    
    return srcRects;
}

void destroyRects(Rectangle* pRects)
{
    MemFree(pRects);
    pRects = NULL;
}
