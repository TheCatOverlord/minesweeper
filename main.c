/* Mine Sweeper
// made by Firewire
// 
// 0 = Bomb
// 1 - 8 = number of surrounding bombs
// 9 = flag
// 10 = no bombs around
*/

#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define TILES_HEIGHT 10
#define TILES_WIDTH 10
#define BOMBS 10

typedef enum GameScreen { LOGO, TITLE, GAMEPLAY } GameScreen;

typedef struct Tiles
{
    Vector2 pos;
    Vector2 size;
    int value;
    bool flagged;
    bool visible;
} Tiles;

// Global varibles
Tiles tiles[TILES_WIDTH][TILES_HEIGHT] = { 0 }; 

int SurroundingBombs(int x, int y)
{
    int count = 0;
    if (tiles[x - 1][y - 1].value == 0) count++;
    if (tiles[x][y - 1].value == 0) count++;
    if (tiles[x + 1][y - 1].value == 0) count++;
    if (tiles[x - 1][y].value == 0) count++;
    if (tiles[x + 1][y].value == 0) count++;
    if (tiles[x - 1][y + 1].value == 0) count++;
    if (tiles[x][y + 1].value == 0) count++;
    if (tiles[x + 1][y + 1].value == 0) count++;

    return count;
}

void GenBombs()
{
    for (int i = 0; i < BOMBS; i++)
    {
        int x = rand() % TILES_WIDTH;
        int y = rand() % TILES_HEIGHT;
        tiles[x][y].value = 0;
    }
}

int main(void)
{
    // Raylib window init
    const int screenWidth = 600;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Minesweeper");
    srand(time(NULL));

    // Game varibles
    GameScreen screen = LOGO;
    int framecounter = 0;

    // init tiles
    int xLength = screenWidth / TILES_WIDTH;
    int yLength = screenHeight / TILES_HEIGHT;

    // Calc tile position and set values to 9
    for (int x = 0; x < TILES_WIDTH; x++)
    {
        for (int y = 0; y < TILES_HEIGHT; y++)
        {
            tiles[x][y].pos = (Vector2) { x * xLength, y * yLength };
            tiles[x][y].size = (Vector2) { xLength, yLength };
            tiles[x][y].visible = true;
            tiles[x][y].value = 9;
        }
    }
    // Generate the bombs
    GenBombs();
    // Get the surrounding bombs
    for (int x = 0; x < TILES_WIDTH; x++)
    {
        for (int y = 0; y < TILES_HEIGHT; y++)
        {
            int count = SurroundingBombs(x, y);
            if (count != 0)
            {
                tiles[x][y].value = count;
            }
            else { tiles[x][y].value = 9; }
        }
        
    }
    

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        // Update
        switch (screen)
        {
            // Our logo screen
            case LOGO:
            {
                framecounter++;
                if (framecounter > 180) // 3 seconds
                {
                    screen = TITLE;
                    framecounter = 0;
                }
            } break;

            // Title screen
            case TITLE:
            {
                framecounter++;
                if (IsKeyPressed(KEY_ENTER)) { screen = GAMEPLAY; }
            } break;

            case GAMEPLAY:
            {
                // Set the tile to visible if the mouse is over it 
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    Vector2 mousePos = GetMousePosition();
                    for (int x = 0; x < TILES_WIDTH; x++)
                    {
                        for (int y = 0; y < TILES_HEIGHT; y++)
                        {
                            if (CheckCollisionPointRec(mousePos, (Rectangle) { tiles[x][y].pos.x, tiles[x][y].pos.y, tiles[x][y].size.x, tiles[x][y].size.y }))
                            {
                                tiles[x][y].visible = !tiles[x][y].visible;
                            }
                        }
                    }
                    
                }
            } break;

            default: break;
        }
        // Draw
        BeginDrawing();
            if (screen != GAMEPLAY) { ClearBackground(BLACK); }
            else { ClearBackground(GRAY); }
            
            switch (screen)
            {
                case LOGO:
                {
                    DrawText("FireWire.dev", 20, 20, 20, LIGHTGRAY);
                } break;

                case TITLE:
                {
                    DrawText("MINESWEEPER", GetScreenWidth()/2 - MeasureText("MINESWEEPER", 20), GetScreenHeight()/2, 40, GRAY);
                    if ((framecounter/30)%2 == 0) DrawText("PRESS [ENTER] to START", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] to START", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
                } break;

                case GAMEPLAY:
                {
                    for (int x = 0; x < TILES_WIDTH; x++)
                    {
                        for (int y = 0; y < TILES_HEIGHT; y++)
                        {
                            if (tiles[x][y].visible)
                            {
                                DrawRectangle(tiles[x][y].pos.x, tiles[x][y].pos.y, tiles[x][y].size.x, tiles[x][y].size.y, WHITE);
                                if (tiles[x][y].value != 10)
                                {
                                    DrawText(TextFormat("%d", tiles[x][y].value),
                                    tiles[x][y].pos.x + tiles[x][y].size.x / 2 - (MeasureText(TextFormat("%d", tiles[x][y].value), 20) / 2),
                                    tiles[x][y].pos.y + tiles[x][y].size.y / 2, 20, BLACK);
                                }
                            }
                            DrawRectangleLines(tiles[x][y].pos.x, tiles[x][y].pos.y, tiles[x][y].size.x, tiles[x][y].size.y, DARKGRAY);
                        }
                    }
                    
                } break;

                default: break;
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}