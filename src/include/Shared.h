#pragma once

#include <vector>

#define ROWS 10
#define COLS 24
#define GRID_SQR_WIDTH 50
#define GRID_SQR_HEIGHT 50
#define MAX_SNAKE_SIZE 20
#define INITIAL_GAME_SPEED 10

enum PlayerDirection {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct VecIndex {
	int x;
	int y;
};