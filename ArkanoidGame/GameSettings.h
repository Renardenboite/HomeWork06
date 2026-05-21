#pragma once
#include <string>

namespace SnakeGame
{
	// Resources path
	const std::string RESOURCES_PATH = "Resources/";
	const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
	const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
	const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";

	// Game settings constants
	const float INITIAL_SPEED = 200.f;	
	const float PLATFORM_SPEED = 200.f;
	const float PLATFORM_WIGTH = 80.f;
	const float PLATFORM_HEIGHT = 20.f;
		
	const float BLOCK_WIDTH = 80.f;
	const float BLOCK_HEIGHT = 30.f;
	const int COLS = 7;
	const int ROWS = 3;
	const int BLOCKS_COUNT = COLS * ROWS;
	const float startX = BLOCK_WIDTH + 50.f;
	const float startY = BLOCK_HEIGHT + 50.f;
	const float blockSpacingX = 15.f;
	const float blockSpacingY= 10.f;

	const float BALL_SIZE = 20.f;
	const float ACCELERATION = 200.f;
	const int MAX_APPLES = 80;
	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;
	const float TIME_PER_FRAME = 1.f / 60.f; // 60 fps	

	const int MAX_RECORDS_TABLE_SIZE = 5;
	extern const char* PLAYER_NAME; // We need to define this constant in some CPP

	const std::string GAME_NAME = "ArkanoidGame";
}
