#pragma once
#include "Scene.h"
//#include "SudokuBoard.h"

class SudokuBoard;

enum class DifficultyLevel
{
    Easy,
    Medium,
    Hard
};

using Grid = std::array<std::array<int, 9>, 9>;
struct PuzzleData
{
    Grid puzzle{};
    Grid solution{};
    DifficultyLevel difficulty = DifficultyLevel::Easy;
};

class GameScene : public Scene
{
private:
    sf::Font font;
    std::optional<sf::Sprite> backgroundSprite;
    std::optional<sf::Sprite> sprite;

    std::unique_ptr<SudokuBoard> leftBoard;
    std::unique_ptr<SudokuBoard> rightBoard;

    std::vector<PuzzleData> puzzles;
public:
    GameScene(int difficulty, sf::RenderWindow& window);
    ~GameScene();

    void loadPuzzles();
    void startNewGame(DifficultyLevel selectedDifficulty);

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void onEnter() override {};
    void onExit() override {};
};