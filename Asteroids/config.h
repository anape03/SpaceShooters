#pragma once

#define ASSETS_PATH "assets\\"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

#define CANVAS_WIDTH 1200
#define CANVAS_HEIGHT 700

#define SHIP_DIM_X 35
#define SHIP_DIM_Y 45

#define MIN_ASTEROIDS 20 // minimum number of asteroids on screen

#define HIGH_SCORES_FILE "high scores.txt" // file containing high scores
#define NUM_TOP_SCORES 10 // number of top high scores

#define FIRE_COOLDOWN 350 // fire cooldown for ship bullets
#define TYPING_COOLDOWN 100 // typing cooldown for end screen
#define SHIP_COOLDOWN 1500 // cooldown when it first appears (begining or after it dies) during which it can't be hit
#define ENDSCREEN_DELAY 500 // delay for endscreen to appear

#define APPEAR_EVERY 300 // (for orb) appear once every ___ in score
#define DIF_CHANGE_EVERY 500 // (for asteroids) up the speed every ___ in score