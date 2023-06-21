#ifndef TILEMAP_H
#define TILEMAP_H
#include "./tileset.h"
#include "./input.h"

typedef struct
{
    int mapW;
    int mapH;
    float tileSize;
    int numIndices;
    char* pIndices;
    Rectangle* pDstRects;
    Tileset* pTileset;
} Tilemap;

Tilemap createTilemap(const int MAP_W, const int MAP_H, const int TILE_SIZE, Tileset* pTileset);
Tilemap loadTilemap(const float TILE_SIZE, const char* FILEPATH, Tileset* pTileset);
void addTileset(Tilemap* pTilemap, Tileset* pTileset);
void saveTilemap(Tilemap* pTilemap, const char* FILEPATH);
void drawTilemap(Tilemap* pTilemap, Camera2D* pCam, Mouse* pMouse, const bool EDITOR);
void drawStartup(Rectangle* pCreateRect, Rectangle* pLoadRect, int x, int y);
void drawCreation(char* widthStr, char* heightStr, char* sizeStr, int x, int y);
void drawLoading(char* fileStr, int x, int y);
void destroyTilemap(Tilemap* pTilemap);

#endif
