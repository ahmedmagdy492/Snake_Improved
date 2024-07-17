#pragma once

#include "Shared.h"

struct Player {
	VecIndex body[MAX_SNAKE_SIZE];
	size_t actualSize = 1;
	unsigned short score;
	bool isMoving;
	PlayerDirection direction;
};