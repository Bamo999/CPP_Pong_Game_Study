// PongGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{
    38,
    185,
    154,
    255
};

Color Dark_Green = Color{
    20,
    160,
    133,
    255
};

Color Light_Green = Color{
    129,
    204,
    133,
    255
};

Color Yellow = Color{
    243,
    213,
    91,
    255
};

int player1_score = 0;
int player2_score = 0;

class Ball {
public:
    float x = 0, y = 0;
    int speed_x = 7, speed_y = 7;
    int radius = 20;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    };

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        };

        // Computer wins
        if (x + radius >= GetScreenWidth()) {
            player2_score++;
            Reset();
        };

        // Player wins
        if (x - radius <= 0) {
            player1_score++;
            Reset();
        };
    }

    void Reset() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_dir[2] = { -1, 1 };
        speed_x *= speed_dir[GetRandomValue(0, 1)];
        speed_y *= speed_dir[GetRandomValue(0, 1)];
    }
};

class Paddle {

protected:

    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }

        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:

    float x = 0, y = 0;
    float width = 25, height = 120;
    int speed = 7;

    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
    };

    void Update() {

        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }

        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }

        LimitMovement();
    };
};

class CPUPaddle: public Paddle {

public:

    void Update(int ball_y) {

        if (y + height / 2 > ball_y) {
            y -= speed;
        }

        if (y + height / 2 <= ball_y) {
            y += speed;
        }

        LimitMovement();
    }

};

int main()
{
    cout << "This is my first C++ Project ever. A pong game with Raylib! Cool right?" << endl;

    const int screen_width = 1280, screen_height = 800;
    InitWindow(screen_width, screen_height, "Bamo's Pong Game");

    const int FPS = 60;
    SetTargetFPS(FPS);

    int side_padding_pixel = 10;

    Ball pong_ball;

    pong_ball.x = screen_width / 2;
    pong_ball.y = screen_height / 2;

    Paddle player1;
    player1.x = screen_width - player1.width - side_padding_pixel;
    player1.y = (screen_height / 2) - (player1.height / 2);

    CPUPaddle player2;
    player2.x = side_padding_pixel;
    player2.y = (screen_height / 2) - (player2.height / 2);

    while (WindowShouldClose() == false ) {

        BeginDrawing();
        
        // Updates
        pong_ball.Update();
        player1.Update();
        player2.Update(pong_ball.y);

        // Ball on paddle collision checks
        if (CheckCollisionCircleRec( Vector2{pong_ball.x, pong_ball.y}, pong_ball.radius, Rectangle{player1.x, player1.y, player1.width, player1.height} )) 
        {
            //cout << "Hitted";
            pong_ball.speed_x *= -1;
        };

        if (CheckCollisionCircleRec(Vector2{pong_ball.x, pong_ball.y }, pong_ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.height }))
        {
            pong_ball.speed_x *= -1;
        };

        // Background clear
        ClearBackground(Dark_Green);

        // Ball draw
        pong_ball.Draw();

        // Player draws
        player1.Draw();
        player2.Draw();

        // Playspace draws
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);

        DrawText(TextFormat("%i", player1_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player2_score), screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
