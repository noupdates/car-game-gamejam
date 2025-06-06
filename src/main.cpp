#include <raylib.h>

bool paused = false;
int car_passed = 0;
int level = 1;
int best_level = 0;
int wheelCounter = 0;
int carsOnScreen = 0;

class Car {
protected:
    void LimitMovement() {
        if (x <= 120) 
        {
            x = 120;
        }  
        if (x + width >= 600) 
        {
            x = 600 - width;
        }
    }

public:
    float x,y;
    int width = 120, height = 160;
    int speed;
    float block = 40;
    Color color;
    Color wheel = Color{34, 32, 28, 200};
    Color linesWhell = Color{191, 191, 191, 200};
    bool rotateWheel = true;
    float offsetWheel = 3*block-5;
    int middleEnemyOffset = 0;
    Texture2D texture;

    void Draw() {
        DrawTexture(texture, x, y, WHITE);
    }

    void Update() {
        if (IsKeyPressed(KEY_RIGHT)) 
        {
            x = x + 180;
        }
        if (IsKeyPressed(KEY_LEFT)) 
        {
            x = x - 180;
        }

        if (wheelCounter == 4) {
            if (rotateWheel) {
                offsetWheel = offsetWheel-9;
            } else {
                offsetWheel = offsetWheel+9;
            }
            rotateWheel = !rotateWheel;
            wheelCounter = 0;
        } 
        ++wheelCounter;
        
        LimitMovement();
    }
};

class EnemyCar : public Car {
public:
    void Update() {
        y += speed + level;

        if(y > GetScreenHeight()+height) {
            ++car_passed;
            if (car_passed >= 10 && car_passed%10==0) ++level;
            ++carsOnScreen;
            if (carsOnScreen >= 2) {
                y = GetRandomValue(-2400,-1600);
                carsOnScreen = 0;
            } else {
                y = GetRandomValue(-1600,0);
            }
            
        }

        if (wheelCounter == 4) {
            if (rotateWheel) {
                offsetWheel = offsetWheel-9;
            } else {
                offsetWheel = offsetWheel+9;
            }
            rotateWheel = !rotateWheel;
        } 
    }

    void ResetEnemy() {
        y = GetRandomValue(-1600,0);
        speed = speed;
    }
};

class StreetLine {
public:
    int x,y;
    float width = 20, height = 160;
    int speed = 10;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
        DrawRectangle(x, y+height, width, height, GRAY);
        DrawRectangle(x+180, y, width, height, WHITE);
        DrawRectangle(x+180, y+height, width, height, GRAY);
    }

    void Update() {
        y += speed;

        if(y > GetScreenHeight()) {
            y = 0 - height;
        }
    }
};

class StreetBorder {
public:
    int x,y;
    int width = 30, height = 20;
    int speed = 10;

    void Draw() {
        for (int i =0; i < 40; i++) {
            DrawRectangle(x,y+0+(i*40), 30, 20, RED);
            DrawRectangle(x, y+20+(i*40), 30, 20, WHITE);     
        }
    }

    void Update() {
        y += speed;

        if(y+800 > GetScreenHeight()) {
            y = -800;
        }
        
    }
};

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screen_width = 1200;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "Car Game");
    SetTargetFPS(60);                // Set our game to run at 60 frames-per-second

    InitAudioDevice();              // Initialize audio device
    

    // Load Assets

    Music music = LoadMusicStream("assets/music/racing_tune.mp3");
    Sound kaboooom = LoadSound("assets/music/kaboooom.mp3");
    PlayMusicStream(music);

    Image playerImg = LoadImage("assets/img/player.png"); 
    Texture2D playerTexture = LoadTextureFromImage(playerImg);   
    UnloadImage(playerImg); 

    Image enemyImg = LoadImage("assets/img/enemy.png"); 
    Texture2D enemyTexture = LoadTextureFromImage(enemyImg);   
    UnloadImage(enemyImg); 

    Image explosionImg = LoadImage("assets/img/explosion.png"); 
    Texture2D explosionTexture = LoadTextureFromImage(explosionImg);   
    UnloadImage(explosionImg); 

    // Set objects
    //--------------------------------------------------------------------------------------
    Color grass = Color{16, 201, 16, 1};
    Car player;
    EnemyCar enemyL;
    EnemyCar enemyM;
    EnemyCar enemyR;
    StreetLine streetlineA;
    StreetLine streetlineB;
    StreetLine streetlineC;
    StreetBorder streetBorderL;
    StreetBorder streetBorderR;

    player.x = 120;
    player.y = 630;
    player.color = RED;
    player.texture = playerTexture;

    enemyL.color = BLUE;
    enemyL.speed = 4;
    enemyL.x = 120;
    enemyL.y = GetRandomValue(-1400,0);
    enemyL.texture = enemyTexture;

    enemyM.color = GREEN;
    enemyM.speed = 4;
    enemyM.x = 300;
    enemyM.y = GetRandomValue(-2400,1600);
    enemyM.texture = enemyTexture;

    enemyR.color = YELLOW;
    enemyR.speed = 4;
    enemyR.x = 480;
    enemyR.y = GetRandomValue(-1400,0);
    enemyR.texture = enemyTexture;

    streetlineA.x = 260;
    streetlineA.y = 0;
    streetlineB.x = 260;
    streetlineB.y = 320;
    streetlineC.x = 260;
    streetlineC.y = 640;

    streetBorderL.x = 70;
    streetBorderL.y = -800;
    streetBorderR.x = 620;
    streetBorderR.y = -800;

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())                    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        if(!paused) {
            PlaySound(kaboooom);
            player.Update();
            enemyL.Update();
            enemyM.Update();
            enemyR.Update();
            streetlineA.Update();
            streetlineB.Update();
            streetlineC.Update();
            streetBorderL.Update();
            streetBorderR.Update();
        }

        UpdateMusicStream(music);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(grass);
            DrawText(TextFormat("Level:              %i", level), 740, 100, 30, WHITE);
            DrawText(TextFormat("Car passed:     %i", car_passed), 740, 200, 30, WHITE);
            DrawText(TextFormat("Best Level:     %i", best_level), 740, 300, 30, WHITE);
            
            DrawRectangle(100, 0, 520, screen_height, GRAY);

            streetBorderL.Draw();

            streetlineA.Draw();
            streetlineB.Draw();  
            streetlineC.Draw();

            streetBorderR.Draw();

            player.Draw();
            enemyL.Draw();
            enemyM.Draw();
            enemyR.Draw();

        if (CheckCollisionRecs(Rectangle{(float)player.x, (float)player.y, 120, 160}, Rectangle{(float)enemyL.x, (float)enemyL.y, 120,160}) || CheckCollisionRecs(Rectangle{(float)player.x, (float)player.y, 120, 160}, Rectangle{(float)enemyM.x, (float)enemyM.y, 120,160}) || CheckCollisionRecs(Rectangle{(float)player.x, (float)player.y, 120, 160}, Rectangle{(float)enemyR.x, (float)enemyR.y, 120,160}))
        {   
            
            DrawText("Game Over!", 840, 400, 30, WHITE);
            DrawText("Press SPACE to start a new round", 740, 500, 20, WHITE);
               
            paused = true;
            if (IsKeyPressed(KEY_SPACE)) 
            {
                if (level > best_level) best_level = level;
                paused = false;
                enemyL.ResetEnemy();
                enemyM.ResetEnemy();
                enemyR.ResetEnemy();
                level = 1;
                car_passed = 0;
                SeekMusicStream(music, 0.0f);
            } 
            DrawTexture(explosionTexture, player.x, player.y, WHITE);
        }

        EndDrawing();
        
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    UnloadSound(kaboooom);
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(explosionTexture);

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}