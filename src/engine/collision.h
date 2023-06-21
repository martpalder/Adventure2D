#ifndef COLLISION_H
#define COLLISION_H

bool isCollisionMap(const int TILE_SIZE, Rectangle* pEntityRect, Tilemap* pTilemap);
bool isCollisionEntities(Rectangle* pEntityRect, Entity* pEntities, int numEntities);

#endif