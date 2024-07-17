#include <iostream>
#include <sstream>
#include <cmath>

#include <raylib.h>

#include "include\Player.h"
#include "include\Food.h"

int gameSpeed = INITIAL_GAME_SPEED;
bool shouldGameSpeedChange = false;

unsigned char map[ROWS][COLS] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

void InitGame(Player& player, Food& food) {

	gameSpeed = INITIAL_GAME_SPEED;

	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			map[i][j] = 0;
		}
	}

	player.body[0].x = 7;
	player.body[0].y = 2;
	player.score = 0;
	player.actualSize = 1;
	player.isMoving = false;
	player.direction = PlayerDirection::NONE;
	map[player.body[0].y][player.body[0].x] = 1;

	food.pos.x = 3;
	food.pos.y = 5;
	map[food.pos.y][food.pos.x] = 2;
}

VecIndex GetRandomEmptyVec(unsigned char gameMap[ROWS][COLS]) {
	std::vector<VecIndex> availableVecs;

	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			if (gameMap[i][j] == 0) {
				availableVecs.push_back({ j, i });
			}
		}
	}

	int randIndex = GetRandomValue(0, availableVecs.size()-1);
	return availableVecs[randIndex];
}

void CheckForCollisionWithFood(Player& player, Food& food) {
	if (player.body[0].y == food.pos.y && player.body[0].x == food.pos.x) {
		player.score++;

		if (player.score != 0 && (player.score % 10) == 0) {
			shouldGameSpeedChange = true;
		}

		food.pos = GetRandomEmptyVec(map);
		map[food.pos.y][food.pos.x] = 2;
		
		if (player.actualSize < MAX_SNAKE_SIZE) {
			player.actualSize++;
		}
	}
}

bool IsPlayerCollidingWithItself(const Player& player) {
	for (int i = 1; i < player.actualSize; ++i) {
		if (player.body[0].x == player.body[i].x && player.body[0].y == player.body[i].y) {
			return true;
		}
	}

	return false;
}

void RenderTextONScreen(std::string txt, int screenW, int screenH) {
	int txtW = MeasureText(txt.c_str(), 26);
	DrawText(txt.c_str(), (screenW - txtW) / 2, 20, 26, BLACK);
}

void MovePlayer(Player& player, Food& food, unsigned char map[ROWS][COLS]) {
	switch (player.direction) {
	case PlayerDirection::UP:
		if (player.body[0].y > 0 && !IsPlayerCollidingWithItself(player)) {
			VecIndex prevPos = { 0 };
			for (int i = 0; i < player.actualSize; ++i) {
				if (i == 0) {
					prevPos.x = player.body[i].x;
					prevPos.y = player.body[i].y;
					player.body[i].y--;
					map[prevPos.y][prevPos.x] = 0;
				}
				else if (i == (player.actualSize - 1)) {
					map[player.body[i].y][player.body[i].x] = 0;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
				}
				else {
					VecIndex temp;
					temp.x = player.body[i].x;
					temp.y = player.body[i].y;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
					prevPos.x = temp.x;
					prevPos.y = temp.y;
				}
				CheckForCollisionWithFood(player, food);
				map[player.body[i].y][player.body[i].x] = 1;
			}
		}
		else {
			InitGame(player, food);
		}
		break;
	case PlayerDirection::DOWN:
		if (player.body[0].y < ROWS - 1 && !IsPlayerCollidingWithItself(player)) {
			VecIndex prevPos = { 0 };
			for (int i = 0; i < player.actualSize; ++i) {
				if (i == 0) {
					prevPos.x = player.body[i].x;
					prevPos.y = player.body[i].y;
					player.body[i].y++;
					map[prevPos.y][prevPos.x] = 0;
				}
				else if (i == (player.actualSize - 1)) {
					map[player.body[i].y][player.body[i].x] = 0;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
				}
				else {
					VecIndex temp;
					temp.x = player.body[i].x;
					temp.y = player.body[i].y;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
					prevPos.x = temp.x;
					prevPos.y = temp.y;
				}
				CheckForCollisionWithFood(player, food);
				map[player.body[i].y][player.body[i].x] = 1;
			}
		}
		else {
			InitGame(player, food);
		}
		break;
	case PlayerDirection::LEFT:
		if (player.body[0].x > 0 && !IsPlayerCollidingWithItself(player)) {
			VecIndex prevPos = { 0 };
			for (int i = 0; i < player.actualSize; ++i) {
				if (i == 0) {
					prevPos.x = player.body[i].x;
					prevPos.y = player.body[i].y;
					player.body[i].x--;
					map[prevPos.y][prevPos.x] = 0;
				}
				else if (i == (player.actualSize - 1)) {
					map[player.body[i].y][player.body[i].x] = 0;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
				}
				else {
					VecIndex temp;
					temp.x = player.body[i].x;
					temp.y = player.body[i].y;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
					prevPos.x = temp.x;
					prevPos.y = temp.y;
				}
				CheckForCollisionWithFood(player, food);
				map[player.body[i].y][player.body[i].x] = 1;
			}
		}
		else {
			InitGame(player, food);
		}
		break;
	case PlayerDirection::RIGHT:
		if (player.body[0].x < COLS - 1 && !IsPlayerCollidingWithItself(player)) {
			VecIndex prevPos = { 0 };
			for (int i = 0; i < player.actualSize; ++i) {
				if (i == 0) {
					prevPos.x = player.body[i].x;
					prevPos.y = player.body[i].y;
					player.body[i].x++;
					map[prevPos.y][prevPos.x] = 0;
				}
				else if (i == (player.actualSize-1)) {
					map[player.body[i].y][player.body[i].x] = 0;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
				}
				else {
					VecIndex temp;
					temp.x = player.body[i].x;
					temp.y = player.body[i].y;
					player.body[i].x = prevPos.x;
					player.body[i].y = prevPos.y;
					prevPos.x = temp.x;
					prevPos.y = temp.y;
				}
				CheckForCollisionWithFood(player, food);
				map[player.body[i].y][player.body[i].x] = 1;
			}
		}
		else {
			InitGame(player, food);
		}
		break;
	}
}

int main() {
	Player player = { 0 };
	Food food = { 0 };
	int timer = 0;

	InitGame(player, food);

	const int screenWidth = 1280;
	const int screenHeight = 680;

	const int gridWidth = COLS * GRID_SQR_WIDTH;
	const int gridHeight = ROWS * GRID_SQR_HEIGHT;

	int gridStartPositionX = (screenWidth - gridWidth) / 2;
	int gridStartPositionY = (screenHeight - gridHeight) / 2;

	InitWindow(screenWidth, screenHeight, "Snake Improved");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(ORANGE);

		std::stringstream strStream;
		strStream << "Score " << player.score;
		RenderTextONScreen(strStream.str(), screenWidth, screenHeight);

		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				if (map[i][j] == 0) {
					DrawRectangleLines(gridStartPositionX + j * GRID_SQR_WIDTH, gridStartPositionY + i * GRID_SQR_HEIGHT, GRID_SQR_WIDTH, GRID_SQR_HEIGHT, BLACK);
				}
				else if (map[i][j] == 1) {
					DrawRectangle(gridStartPositionX + j * GRID_SQR_WIDTH, gridStartPositionY + i * GRID_SQR_HEIGHT, GRID_SQR_WIDTH, GRID_SQR_HEIGHT, BLACK);
				}
				else {
					DrawRectangle(gridStartPositionX + j * GRID_SQR_WIDTH, gridStartPositionY + i * GRID_SQR_HEIGHT, GRID_SQR_WIDTH, GRID_SQR_HEIGHT, RED);
				}
			}
		}

		if (IsKeyPressed(KEY_RIGHT) && player.direction != PlayerDirection::LEFT) {
			player.direction = PlayerDirection::RIGHT;
			player.isMoving = true;
		}

		if (IsKeyPressed(KEY_LEFT) && player.direction != PlayerDirection::RIGHT) {
			player.direction = PlayerDirection::LEFT;
			player.isMoving = true;
		}

		if (IsKeyPressed(KEY_UP) && player.direction != PlayerDirection::DOWN) {
			player.direction = PlayerDirection::UP;
			player.isMoving = true;
		}

		if (IsKeyPressed(KEY_DOWN) && player.direction != PlayerDirection::UP) {
			player.direction = PlayerDirection::DOWN;
			player.isMoving = true;
		}

		if (player.isMoving) {
			if ((timer % gameSpeed) == 0) {
				MovePlayer(player, food, map);
			}
		}

		EndDrawing();

		if (timer < INT_MAX) {
			++timer;
		}
		else {
			timer = 0;
		}

		if (shouldGameSpeedChange) {
			if (gameSpeed > 3) {
				gameSpeed -= 2;
				shouldGameSpeedChange = false;
			}
		}
	}

	CloseWindow();
}