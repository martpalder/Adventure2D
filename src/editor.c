#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "./engine/rect.h"
#include "./engine/input.h"
#include "./engine/text.h"
#include "./engine/tileset.h"
#include "./engine/tilemap.h"

#define W 800
#define H 600
#define HALF_W W / 2
#define HALF_H H / 2
#define FPS_MAX 60
#define TILE_SIZE 128
#define EDITOR 1

KB kb;
Mouse mouse;

void init(void)
{
    // Create a window
    InitWindow(W, H, "Tilemap Editor");

    // Initialize input structs
    kb = initKB();

    // Set target FPS
    SetTargetFPS(FPS_MAX);
}

int main(void)
{
    // Variables
    // Strings
    char widthStr[4];
    char heightStr[4];
    char sizeStr[4];
    char fileStr[24];

    // String lengths
    int lenW = 0;
    int lenH = 0;
    int lenSize = 0;
    int lenFile = 0;

    // Text objects
    Text widthTxt = { 0, 3, widthStr };
    Text heightTxt = { 0, 3, heightStr };
    Text sizeTxt = { 0, 3, sizeStr };
    Text fileTxt = { 0, 23, fileStr };

    // Initialize
    init();

    // Create a tileset
    Tileset tset1 = createTileset(1, 4, TILE_SIZE, "./tilesets/tiles1.png");
    Tilemap tmap;

    // Set text start position
    int textX = HALF_W - 160;
    int textY = HALF_H - 64;

    // Create startup rectangles
    Rectangle menuRect = { HALF_W - 192, HALF_H - 96, 384, 192 };
    Rectangle createRect = { textX, textY + 56, 128, 48 };
    Rectangle loadRect = { textX + 192, textY + 56, 128, 48 };

    // Field and load check
    char field = 0;
    bool create = 0;
    bool load = 0;
    // Startup loop
    while(!WindowShouldClose())
    {
        // Check keyboard input
		checkKbInput(&kb);
        if(kb.enter)
        {
            if(create && widthTxt.len && heightTxt.len && sizeTxt.len)
            {
                // Convert strings to int
                int mapW = atoi(widthStr);
                int mapH = atoi(heightStr);
                int tileSize = atoi(sizeStr);
                printf("Map width: %d\n", mapW);
                printf("Map height: %d\n", mapH);

                // Create a blank tilemap
                if(!load){ tmap = createTilemap(mapW, mapH, tileSize, &tset1); }
                break;
            }
            else if(load && fileTxt.len)
            {
                if(FileExists(fileTxt.str))
                {
                    tmap = loadTilemap(TILE_SIZE, fileTxt.str, &tset1);
                    break;
                }
            }
        }
        else if(kb.tab)
        {
            ++field;
            if(field > 2){ field = 0; }
        }
        
        // If creating
        if(create)
        {
            // Write texts to strings
            switch(field)
            {
                case 0:
                    textInput(&widthTxt, &kb);
                    break;
                case 1:
                    textInput(&heightTxt, &kb);
                    break;
                case 2:
                    textInput(&sizeTxt, &kb);
                    break;
            }
        }
        // If loading
        else if(load)
        {
            // Write filepath text to string
            textInput(&fileTxt, &kb);
        }

        // Check mouse input
        checkMouseInput(&mouse, NULL);

        // If mouse over "create" and left-clicked
        if(CheckCollisionPointRec(mouse.screenPos, createRect)) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                create = 1;
            }
        }
        // If mouse over "load" and left-clicked
        else if(CheckCollisionPointRec(mouse.screenPos, loadRect)) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                load = 1;
            }
        }

        // Draw
        BeginDrawing();
            // Clear screen
            ClearBackground(BLACK);

            // Draw the menu rectangle
            DrawRectangleRec(menuRect, WHITE);

            // If creating map
            if(create)
            {
                // Draw map creation text
                drawCreation(widthTxt.str, heightTxt.str, sizeTxt.str, textX, textY);
            }
            // If loading map
            else if(load)
            {
                // Draw map loading text
                drawLoading(fileTxt.str, textX, textY);
            }
            // Otherwise
            else
            {
                // Draw tilemap startup screen
                drawStartup(&createRect, &loadRect, textX, textY);
            }
        EndDrawing();
    }

    // Create a 2D camera
    Camera2D cam = { 0 };
    cam.target = (Vector2){ HALF_W, HALF_H };
    cam.offset = (Vector2){ HALF_W, HALF_H };
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;

    // Main loop
    float dt;
    float errTime = 0.0f;
    float moveStep;
    char mapStr[16];
    char camStr[22];
    char idxStr[16];
    char* errStr = NULL;
    int fileLen = 0;
    bool save = 0;
    load = 0;
    clearText(&fileTxt);
    while(!WindowShouldClose())
    {
        // Get delta time
        dt = GetFrameTime();
        errTime += dt;

        // Calculate move step
        moveStep = 512.0f * dt;

		// Check keyboard input
		checkKbInput(&kb);

		// Move the camera
		if(kb.left) {
            cam.target.x -= moveStep;
        }
		else if(kb.right) {
            cam.target.x += moveStep;
        }
		else if(kb.up) {
            cam.target.y -= moveStep;
        }
		else if(kb.down) {
            cam.target.y += moveStep;
        }

        // If "ctrl" is pressed
        if(kb.ctrl)
        {
            if(kb.s)
            {
                // Enter save menu
                if(save){ save = 0; }
                else{ save = 1; clearText(&fileTxt); }
            }
            else if(kb.l)
            {
                // Enter load menu
                if(load){ load = 0; }
                else{ load = 1; clearText(&fileTxt); }
            }
        }

        // Check mouse input
		checkMouseInput(&mouse, &cam);

        // Set string values
        sprintf(mapStr, "Map W: %d H: %d", tmap.mapW, tmap.mapH);
        sprintf(camStr, "Cam pos X: %d Y: %d", (int)cam.target.x, (int)cam.target.y);
        sprintf(idxStr, "Tile index: %d", tset1.currIdx);

        // If in save menu
        if(save)
        {
            textInput(&fileTxt, &kb);
            if(kb.enter)
            {
                saveTilemap(&tmap, fileStr);
                save = 0;
            }
        }
        // If in load menu
        else if(load)
        {
            textInput(&fileTxt, &kb);
            if(kb.enter)
            {
                if(FileExists(fileTxt.str))
                {
                    destroyTilemap(&tmap);
                    tmap = loadTilemap(TILE_SIZE, fileStr, &tset1);
                    load = 0;
                }
                else
                {
                    errStr = "Map does not exist!";
                    errTime = 0.0f;
                }
            }
        }

        // Draw
        BeginDrawing();
            ClearBackground(BLACK); // Clear screen  
            BeginMode2D(cam); // Begin 2D camera mode
                drawTilemap(&tmap, &cam, &mouse, EDITOR);
            EndMode2D(); // End 2D camera mode
            // Draw tileset and text
            drawTileset(&tset1, &mouse, W);
            if(save)
            {
                // Draw the menu rectangle
                DrawRectangleRec(menuRect, WHITE);
                DrawText("Save the tilemap", textX, textY, 24, BLUE);
                DrawText(fileTxt.str, textX, textY + 64, 24, RED);
            }
            else if(load)
            {
                // Draw the menu rectangle
                DrawRectangleRec(menuRect, WHITE);
                DrawText("Load the tilemap", textX, textY, 24, BLUE);
                DrawText(fileTxt.str, textX, textY + 64, 24, RED);
            }
            if(errStr && errTime < 3.0f){ DrawText(errStr, 320, 64, 36, RED); }
            DrawFPS(0, 0);
            DrawText(mapStr, 0, 32, 24, ORANGE);
            DrawText(camStr, 0, 64, 24, ORANGE);
            DrawText(idxStr, 0, 96, 24, ORANGE);
        EndDrawing();
    }

    // Free resources
    destroyTilemap(&tmap);
    destroyTileset(&tset1);

    // Close the window
    CloseWindow();
    return 0;
}
