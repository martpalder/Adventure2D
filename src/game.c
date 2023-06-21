#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "./engine/entity.h"
#include "./engine/rect.h"
#include "./engine/input.h"
#include "./engine/tileset.h"
#include "./engine/tilemap.h"
#include "./engine/camera.h"
#include "./engine/collision.h"
#include "./engine/text.h"
#include "./engine/screenshot.h"

#define W 800
#define H 600
#define W_HALF W>>1
#define H_HALF H>>1
#define FPS_MAX 60
#define TILE_SIZE 128
#define TILE_HALF TILE_SIZE>>1
#define EDITOR 0

// Globals
KB kb;
// Dialogue text
const int NUM_TEXTS = 3;
int textIdx = 0;
char text1[][48] = {
    "Welcome, player!",
    "This is a 2D action-adventure.",
    "Thanks for playing my game!"
};

void init(void)
{
    // Create a window
    InitWindow(W, H, "2D action-adventure");

    // Set target FPS
    SetTargetFPS(FPS_MAX);

    // Initialize audio device
    InitAudioDevice();

    // Initialize input structs
    kb = initKB();
}

int main(void)
{
    // Initialize
    init();

    // Load resources
    // Load map tilesets
    Tileset tset1 = createTileset(1, 4, TILE_SIZE, "./tilesets/tiles1.png");
    // Load a tilemap
    Tilemap tmap = loadTilemap(TILE_SIZE, "./maps/map2.txt", &tset1);
    // Load entity spritesheets
    Tileset playerTiles = createTileset(1, 1, TILE_SIZE, "./sprites/Topdown.png");
    // Load HUD textures
    Texture2D heartTex = LoadTexture("./hud/heart.png");
    // Load and play music
    Music music = LoadMusicStream("./music/adventure.mp3");
    PlayMusicStream(music);
    // Load sound effects
    //Sound slashSnd = LoadSound("./sfx/slash.wav");

    // Create entities
    Entity player = createEntity(0, 600, 128, TILE_SIZE);
    Entity* pEnemies = (Entity*)malloc(1 * sizeof(Entity));
    pEnemies[0] = createEntity(320, 600, 128, TILE_SIZE );

    // Create a 2D camera
    Camera2D cam = createCamera2D(player.center, (Vector2){ W_HALF, H_HALF });

    // Main loop
    float dt;
    float seconds = 0.0f;
    float errTime = 0.0f;
    char posStr[16];
    char errStr[64];
    bool textOpen = 0;
    while(!WindowShouldClose())
    {
        // Get delta time
        dt = GetFrameTime();
        seconds += dt;
        errTime += dt;

        // Calculate move steps
        updateEntity(&player, dt);

		// Check keyboard input
		checkKbInput(&kb);

		// Player movement
		if(kb.left)
        {
            // Move left if no collision
            moveLeft(&player);
            if(isCollisionMap(TILE_SIZE, &player.dstRect, &tmap))
            {
                moveRight(&player);
            }

            // Set direction to left
            player.dir = LEFT;
        }
		else if(kb.right)
        {
            // Move right if no collision
            moveRight(&player);
            if(isCollisionMap(TILE_SIZE, &player.dstRect, &tmap))
            {
                moveLeft(&player);
            }

            // Set direction to right
            player.dir = RIGHT;
        }
		else if(kb.up)
        {
            // Move up if no collision
            moveUp(&player);
            if(isCollisionMap(TILE_SIZE, &player.dstRect, &tmap))
            {
                moveDown(&player);
            }

            // Set direction to up
            player.dir = UP;
        }
		else if(kb.down)
        {
            // Move down if no collision
            moveDown(&player);
            if(isCollisionMap(TILE_SIZE, &player.dstRect, &tmap))
            {
                moveUp(&player);
            }

            // Set direction to down
            player.dir = DOWN;
        }

        // Open text box
        if(kb.space)
        {
            if(!textOpen && CheckCollisionRecs(player.dstRect, pEnemies->dstRect))
            {
                textOpen = 1;
                textIdx = 0;
            }
            else
            {
                ++textIdx;
                if(textIdx > NUM_TEXTS - 1)
                {
                    textOpen = 0;
                    textIdx = 0;
                }
            }
        }
        // Take a screenshot
        else if(kb.p)
        {
            if(!screenshot())
            {
                sprintf(errStr, "Directory '../screenshots' does not exist!");
                errTime = 0.0f;
            }
        }

        // Camera target follows player
        updateCamera2D(&cam, player.center);

        // Update music stream
        UpdateMusicStream(music);

        // Set string values
        sprintf(posStr, "X: %d Y: %d", (int)player.dstRect.x, (int)player.dstRect.y);

        // Draw
        BeginDrawing();
            // Clear screen
            ClearBackground(BLACK);

            BeginMode2D(cam); // Begin 2D camera mode
                // Draw the tilemap
                drawTilemap(&tmap, &cam, NULL, EDITOR);
                // Draw entities
                drawEntityRotation(&player, &playerTiles);
                DrawRectangleRec(pEnemies->dstRect, (Color){ 255, 0, 0, 255 });
            EndMode2D(); // End 2D camera mode

            // Draw text and HUD elements
            if(textOpen){ textBox(text1[textIdx], W, H); }
            for(unsigned char i = 0; i < player.lives; ++i) {
                if(i < 5){ DrawTexture(heartTex, i * 48, 0, WHITE); }
                else if(i < 10){ DrawTexture(heartTex, (i - 5) * 48, 48, WHITE); }
                else{ DrawTexture(heartTex, (i - 10) * 48, 96, WHITE); }
            }
            DrawFPS(0, 0);
            DrawText(posStr, 0, 96, 24, ORANGE);
            if(errTime < 2.0f){ DrawText(errStr, 32, 384, 36, RED); }
        EndDrawing();
    }

    // Free resources
    // Entities
    free(pEnemies);
    pEnemies = NULL;
    // Audio
    UnloadMusicStream(music);
    // Graphics
    UnloadTexture(heartTex);
    destroyTileset(&playerTiles);
    destroyTilemap(&tmap);

    // Close audio device
    CloseAudioDevice();

    // Close the window
    CloseWindow();
    return 0;
}
