# Исправления системы переходов между состояниями

## ? Проблемы, которые были исправлены

### 1. **Главное меню не отображалось**
- **Проблема**: InitGame() сразу запускал PlayingState вместо MainMenuState
- **Решение**: Изменено на pushState(std::make_unique<MainMenuState>())

### 2. **ESC не работал для паузы**
- **Проблема**: PlayingState не обрабатывал ESC, нет связи между состояниями
- **Решение**: Добавлена обработка ESC в Game::HandleWindowEvents() - при ESC в PlayingState пушится PauseMenuState

### 3. **Отсутствовали переходы между состояниями**
- **Проблема**: Нет связи между состояниями (меню ? игра, игра ? пауза, пауза ? меню и т.д.)
- **Решение**: Реализованы callbacks в меню для управления переходами

## ?? Полный поток игры

### Инициализация
1. `InitGame()` ? pushState(MainMenuState)
2. MainMenuState::onEnter() ? загрузка ресурсов меню

### Главное меню
- **Start Game** ? switchState(PlayingState) - заменяет меню на игру
- **Options** ? подменю с опциями
- **Records** ? pushState(RecordsState) - добавляет экран рекордов поверх меню
- **Exit Game** ? pushState(подменю с "Вы уверены?")
  - **Yes** ? window.close()
  - **No** ? menu.GoBack()

### Во время игры (PlayingState)
- **ESC** ? Game обнаруживает ESC в PlayingState, пушит PauseMenuState
- **SPACE** ? запуск шара
- **Arrow Keys / Mouse** ? управление платформой

### Пауза (PauseMenuState)
- **Return to game** ? popState() - удаляет паузу, возвращает в игру
- **ESC** ? popState() - то же самое
- **Exit to menu** ? popState() + switchState(MainMenuState)

### Конец игры (GameOverState)
- Автоматически пушится когда:
  - Жизней <= 0
  - Все кирпичи разрушены
- **SPACE** ? switchState(PlayingState) - новая игра
- **ESC** ? switchState(MainMenuState) - в меню

### Records (RecordsState)
- Показывает таблицу рекордов
- **ESC** ? popState() - возвращает в предыдущее состояние (меню)

## ?? Ключевые изменения в файлах

### Game.cpp
```cpp
// Изменение инициализации
void InitGame(Game& game) {
    game.pushState(std::make_unique<MainMenuState>()); // было PlayingState
}

// Новая обработка ESC
if (std::strcmp(currentState->getStateName(), "PlayingState") == 0) {
    game.pushState(std::make_unique<PauseMenuState>());
}
```

### Game.h
```cpp
// Улучшенные методы управления стеком состояний
void pushState(GameStatePtr state);    // Добавить состояние поверх стека
void popState();                       // Удалить верхнее состояние
void switchState(GameStatePtr state);  // Заменить весь стек новым состоянием
```

### IGameState.h
```cpp
virtual const char* getStateName() const { return "UnknownState"; }
```
Используется для идентификации состояния при обработке ESC.

### Все State классы
Реализованы:
- `onEnter()` - инициализация при входе
- `onExit()` - очистка при выходе
- Callback-и в меню для переходов между состояниями
- Консоль-логирование для отладки

## ?? Диаграмма переходов

```
MainMenuState
    ?? ENTER ? PlayingState (switchState)
    ?? Records ? RecordsState (pushState)
    ?? Exit ? close

PlayingState
    ?? ESC ? PauseMenuState (pushState)

PauseMenuState
    ?? Resume ? PlayingState (popState)
    ?? ESC ? PlayingState (popState)
    ?? Exit ? MainMenuState (popState + switchState)

GameOverState
    ?? SPACE ? PlayingState (switchState)
    ?? ESC ? MainMenuState (switchState)

RecordsState
    ?? ESC ? MainMenuState (popState)
```

## ?? Тестирование

1. Запустите игру ? должно появиться главное меню
2. ENTER или вниз/Enter ? Start Game
3. Должна запуститься игра (платформа, шарик, кирпичи)
4. ESC во время игры ? Pause Menu
5. ENTER ? Resume Game (должны вернуться в игру)
6. ESC в паузе ? Return to menu
7. Проиграйте игру ? Game Over screen
8. SPACE ? новая игра
9. ESC ? главное меню

## ?? Консоль-логи для отладки

При запуске в консоли будут видны переходы:
```
[Game] Initializing...
[Game] Pushing MainMenuState...
[MainMenuState] onEnter() called
[Game] Initialization complete. Active states: 1

[Когда нажимаете Start Game]
[MainMenuState] Start Game selected
[Game] Pushing PlayingState...
[PlayingState] onEnter() called
[PlayingState] Constructor called

[Когда нажимаете ESC]
[Game] Pushing PauseMenuState...
[PauseMenuState] onEnter() called
```

## ? Улучшения архитектуры

- ? Типобезопасная система состояний (no void*)
- ? Автоматическое управление памятью (unique_ptr)
- ? Lifecycle методы (onEnter/onExit)
- ? Полиморфизм вместо switch-case
- ? Легко добавлять новые состояния
- ? Полная отладка через консоль
