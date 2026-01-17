#pragma once

#include <string>

namespace ApplesGame
{
    // ѕуть к ресурсам
    const std::string RESOURCES_PATH = "Resources/";

    // –азмеры объектов
    const float PLAYER_SIZE = 20.f;
    const float APPLE_SIZE = 20.f;
    const float OBSTACLE_SIZE = 25.f;

    // ƒвижение
    const float INITIAL_SPEED = 100.f;
    const float ACCELERATION = 10.f;   // базовое ускорение за €блоко

    // Ёкран
    const unsigned int SCREEN_WIDTH = 800;
    const unsigned int SCREEN_HEGHT = 600;
   
    // ћаксимальное число преп€тствий (камней)
    const int MAX_OBSTACLES = 50;

    // Ѕазовое число преп€тствий на старте
    const int BASE_NUM_OBSTACLES = 5;

    // ћаксимальное число €блок
    const int MAX_APPLES = 100;

    // ===== Ѕитовыми флаги режимов =====
    // тип игры по €блокам
    const unsigned int MODE_APPLES_FINITE = 1u << 0; // конечное число €блок
    const unsigned int MODE_APPLES_INFINITE = 1u << 1; // бесконечные €блоки

    // поведение скорости
    const unsigned int MODE_SPEED_ACCEL_ON = 1u << 2; // есть ускорение
    const unsigned int MODE_SPEED_ACCEL_OFF = 1u << 3; // без ускорени€

    // ===== √отовые пресеты (комбинации флагов) =====
    // 1) 20 €блок, с изменЄнной скоростью
    const unsigned int MODE_PRESET_20_ACCEL =
        MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_ON;

    // 2) 20 €блок, без изменЄнной скорости
    const unsigned int MODE_PRESET_20_NOACCEL =
        MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_OFF;

    // 3) 50 €блок, с изменЄнной скоростью
    const unsigned int MODE_PRESET_50_ACCEL =
        MODE_APPLES_INFINITE | MODE_SPEED_ACCEL_ON;

    // ƒополнительные примеры дл€ конечных режимов:
    const unsigned int MODE_PRESET_20_FINITE_ACCEL =
        MODE_APPLES_FINITE | MODE_SPEED_ACCEL_ON;

    const unsigned int MODE_PRESET_50_FINITE_NOACCEL =
        MODE_APPLES_FINITE | MODE_SPEED_ACCEL_OFF;
}
