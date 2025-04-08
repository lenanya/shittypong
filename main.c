#include <stdio.h>
#include "raylib.h"
#include <stdbool.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define W_TITLE "Shitty Pong"

#define PLAYER_WIDTH W_WIDTH / 25
#define PLAYER_HEIGHT W_HEIGHT / 4
#define PLAYER_SPEED 10.0f

#define BALL_WIDTH W_WIDTH / 20
#define BALL_HEIGHT BALL_WIDTH
#define BALL_INITIAL_VELOCITY (Vector2) {.x = 5.0f, .y = 3.0f}

#define BG_COLOR (Color) {0x15, 0x15, 0x15, 0xFF}
#define PLAYER_COLOR (Color) {0xFF, 0xFF, 0xFF, 0xFF}
#define BALL_COLOR (Color) {0x0, 0xFF, 0x0, 0xFF}

typedef struct Player {
    Vector2 position;
    Vector2 size;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
} Ball;

bool ball_collide_player(Ball* b, Player* p) {
    return CheckCollisionRecs(
        (Rectangle) {
            b->position.x, 
            b->position.y, 
            b->size.x, 
            b->size.y},
        (Rectangle) {
            p->position.x,
            p->position.y,
            p->size.x,
            p->size.y,
        }
    );
}

void draw_player(Player* p) {
    DrawRectangleV(p->position, p->size, PLAYER_COLOR);
}

void draw_ball(Ball* b) {
    DrawRectangleV(b->position, b->size, BALL_COLOR);
}

void move_ball(Ball* b, Player* p1, Player* p2) {
    if (ball_collide_player(b, p1)) {
        b->position.x = p1->position.x + p1->size.x;
        b->velocity.x *= -1;
    }

    if (ball_collide_player(b, p2)) {
        b->position.x = p2->position.x - b->size.x - 1;
        b->velocity.x *= -1;
    }

    if (b->position.y + b->size.y + b->velocity.y > W_HEIGHT) {
        b->position.y = W_HEIGHT - b->size.y;
        b->velocity.y *= -1;
    } else {
        b->position.y += b->velocity.y;
    }
    if (b->position.x + b->size.x + b->velocity.x > W_WIDTH) {
        b->position.x = W_WIDTH - b->size.x;
        b->velocity.x *= -1;
    } else {
        b->position.x += b->velocity.x;
    }
    if (b->position.y + b->velocity.y < 0) {
        b->position.y = 0;
        b->velocity.y *= -1;
    } else {
        b->position.y += b->velocity.y;
    }
    if (b->position.x + b->velocity.x < 0) {
        b->position.x = 0;
        b->velocity.x *= -1;
    } else {
        b->position.x += b->velocity.x;
    }
}

void move_player(Player* p) {
    if (IsKeyDown(KEY_W)) {
        p->position.y -= PLAYER_SPEED;
        if (p->position.y < 0) p->position.y = 0;
    } else if (IsKeyDown(KEY_S)) {
        p->position.y += PLAYER_SPEED;
        if (p->position.y + p->size.y > W_HEIGHT) p->position.y = W_HEIGHT - p->size.y;
    }
}

void move_ai(Player* p, Ball* b) {
    if (b->position.y < p->position.y) {
        p->position.y -= PLAYER_SPEED;
        if (p->position.y < 0) p->position.y = 0;
    } else if (b->position.y > p->position.y) {
        p->position.y += PLAYER_SPEED;
        if (p->position.y + p->size.y > W_HEIGHT) p->position.y = W_HEIGHT - p->size.y;
    }
}

int main(void) {
    InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);

    Player p1 = {
        .position = {
            .x = 50.0f,
            .y = (W_HEIGHT / 2) - (PLAYER_HEIGHT / 2),
        },
        .size = (Vector2) {
            .x = PLAYER_WIDTH,
            .y = PLAYER_HEIGHT,
        }
    };

    Player p2 = {
        .position = {
            .x = 700.0f,
            .y = (W_HEIGHT / 2) - (PLAYER_HEIGHT / 2),
        },
        .size = (Vector2) {
            .x = PLAYER_WIDTH,
            .y = PLAYER_HEIGHT,
        }
    };

    Ball ball = {
        .position = {
            .x = W_WIDTH / 2 - BALL_WIDTH / 2,
            .y = W_HEIGHT /2 - BALL_HEIGHT / 2,
        },
        .size = {
            .x = BALL_WIDTH,
            .y = BALL_HEIGHT,
        },
        .velocity = BALL_INITIAL_VELOCITY,
    };

    bool gameRunning = true;

    SetTargetFPS(60);

    while (!WindowShouldClose() && gameRunning) {
        BeginDrawing();
        ClearBackground(BG_COLOR);
        
        move_ball(&ball, &p1, &p2);
        move_ai(&p2, &ball);
        move_player(&p1);

        draw_player(&p1);
        draw_player(&p2);
        draw_ball(&ball);

        EndDrawing();
    }

    return 0;
}