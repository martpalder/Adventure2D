#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include "./rect.h"
#include "./tilemap.h"
#include "./entity.h"

bool isCollisionMap(const int TILE_SIZE, Rectangle* pEntityRect, Tilemap* pTilemap)
{
    // Variables
    int lenX;
    int lenY;
    float dist;
    Rectangle dstRect;

    // For every map tile
    for(int i = 0; i < pTilemap->numIndices; ++i)
    {
        // If index is greater than 1
        if(*(pTilemap->pIndices + i) > 1)
        {
            // Set current destination rectangle
            dstRect = *(pTilemap->pDstRects + i);

            // Calculate distance
            lenX = pEntityRect->x - dstRect.x;
            lenY = pEntityRect->y - dstRect.y;
            dist = sqrt((lenX*lenX) + (lenY*lenY));
            //printf("Distance: %f\n", dist);

            // If distance is less than 1.5 times tile size
            if(dist < 1.5f * (float)TILE_SIZE)
            {
                // If there's collision
                if(CheckCollisionRecs(*pEntityRect, dstRect))
                {
                    return 1;
                }
            }
        }
    }

    // Otherwise: no collision
    return 0;
}

bool isCollisionEntities(Rectangle* pEntityRect, Entity* pEntities, int numEntities)
{
    // For each entity
    for(int i = 0; i < numEntities; ++i)
    {
        // Check collision
        if(pEntities + i) {
            if(CheckCollisionRecs(*pEntityRect, (pEntities + i)->dstRect))
            {
                (pEntities + i)->destroyed = 1;
                return 1;
            }
        }
    }
    return 0;
}