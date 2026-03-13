# Отладка проблемы с чёрным экраном

## Кратко - что нужно проверить

### 1. Запустите игру и посмотрите консоль
Вывод в консоль будет выглядеть так:
```
[Game] Initializing...
[PlayingState] Constructor called
[PlayingState] Constructor complete - geometry initialized
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

### 2. Возможные проблемы

#### Черный экран, но видна платформа/шар
- ? Состояние инициализируется правильно
- ? Отрисовка работает
- ?? Может быть проблема с размерами/цветами

#### Полностью черный экран
- ?? **onEnter() не вызывается** ? проблема в pushState()
- ?? **draw() не вызывается** ? проблема в DrawGame()
- ?? **Фон рисуется черным** ? это корректно (цвет 0,0,50 очень темный)

#### Видна консоль с сообщениями, но игры нет
- ?? onEnter() вызывается, но draw() нет
- ?? Проверьте Application::Run() и window.clear()/display()

### 3. Минимальная отладка

Добавьте в PlayingState::draw():
```cpp
void PlayingState::draw(sf::RenderWindow& window)
{
    // Draw solid background first to see if draw is called at all
    sf::RectangleShape testBG(sf::Vector2f(800, 600));
    testBG.setFillColor(sf::Color::Green);  // Ярко-зеленый для видимости
    window.draw(testBG);
    
    // If you see green, draw() works!
    // Then add other objects...
}
```

### 4. Проверка ресурсов

Убедитесь, что папка `Resources/` находится в корректном месте:
```
Project/
??? Resources/
?   ??? Textures/ (Ball.png, Brick.png, Platform.png)
?   ??? Fonts/ (Roboto-Regular.ttf)
?   ??? Sounds/ (Hit.wav, GameOver.wav)
??? ArkanoidGame.exe (or Debug folder)
```

Если ресурсы не найдены, фолбэк цвета должны отобразиться:
- Платформа ? синяя
- Шар ? красный
- Кирпичи ? разноцветные

### 5. Возможные исправления

**Если видна только черный экран:**
```cpp
// В PlayingState::draw(), добавьте вывод диагностики:
void PlayingState::draw(sf::RenderWindow& window) {
    // Debug: draw test rectangle
    sf::RectangleShape test(sf::Vector2f(100, 100));
    test.setFillColor(sf::Color::White);
    test.setPosition(350, 250);
    window.draw(test);
    
    // ... rest of draw code
}
```

**Если onEnter() не вызывается:**
- Проверьте Game::pushState()
- Убедитесь, что stateStack содержит состояние
- Проверьте, вызывается ли state->onEnter()

**Если фон черный, но ничего не видно:**
- Цвет фона (0,0,50) очень темный
- Измените на что-то светлее для отладки: sf::Color(100, 100, 150)

## Быстрая проверка

1. Запустите программу
2. Посмотрите консоль ? убедитесь, что все инициализируется
3. Посмотрите на экран ? должна быть что-то видно (даже если не полная игра)
4. Нажмите F3 ? вкл/выкл отладочный текст

## Вывод

Если видите консоль с логами, но экран черный:
1. Проблема в Application::Run() или DrawGame()
2. Проверьте window.clear() / window.display()

Если видите консоль без "onEnter()", то:
1. pushState() не срабатывает
2. Проверьте Game::pushState() в Game.h
