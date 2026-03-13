# Архитектура игровых состояний - Рефакторинг

## Проблемы с предыдущей архитектурой

### 1. **Типобезопасность (Type Safety)**
```cpp
// Было: использование void* и ручные приведения
class GameState {
    void* data;  // Опасно, нет проверки типов
    ~GameState() {
        switch(type) {
            case GameStateType::Playing:
                delete static_cast<GameStatePlayingData*>(data);  // Ручное приведение
        }
    }
};
```

### 2. **Масштабируемость (Open/Closed Principle)**
При добавлении нового состояния нужно было менять 5+ мест:
- Enum `GameStateType`
- Конструктор `GameState`
- Деструктор `GameState`
- Switch-case в функциях Init/Update/Draw/HandleEvent
- Forward declarations

### 3. **Разброс логики**
- Инициализация в `InitGameState*`
- Обновление в `UpdateGameState*`
- Отрисовка в `DrawGameState*`
- Обработка событий в `HandleGameState*Event`
- Lifecycle события (onEnter, onExit) не существовали

---

## Новая архитектура - IGameState

### Базовый класс состояния

```cpp
class IGameState {
public:
    virtual ~IGameState() = default;
    
    virtual void onEnter() {}           // Вызывается при входе
    virtual void onExit() {}            // Вызывается при выходе
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float timeDelta) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

using GameStatePtr = std::unique_ptr<IGameState>;
```

### Пример конкретного состояния

```cpp
class PlayingState : public IGameState {
private:
    sf::Font font;
    sf::Sound hitSound;
    // ... другие данные
    
public:
    PlayingState(float screenWidth, float screenHeight);
    
    void onEnter() override {
        // Загрузка ресурсов
    }
    
    void onExit() override {
        // Очистка (RAII)
    }
    
    void handleEvent(const sf::Event& event) override { ... }
    void update(float timeDelta) override { ... }
    void draw(sf::RenderWindow& window) override { ... }
};
```

### Упрощённый Game класс

```cpp
class Game {
public:
    std::vector<GameStatePtr> stateStack;
    
    void pushState(GameStatePtr state) {
        if (!stateStack.empty()) stateStack.back()->onExit();
        state->onEnter();
        stateStack.push_back(std::move(state));
    }
    
    void popState() {
        if (!stateStack.empty()) {
            stateStack.back()->onExit();
            stateStack.pop_back();
        }
        if (!stateStack.empty()) stateStack.back()->onEnter();
    }
    
    void switchState(GameStatePtr state) {
        stateStack.clear();
        state->onEnter();
        stateStack.push_back(std::move(state));
    }
};
```

### Упрощённый Game.cpp

```cpp
void HandleWindowEvents(Game& game, sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        IGameState* currentState = game.getCurrentState();
        if (currentState) {
            currentState->handleEvent(event);  // Полиморфизм!
        }
    }
}

void UpdateGame(Game& game, float timeDelta) {
    IGameState* currentState = game.getCurrentState();
    if (currentState) {
        currentState->update(timeDelta);  // Полиморфизм!
    }
}

void DrawGame(Game& game, sf::RenderWindow& window) {
    for (auto& state : game.stateStack) {
        state->draw(window);  // Полиморфизм!
    }
}
```

---

## Преимущества

| Аспект | Было | Стало |
|--------|------|-------|
| **Типобезопасность** | `void*` + ручное приведение | Virtual methods, `std::unique_ptr` |
| **Switch-case** | 5+ мест в Game.cpp | 0 switch-case, только полиморфизм |
| **Добавление состояния** | Изменить 6+ файлов | Создать класс, наследовать IGameState |
| **Lifecycle события** | Нет | `onEnter()`, `onExit()` |
| **Утечки памяти** | Рискованно | RAII с `unique_ptr` |
| **Читаемость** | Сложно отследить логику | Вся логика состояния в одном классе |

---

## Реализованные состояния

1. **PlayingState** - основная игра (платформа, шар, кирпичи)
2. **MainMenuState** - главное меню с опциями
3. **PauseMenuState** - меню паузы
4. **GameOverState** - экран проигрыша со статистикой
5. **RecordsState** - таблица рекордов

Каждое состояние:
- ? Инкапсулирует собственные данные
- ? Управляет ресурсами через RAII
- ? Имеет lifecycle методы (onEnter/onExit)
- ? Реализует virtual interface (handleEvent/update/draw)

---

## Пример использования

```cpp
// Старый способ:
PushGameState(game, GameStateType::Playing, false);
InitGameStatePlaying(*static_cast<GameStatePlayingData*>(state.getData()));

// Новый способ:
game.pushState(std::make_unique<PlayingState>(SCREEN_WIDTH, SCREEN_HEIGHT));
// onEnter() вызовется автоматически!
```

---

## Дальнейшие улучшения

1. Добавить callback-и для переходов между состояниями (вместо жёсткой связи)
2. Добавить state machine для управления сложными переходами
3. Реализовать pause/resume для состояний
4. Добавить animation transitions между состояниями
