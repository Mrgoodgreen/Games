#pragma once
#include <string>

namespace ArkanoidGame
{
    // Resources path
    const std::string RESOURCES_PATH = "Resources/";
    const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
    const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
    const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";

    // Game settings constants
    const float PLATFORM_WIDTH = 100.0f;
    const float PLATFORM_HEIGHT = 20.0f;
    const float BALL_RADIUS = 10.0f;
    const float INITIAL_BALL_SPEED = 1000.0f;
    const float PLATFORM_SPEED = 300.0f;

    const unsigned int BRICK_WIDTH = 80;
    const unsigned int BRICK_HEIGHT = 30;
    const unsigned int BRICK_ROWS = 5;
    const unsigned int BRICK_COLUMNS = 10;

    const unsigned int SCREEN_WIDTH = 800;
    const unsigned int SCREEN_HEIGHT = 600;
    const float TIME_PER_FRAME = 1.0f / 60.0f; // 60 fps

    const int MAX_RECORDS_TABLE_SIZE = 5;
    extern const char* PLAYER_NAME;

    const std::string GAME_NAME = "ArkanoidGame";
}