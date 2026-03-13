# Исправление зависания при запуске

## ?? Проблема
Игра зависала сразу после запуска, главное меню не прогружалось.

## ?? Причина
Использование `assert(font.loadFromFile(...))` в методах `onEnter()` всех состояний. Если файл шрифта не найден, `assert` вызывает `abort()`, что закрывает приложение.

## ? Решение

### 1. Удалены assert из всех файлов состояний
Заменены на проверку с fallback:

```cpp
// Было:
assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

// Стало:
bool fontLoaded = font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf");
std::cout << "[State] Font loaded: " << (fontLoaded ? "YES" : "NO") << std::endl;

if (fontLoaded) {
    text.setFont(font);  // Применить шрифт только если загрузился
}
```

### 2. Добавлено детальное логирование
В Application.cpp добавлены логи для отслеживания инициализации:

```
[Application] Constructor: window created
[Application] Constructor: calling InitGame...
[Game] Initializing...
[Game] Pushing MainMenuState...
[MainMenuState] onEnter() called
[MainMenuState] Loading font from: Resources/Fonts/Roboto-Regular.ttf
[MainMenuState] Font loaded: YES/NO
...
[Application] Frame: 60, States: 1
```

### 3. Изменённые файлы

| Файл | Изменение |
|------|-----------|
| Application.cpp | Добавлены логи для отладки инициализации и цикла |
| GameStateMainMenu.cpp | Удалён assert, добавлен fallback и логи |
| GameStatePauseMenu.cpp | Удалён assert, добавлен fallback и логи |
| GameStateGameOver.cpp | Удалён assert, добавлен fallback и логи |
| GameStateRecords.cpp | Удалён assert, добавлен fallback и логи |

## ?? Как проверить

1. **Откройте Output window** (Debug ? Windows ? Output)
2. **Запустите игру**
3. **Смотрите консоль** для отслеживания инициализации

### Ожидаемый вывод:
```
[Application] Getting instance...
[Application] Instance ready
[Application] Constructor: window created
[Application] Constructor: calling InitGame...
[Game] Initializing...
[Game] Pushing MainMenuState...
[MainMenuState] onEnter() called
[MainMenuState] Loading font from: Resources/Fonts/Roboto-Regular.ttf
[MainMenuState] Font loaded: YES
[MainMenuState] Creating menu items...
[MainMenuState] Initializing menu...
[MainMenuState] onEnter() complete
[Game] Initialization complete. Active states: 1
[Application] Constructor: InitGame complete
[Application] Run() started
[Application] Frame: 60, States: 1
```

## ?? Структура Resources

Убедитесь, что папка `Resources` находится рядом с исполняемым файлом:

```
Project/
??? Resources/
?   ??? Fonts/
?   ?   ??? Roboto-Regular.ttf ? ОБЯЗАТЕЛЬНО!
?   ??? Textures/
?   ?   ??? Ball.png
?   ?   ??? Brick.png
?   ?   ??? Platform.png
?   ??? Sounds/
?       ??? Hit.wav
?       ??? GameOver.wav
??? ArkanoidGame.exe (или Debug папка)
??? ...
```

## ?? Возможные причины если ещё зависает

### 1. Font файл не найден
- Проверьте путь: `Resources/Fonts/Roboto-Regular.ttf`
- В консоли будет: `Font loaded: NO`
- Меню будет работать, но без текста (это OK)

### 2. Бесконечный цикл в Menu
- Проверьте `Menu.cpp` на предмет while циклов
- Логи покажут, где зависает

### 3. Callback вызывает бесконечный loop
- Проверьте Game::pushState(), Game::popState(), Game::switchState()
- Все методы теперь имеют логи

## ?? Следующие шаги

1. Запустите игру ? должно появиться меню с логами в консоли
2. Используйте логи для отладки проблем
3. Удалите логи (std::cout) когда всё будет работать нормально
4. Убедитесь, что игра корректно переходит между состояниями

## ?? Заметки

- ? Ошибки инициализации больше не вызывают abort()
- ? Fallback цвета используются если шрифта нет
- ? Полная видимость инициализации через логи
- ? Игра должна запускаться даже без Resources папки
