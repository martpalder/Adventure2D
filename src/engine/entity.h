#ifndef ENTITY_H
#define ENTITY_H
#include <raylib.h>
#include "./tileset.h"

enum direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

typedef struct
{
    float spd;
    float moveStep;
    int idx;
    unsigned char lives;
    Rectangle dstRect;
    Vector2 center;
    enum direction dir;
    float rotation;
    bool destroyed;
} Entity;

Entity createEntity(float x, float y, float spd, const float TILE_SIZE);
Vector2 getPos(Entity* pEntity);
void updateEntity(Entity* pEntity, float dt);
void animateEntity(Entity* pEntity, int start, int end);
void moveLeft(Entity* pEntity);
void moveRight(Entity* pEntity);
void moveUp(Entity* pEntity);
void moveDown(Entity* pEntity);
void drawEntity(Entity* pEntity, Tileset* pTileset);
void drawEntityRotation(Entity* pEntity, Tileset* pTileset);

#endif