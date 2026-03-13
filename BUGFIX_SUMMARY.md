# Исправления проблемы с черным экраном

## ? Что было сделано

### 1. **Дебаг-вывод в консоль**
Добавлены сообщения инициализации для отслеживания:
- Инициализации игры
- Создания состояния PlayingState
- Загрузки ресурсов
- Вызова методов lifecycle (onEnter/onExit)

### 2. **Исправления в DrawGame()**
- Удалены `window.clear()` и `window.display()` из DrawGame
- Они уже вызываются в Application::Run()
- Проблема была в двойном clear, что приводило к пустому экрану

### 3. **Улучшение видимости**
- Цвет фона изменён с (0,0,50) на (30,30,80) для лучшей видимости
- Платформа: синяя (если нет текстуры)
- Шар: красный (если нет текстуры)
- Кирпичи: разноцветные (градиент)

### 4. **Диагностические методы**
Добавлены в IGameState:
```cpp
virtual const char* getStateName() const { return "UnknownState"; }
```

Реализованы во всех состояниях для идентификации.

### 5. **Проверка ресурсов**
Логирование загрузки:
- Шрифты
- Текстуры (Ball, Brick, Platform)
- Звуки (Hit, GameOver)

## ?? Как использовать отладку

### Во время запуска
1. Откройте "Output" window (Debug ? Windows ? Output)
2. Запустите программу
3. Смотрите консоль вывода

### Ожидаемый вывод
```
[Game] Initializing...
[PlayingState] Constructor called
[PlayingState] Constructor complete - geometry initialized
[PlayingState] Background size: 800x600
[PlayingState] Bricks created: 50
[Game] Pushing PlayingState...
[PlayingState] onEnter() called
[PlayingState] Loading resources...
[PlayingState] Font loaded: YES/NO
[PlayingState] Ball texture loaded: YES/NO
[PlayingState] Brick texture loaded: YES/NO
[PlayingState] Platform texture loaded: YES/NO
[PlayingState] onEnter() complete
[Game] Initialization complete. Active states: 1
```

## ?? Проверка игры

После запуска вы должны увидеть:
1. **Синий фон** (темно-синий, почти черный)
2. **Синюю платформу** внизу
3. **Красный шарик** над платформой
4. **Разноцветные кирпичи** в верхней части
5. **Желтый текст** "Score:" и "Lives:" в углах

### Управление
- **Arrow Keys / A-D** ? движение платформы
- **Mouse** ? движение платформы (когда клавиши не нажаты)
- **Space** ? запуск шара
- **F3** ? вкл/выкл отладочный overlay
- **Escape** ? пауза (не реализовано полностью)
- **Закрыть окно** ? выход

## ?? Возможные проблемы

### 1. Вообще черный экран (no console output)
- Проблема в инициализации Application
- Проверьте GameMain.cpp

### 2. Console выводит логи, но экран черный
- DrawGame() не рисует состояния
- Проверьте window.clear()/window.display() в Application::Run()

### 3. Console выводит "onEnter()" не вызывается
- pushState() не работает
- Проверьте Game::pushState()

### 4. Видна только синяя платформа, нет шара/кирпичей
- draw() вызывается парциально
- Проверьте порядок draw() в PlayingState::draw()

## ?? Следующие шаги

1. **Подтвердите, что игра видна** ? если да, отладка успешна
2. **Удалите debug-выводы** (std::cout) после решения проблемы
3. **Вернитесь к исходному цвету фона** (0,0,50) если хотите
4. **Реализуйте переходы состояний** (pause, game over и т.д.)

## ?? Файлы с изменениями

- `ArkanoidGame/Game.cpp` - добавлены логи, исправлено DrawGame()
- `ArkanoidGame/GameStatePlaying.cpp` - логирование, освещение фона
- `ArkanoidGame/GameStatePlaying.h` - getStateName()
- `ArkanoidGame/IGameState.h` - getStateName()
- `ArkanoidGame/GameStateMainMenu.h` - getStateName()
- `ArkanoidGame/GameStatePauseMenu.h` - getStateName()
- `ArkanoidGame/GameStateGameOver.h` - getStateName()
- `ArkanoidGame/GameStateRecords.h` - getStateName()
