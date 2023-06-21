#include "./tileset.h"
#include "./entity.h"

Entity createEntity(float x, float y, float spd, const float TILE_SIZE) {
    return (Entity) {
        256.0f,
        0.0f,
        0,
        3,
        (Rectangle){ x, y, TILE_SIZE, TILE_SIZE },
        (Vector2){ 0.0f, 0.0f },
        DOWN,
        0.0f,
        0
    };
}

Vector2 getPos(Entity* pEntity)
{
    return (Vector2){ pEntity->dstRect.x, pEntity->dstRect.y };
}

void updateEntity(Entity* pEntity, float dt) {
    pEntity->moveStep = pEntity->spd * dt;
    pEntity->center = (Vector2){
        pEntity->dstRect.x + 64.0f,
        pEntity->dstRect.y + 64.0f
    };
    switch(pEntity->dir)
    {
    case 0:
        pEntity->rotation = 270.0f;
        break;
    case 1:
        pEntity->rotation = 90.0f;
        break;
    case 2:
        pEntity->rotation = 0.0f;
        break;
    case 3:
        pEntity->rotation = 180.0f;
        break;
    }
}

void animateEntity(Entity* pEntity, int start, int end)
{
    ++pEntity->idx;
    if(pEntity->idx < start || pEntity->idx > end)
    {
        pEntity->idx = start;
    }
}

void moveLeft(Entity* pEntity) {
    pEntity->dstRect.x -= pEntity->moveStep;
}

void moveRight(Entity* pEntity) {
    pEntity->dstRect.x += pEntity->moveStep;
}

void moveUp(Entity* pEntity) {
    pEntity->dstRect.y -= pEntity->moveStep;
}

void moveDown(Entity* pEntity) {
    pEntity->dstRect.y += pEntity->moveStep;
}

void drawEntity(Entity* pEntity, Tileset* pTileset)
{
    // Draw the source rectangle 
    DrawTextureRec(
        pTileset->texture,
        *(pTileset->pSrcRects + pEntity->idx),
        (Vector2){ pEntity->dstRect.x, pEntity->dstRect.y },
        WHITE
    );

    // Draw a rectangle outline
    //DrawRectangleLinesEx(pEntity->dstRect, 2.0f, (Color){ 255, 0, 0, 255 });
}

void drawEntityRotation(Entity* pEntity, Tileset* pTileset)
{
    // Set destination rectangle
    Rectangle dstRect = {
        pEntity->center.x,
        pEntity->center.y,
        pTileset->tileSize,
        pTileset->tileSize
    };

    // Draw texture with rotation
    DrawTexturePro(
        pTileset->texture,
        *pTileset->pSrcRects,
        dstRect,
        (Vector2){ pTileset->tileHalf, pTileset->tileHalf },
        pEntity->rotation,
        WHITE
    );

    // Draw rectangle outlines
    //DrawRectangleLinesEx(pEntity->dstRect, 2.0f, (Color){ 255, 0, 0, 255 });
}
