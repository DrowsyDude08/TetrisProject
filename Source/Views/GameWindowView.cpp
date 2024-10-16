#include "GameWindowView.hpp"
/// <summary>
/// Конструктор для класса GameWindowView. Инициализирует GameWindowView  с помощью данного окна рендеринга.
/// 
/// Он также настраивает кнопки главного меню, их текст и инициализирует кнопки завершения игры. 
/// Шрифт для текста загружается из указанного источника. 
/// Расположение кнопок, размеры и цвета устанавливаются в зависимости от размера игрового окна и предопределенных констант.
/// </summary>
/// <param name="window">Окно рендеринга SFML, которое будет использоваться для рендеринга игрового представления.</param>
GameWindowView::GameWindowView(sf::RenderWindow& window)
    : window(window), palette(), hoveredButton(nullptr), selectedButtonIndex(0) {
    initializeButtons();

    if (!font.loadFromFile(resourcePath + "/Fonts/ShoraiSansStdNVar.ttf")) {
        throw std::runtime_error("Failed to load Fonts for GameWindowView");
    }

    sf::Text::Style boldStyle = sf::Text::Bold;

    mainMenuStartText.setFont(font);
    mainMenuStartText.setString("Start");
    mainMenuStartText.setCharacterSize(24);
    mainMenuStartText.setFillColor(palette.defaultTextColor);
    mainMenuStartText.setStyle(boldStyle);
    mainMenuStartText.setPosition(
        (mainMenuStartButton.getPosition().x + (mainMenuStartButton.getSize().x - mainMenuStartText.getLocalBounds().width) / 2),
        (mainMenuStartButton.getPosition().y + (mainMenuStartButton.getSize().y - mainMenuStartText.getLocalBounds().height) / 2)
    );

    mainMenuChangeColorText.setFont(font);
    mainMenuChangeColorText.setString("Change Color");
    mainMenuChangeColorText.setCharacterSize(24);
    mainMenuChangeColorText.setFillColor(palette.defaultTextColor);
    mainMenuChangeColorText.setStyle(boldStyle);
    mainMenuChangeColorText.setPosition(
        (mainMenuChangeColorButton.getPosition().x + (mainMenuChangeColorButton.getSize().x - mainMenuChangeColorText.getLocalBounds().width) / 2),
        (mainMenuChangeColorButton.getPosition().y + (mainMenuChangeColorButton.getSize().y - mainMenuChangeColorText.getLocalBounds().height) / 2)
    );

    mainMenuExitText.setFont(font);
    mainMenuExitText.setString("Exit");
    mainMenuExitText.setCharacterSize(24);
    mainMenuExitText.setFillColor(palette.defaultTextColor);
    mainMenuExitText.setStyle(boldStyle);
    mainMenuExitText.setPosition(
        (mainMenuExitButton.getPosition().x + (mainMenuExitButton.getSize().x - mainMenuExitText.getLocalBounds().width) / 2),
        (mainMenuExitButton.getPosition().y + (mainMenuExitButton.getSize().y - mainMenuExitText.getLocalBounds().height) / 2)
    );

    gameOverReplayText.setFont(font);
    gameOverReplayText.setString("Restart");
    gameOverReplayText.setCharacterSize(24);
    gameOverReplayText.setFillColor(palette.defaultTextColor);
    gameOverReplayText.setPosition(300, 450);

    gameOverToMainMenuText.setFont(font);
    gameOverToMainMenuText.setString("To Main Menu");
    gameOverToMainMenuText.setCharacterSize(24);
    gameOverToMainMenuText.setFillColor(palette.defaultTextColor);
    gameOverToMainMenuText.setPosition(300, 500);

    selectMainMenuButton(selectedButtonIndex);
}

/// <summary>
/// Инициализирует кнопки главного меню и соответствующий им текст.
/// Эта функция вычисляет расположение кнопок главного меню и их текст на основе размера окна и кнопок.
/// Она также загружает шрифт для текста и устанавливает первоначальный вид кнопок.
/// </summary>
/// 
void GameWindowView::initializeButtons() {
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonSpacing = 20;

    float centerX = (window.getSize().x - buttonWidth) / 2;

    mainMenuStartButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuStartButton.setPosition(centerX, 150);
    mainMenuStartButton.setFillColor(palette.defaultButtonColor);

    mainMenuChangeColorButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuChangeColorButton.setPosition(centerX, 150 + buttonHeight + buttonSpacing);
    mainMenuChangeColorButton.setFillColor(palette.defaultButtonColor);

    mainMenuExitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuExitButton.setPosition(centerX, 150 + 2 * (buttonHeight + buttonSpacing));
    mainMenuExitButton.setFillColor(palette.defaultButtonColor);

    gameOverReplayButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    gameOverReplayButton.setPosition(centerX, 500);
    gameOverReplayButton.setFillColor(palette.defaultButtonColor);

    gameOverToMainMenuButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    gameOverToMainMenuButton.setPosition(centerX, 500 + buttonHeight + buttonSpacing);
    gameOverToMainMenuButton.setFillColor(palette.defaultButtonColor);
}

/// <summary>
/// Функция отрисовки главного меню.
/// </summary>
void GameWindowView::drawMainMenu() {
    window.clear(palette.background);
    window.draw(mainMenuStartButton);
    window.draw(mainMenuChangeColorButton);
    window.draw(mainMenuExitButton);
    window.draw(mainMenuStartText);
    window.draw(mainMenuChangeColorText);
    window.draw(mainMenuExitText);
}

/// <summary>
/// Метод, отслеживающий движение мыши в главном меню.
/// 
/// Эта функция обновляет указатель нажатой кнопки и выбранной кнопки в зависимости от положения мыши.
/// Если мышь не находится в главном меню, она ничего не делает.
/// </summary>
/// <param name="mouseX"> X-координаты позиции мышки </param>
/// <param name="mouseY"> X-координаты позиции мышки </param>
/// <param name="gameState"> Текущее состояние игры </param>
void GameWindowView::handleMainMenuMouseMove(int mouseX, int mouseY, GameState gameState) {
    if (gameState != GameState::MainMenu) {
        return;
    }

    if (mainMenuStartButton.getGlobalBounds().contains(mouseX, mouseY)) {
        hoveredButton = &mainMenuStartButton;
        selectedButtonIndex = 0;
    }
    else if (mainMenuChangeColorButton.getGlobalBounds().contains(mouseX, mouseY)) {
        hoveredButton = &mainMenuChangeColorButton;
        selectedButtonIndex = 1;
    }
    else if (mainMenuExitButton.getGlobalBounds().contains(mouseX, mouseY)) {
        hoveredButton = &mainMenuExitButton;
        selectedButtonIndex = 2;
    }
    else {
        hoveredButton = nullptr;
    }

    updateMainMenuButtonAppearance();
}

/// <summary>
/// Отслеживает нажатие кнопок клавиатуры в главном меню.
/// 
/// Эта функция обновляет индекс выбранной кнопки и вызывает функцию выбора кнопки главного меню на основе ввода с клавиатуры.
/// Если нажата клавиша со стрелкой вверх, индекс выбранной кнопки уменьшается и при необходимости переносится на последнюю кнопку.
/// нажата клавиша со стрелкой вниз, индекс выбранной кнопки увеличивается и при необходимости переносится на первую кнопку.
/// Если нажата клавиша enter, вызывается функция выбора кнопки главного меню с текущим индексом выбранной кнопки.
/// </summary>
/// <param name="event">Ивент с клавиатуры</param>
void GameWindowView::handleMainMenuKeyboardInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedButtonIndex = (selectedButtonIndex + 2) % 3;
            selectMainMenuButton(selectedButtonIndex);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedButtonIndex = (selectedButtonIndex + 1) % 3;
            selectMainMenuButton(selectedButtonIndex);
        }
        else if (event.type == sf::Keyboard::Enter) {
            selectMainMenuButton(selectedButtonIndex);
        }
    }
}

/// <summary>
/// Отслеживает нажата ли была кнопка в зависимости от координат мыши.
/// </summary>
bool GameWindowView::isStartButtonClicked(int mouseX, int mouseY) {
    return mainMenuStartButton.getGlobalBounds().contains(mouseX, mouseY);
}

/// <summary>
/// Отслеживает нажата ли была кнопка в зависимости от координат мыши.
/// </summary>
bool GameWindowView::isChangeColorButtonClicked(int mouseX, int mouseY) {
    return mainMenuChangeColorButton.getGlobalBounds().contains(mouseX, mouseY);
}

/// <summary>
/// Отслеживает нажата ли была кнопка в зависимости от координат мыши.
/// </summary>
bool GameWindowView::isExitButtonClicked(int mouseX, int mouseY) {
    return mainMenuExitButton.getGlobalBounds().contains(mouseX, mouseY);
}

/// <summary>
/// Выбирает и подсвечивает кнопку главного меню в соответствии с заданным кейсом.
/// </summary>
/// <param name="index"> индекс выбранной кнопки </param>
void GameWindowView::selectMainMenuButton(int index) {
    switch (index) {
    case 0:
        hoveredButton = &mainMenuStartButton;
        break;
    case 1:
        hoveredButton = &mainMenuChangeColorButton;
        break;
    case 2:
        hoveredButton = &mainMenuExitButton;
        break;
    default:
        hoveredButton = nullptr;
        break;
    }
    updateMainMenuButtonAppearance();
}

/// <summary>
/// Обновляет внешний вид кнопок в зависимости от выбранной.
/// 
/// Этот метод меняет цвет кнопки и текста на ней в зависимости от того была она выбрана или нет.
/// </summary>
void GameWindowView::updateMainMenuButtonAppearance() {
    mainMenuStartButton.setFillColor(hoveredButton == &mainMenuStartButton ? palette.selectedButtonColor : palette.defaultButtonColor);
    mainMenuChangeColorButton.setFillColor(hoveredButton == &mainMenuChangeColorButton ? palette.selectedButtonColor : palette.defaultButtonColor);
    mainMenuExitButton.setFillColor(hoveredButton == &mainMenuExitButton ? palette.selectedButtonColor : palette.defaultButtonColor);

    mainMenuStartText.setFillColor(hoveredButton == &mainMenuStartButton ? palette.selectedTextColor : palette.defaultTextColor);
    mainMenuChangeColorText.setFillColor(hoveredButton == &mainMenuChangeColorButton ? palette.selectedTextColor : palette.defaultTextColor);
    mainMenuExitText.setFillColor(hoveredButton == &mainMenuExitButton ? palette.selectedTextColor : palette.defaultTextColor);
}

/// <summary>
/// Отрисовка Game over меню.
/// </summary>
/// <param name="leaderboard"> Таблица лидеров </param>
/// <param name="currentScore"> текущий счет </param>
void GameWindowView::drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore) {
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(palette.background.r, palette.background.g, palette.background.b, 150));
    window.draw(background);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(palette.defaultTextColor);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(
        (window.getSize().x - gameOverText.getLocalBounds().width) / 2,
        window.getSize().y * 0.2f);
    window.draw(gameOverText);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Your Score: " + std::to_string(currentScore));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(palette.defaultTextColor);
    scoreText.setPosition(
        (window.getSize().x - scoreText.getLocalBounds().width) / 2,
        window.getSize().y * 0.4f);
    window.draw(scoreText);

    if (!leaderboard.getScores().empty()) {
        sf::Text topScoreText;
        topScoreText.setFont(font);
        topScoreText.setString("Top Score: " + std::to_string(leaderboard.getScores().front().score));
        topScoreText.setCharacterSize(35);
        topScoreText.setFillColor(palette.defaultTextColor);
        topScoreText.setPosition(
            (window.getSize().x - topScoreText.getLocalBounds().width) / 2,
            window.getSize().y * 0.5f);
        window.draw(topScoreText);
    }

    sf::RectangleShape retryButton(sf::Vector2f(200, 50));
    retryButton.setFillColor(palette.defaultButtonColor);
    retryButton.setOutlineColor(palette.defaultOutlineColor);
    retryButton.setOutlineThickness(3);
    retryButton.setPosition((window.getSize().x - retryButton.getSize().x) / 2, window.getSize().y * 0.7f);
    window.draw(retryButton);

    sf::Text retryButtonText;
    retryButtonText.setFont(font);
    retryButtonText.setString("Retry");
    retryButtonText.setCharacterSize(30);
    retryButtonText.setFillColor(palette.defaultTextColor);
    retryButtonText.setPosition(
        retryButton.getPosition().x + (retryButton.getSize().x - retryButtonText.getLocalBounds().width) / 2,
        retryButton.getPosition().y + (retryButton.getSize().y - retryButtonText.getLocalBounds().height) / 2 - 5
    );
    window.draw(retryButtonText);

    sf::RectangleShape menuButton(sf::Vector2f(200, 50));
    menuButton.setFillColor(palette.defaultButtonColor);
    menuButton.setOutlineColor(palette.defaultOutlineColor);
    menuButton.setOutlineThickness(3);
    menuButton.setPosition((window.getSize().x - menuButton.getSize().x) / 2, window.getSize().y * 0.8f);
    window.draw(menuButton);

    sf::Text menuButtonText;
    menuButtonText.setFont(font);
    menuButtonText.setString("Main Menu");
    menuButtonText.setCharacterSize(30);
    menuButtonText.setFillColor(palette.defaultTextColor);
    menuButtonText.setPosition(
        menuButton.getPosition().x + (menuButton.getSize().x - menuButtonText.getLocalBounds().width) / 2,
        menuButton.getPosition().y + (menuButton.getSize().y - menuButtonText.getLocalBounds().height) / 2 - 5
    );
    window.draw(menuButtonText);
}

/// <summary>
/// Отслеживает нажата ли была кнопка в зависимости от координат мыши.
/// </summary>
bool GameWindowView::isGameOverReplayClicked(int mouseX, int mouseY) {
    return gameOverReplayButton.getGlobalBounds().contains(mouseX, mouseY);
}

/// <summary>
/// Отслеживает нажата ли была кнопка в зависимости от координат мыши.
/// </summary>
bool GameWindowView::isGameOverToMainMenuClicked(int mouseX, int mouseY) {
    return gameOverToMainMenuButton.getGlobalBounds().contains(mouseX, mouseY);
}

//void GameWindowView::selectGameOverButton(int index) {
//    switch (index) {
//    case 0:
//        hoveredButton = &gameOverReplayButton;
//        break;
//    case 1:
//        hoveredButton = &gameOverToMainMenuButton;
//        break;
//    default:
//        hoveredButton = nullptr;
//        break;
//    }
//    updateGameOverButtonAppearance();
//}
//void GameWindowView::updateGameOverButtonAppearance() {
//    gameOverReplayButton.setFillColor(hoveredButton == &gameOverReplayButton ? palette.selectedButtonColor : palette.defaultButtonColor);
//    gameOverToMainMenuButton.setFillColor(hoveredButton == &gameOverToMainMenuButton ? palette.selectedButtonColor : palette.defaultButtonColor);
//
//    gameOverReplayText.setFillColor(hoveredButton == &gameOverReplayButton ? palette.selectedTextColor : palette.defaultTextColor);
//    gameOverToMainMenuText.setFillColor(hoveredButton == &gameOverToMainMenuButton ? palette.selectedTextColor : palette.defaultTextColor);
//}