#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <vector>
#include <utility>
#include <random>

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
	DifficultyLevel difficulty;
};

class SudokuGame
{
private:
	sf::Font font;

	std::unique_ptr<SudokuBoard> leftBoard;
	std::unique_ptr<SudokuBoard> rightBoard;

	std::vector<PuzzleData> puzzles;
	
public:
	SudokuGame();
	~SudokuGame();
	
	bool loadFont(const std::string& fontPath);
	void loadPuzzles();
	void startNewGame(DifficultyLevel selectedDifficulty);

	void run();
	void draw(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event);
};