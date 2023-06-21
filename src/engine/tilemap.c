#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "./tileset.h"
#include "./tilemap.h"
#include "./rect.h"
#include "./input.h"

#define W 800
#define H 600

Tilemap createTilemap(const int MAP_W, const int MAP_H, const int TILE_SIZE, Tileset* pTileset)
{
     // Set map attributes
    Tilemap tmap;
    tmap.mapW = MAP_W;
    tmap.mapH = MAP_H;
    tmap.tileSize = TILE_SIZE;
    tmap.numIndices = MAP_W * MAP_H;
    tmap.pTileset = pTileset;

    // Allocate memory for indices
    tmap.pIndices = (char*)MemAlloc(tmap.numIndices);
    if(tmap.pIndices == NULL){ fprintf(stderr, "MemAlloc failed!\n"); }

    // Set indices
    for(int i = 0; i < tmap.numIndices; ++i)
    {
        *(tmap.pIndices + i) = -1;
    }

    return tmap;
}

Tilemap loadTilemap(const float TILE_SIZE, const char* FILEPATH, Tileset* pTileset)
{
    // Variables
    int mapIdx = 0;
    int len;
    char c;
    char* txt;
    char* pch;

    // Set map attributes
    Tilemap tmap;
    tmap.mapW = 0;
    tmap.mapH = 0;
    tmap.tileSize = TILE_SIZE;
    tmap.pTileset = pTileset;

    // FILE READ 1
    // Read the map file
    txt = LoadFileText(FILEPATH);
    // Split the file text by lines
    pch = strtok(txt, "\n");
    // Until file end
    while (pch != NULL)
    {
        // Get line length
        len = strcspn(pch, "\n");

        // For every line character
        tmap.mapW = 0;
        for(int i = 0; i < len; ++i) {
            // Increase map width
            ++tmap.mapW;
        }
        // Increase map height
        ++tmap.mapH;

        // Get the next line
        pch = strtok(NULL, "\n");
    }
    printf("Map width: %d\n", tmap.mapW);
    printf("Map height: %d\n", tmap.mapH);
    printf("TIle size: %f\n", tmap.tileSize);
    // Free the map file
    UnloadFileText(txt);
    txt = NULL;

    // Allocate memory for map tile indices
    tmap.numIndices = tmap.mapW * tmap.mapH;
    tmap.pIndices = (char*)MemAlloc(tmap.numIndices);
    if(tmap.pIndices == NULL){ fprintf(stderr, "MemAlloc failed!\n"); }

    // FILE READ 2
    // Read the map file again
    txt = LoadFileText(FILEPATH);
    // Split the text by lines
    pch = strtok(txt, "\n");
    // Until file end
    while (pch != NULL)
    {
        // Get line length
        len = strcspn(pch, "\n");

        // For every line character
        for(int i = 0; i < len; ++i) {
            // Get its value
            c = *(pch + i);
            // Add to indices as ASCII
            *(tmap.pIndices + mapIdx++) = c - 48;
            //printf("%c", c);
        }
        //printf("\n");
        //printf("Length: %d\n", len);

        // Get the next line
        pch = strtok(NULL, "\n");
    }

    // Check if map was loaded
    if(txt){ printf("Map '%s' loaded!\n", FILEPATH); }
    else{ fprintf(stderr, "Failed to load map '%s'!\n", FILEPATH); }

    // Free the map file
    UnloadFileText(txt);
    txt = NULL;

    // Create destination rectangles (for collision)
    tmap.pDstRects = (Rectangle*)MemAlloc(tmap.numIndices * sizeof(Rectangle));
    if(!tmap.pDstRects){ fprintf(stderr, "MemAlloc failed!\n"); }
    Rectangle dstRect;
    int tileIdx = 0;
    for (int i = 0; i < tmap.mapH; ++i)
    {
        for (int j = 0; j < tmap.mapW; ++j)
        {
            dstRect = (Rectangle){ j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            *(tmap.pDstRects + tileIdx++) = dstRect;
        }
    }

    return tmap;
}

void addTileset(Tilemap* pTilemap, Tileset* pTileset)
{
    pTilemap->pTileset = pTileset;
}

void drawTilemap(Tilemap* pTilemap, Camera2D* pCam, Mouse* pMouse, const bool EDITOR)
{
    // Get map attributes
    int mapW = pTilemap->mapW;
    int mapH = pTilemap->mapH;
    float tileSize = pTilemap->tileSize;
    Rectangle dstRect;
    char* pIndices = pTilemap->pIndices;
    Tileset* pTileset = pTilemap->pTileset;

    // For each map tile
    float originX = pCam->target.x - pCam->offset.x;
    float originY = pCam->target.y - pCam->offset.y;
    int tileIdx = 0;
    char srcIdx = 0;
    for(int i = 0; i < mapH; ++i)
    {
        for(int j = 0; j < mapW; ++j)
        {
            // Get its source index
            srcIdx = *(pIndices + tileIdx);

            // Get its destination rectangle
            //dstRect = *(pTilemap->pDstRects + tileIdx);
            dstRect = (Rectangle){ j * tileSize, i * tileSize, tileSize, tileSize };

            // If it's on the screen
            if(dstRect.x > originX - tileSize && dstRect.x < originX + W
            && dstRect.y > originY - tileSize && dstRect.y < originY + H)
            {
                // If it has a tileset
                if(pTilemap->pTileset)
                {
                    // Draw the tile
                    DrawTextureRec(
                        pTileset->texture,
                        *(pTileset->pSrcRects + srcIdx),
                        (Vector2){ dstRect.x, dstRect.y },
                        WHITE
                    );
                }
            }

            // If used in editor mode
            if(EDITOR && pMouse)
            {
                // If mouse over a tile
                if(pMouse->screenPos.x < (float)W - tileSize && CheckCollisionPointRec(pMouse->worldPos, dstRect))
                {
                    // If clicked on it
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        // Change its tile index
                        *(pIndices + tileIdx) = pTileset->currIdx;
                    }
                    
                    // Draw a red outline
                    DrawRectangleLinesEx(dstRect, 2.0f, GREEN);
                }
            }

            // Increase tile index
            ++tileIdx;
        }
    }

    // Draw map outline if in editor
    if(EDITOR)
    {
        DrawRectangleLines(0, 0, mapW * tileSize, mapH * tileSize, WHITE);
    }
}

void drawStartup(Rectangle* pCreateRect, Rectangle* pLoadRect, int x, int y)
{
    DrawRectangleRec(*pCreateRect, GRAY);
    DrawRectangleRec(*pLoadRect, GRAY);
    DrawText("Tilemap editor", x + 32, y, 36, GREEN);
    DrawText("Create", x + 24, y + 68, 24, YELLOW);
    DrawText("Load", x + 228, y + 68, 24, YELLOW);
}

void drawCreation(char* widthStr, char* heightStr, char* sizeStr, int x, int y)
{
    DrawText("Create a tilemap", x, y, 24, BLUE);
    DrawText("Map width: ", x, y + 32, 24, GREEN);
    DrawText(widthStr, x + 160, y + 32, 24, RED);
    DrawText("Map height: ", x, y + 64, 24, GREEN);
    DrawText(heightStr, x + 160, y + 64, 24, RED);
    DrawText("Tile size: ", x, y + 96, 24, GREEN);
    DrawText(sizeStr, x + 160, y + 96, 24, RED);
}

void drawLoading(char* fileStr, int x, int y)
{
    DrawText("Load a tilemap", x, y, 24, BLUE);
    DrawText(fileStr, x, y + 64, 24, RED);
}

void saveTilemap(Tilemap* pTilemap, const char* FILEPATH)
{
    // Open a file in write mode
    FILE* fp;
    fopen_s(&fp, FILEPATH, "w");
    if(fp == NULL)
    {
        fprintf(stderr, "Failed to open file '%s'!\n", FILEPATH);
        exit(-1);
    }
    else
    {
        int tileIdx = 0;
        for(int i = 0; i < pTilemap->mapH; ++i) {
            for(int j = 0; j < pTilemap->mapW; ++j) {
                fputc(*(pTilemap->pIndices + tileIdx++) + 48, fp);
            }
            fputc('\n', fp);
        }
        fclose(fp);
        fp = NULL;
    }

    printf("Map '%s' saved!\n", FILEPATH);
}

void destroyTilemap(Tilemap* pTilemap)
{
    // Free the destination rectangles
    MemFree(pTilemap->pDstRects);
    pTilemap->pDstRects = NULL;

    // Free the indices
    MemFree(pTilemap->pIndices);
    pTilemap->pIndices = NULL;
    pTilemap = NULL;
}
