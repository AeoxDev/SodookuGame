#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <vector>
#include <utility>
#include <random>

class SudokuBoard;

using Grid = std::array<std::array<int, 9>, 9>;
struct PuzzleData
{
	Grid puzzle;
	Grid solution;
};

class SudokuGame
{
private:
	sf::Font font;
	//std::optional<SudokuBoard> leftBoard;
	//std::optional<SudokuBoard> rightBoard;
	std::unique_ptr<SudokuBoard> leftBoard;
	std::unique_ptr<SudokuBoard> rightBoard;
	std::vector<PuzzleData> puzzles;
	
public:
	SudokuGame();
	~SudokuGame();

	void run();

	bool loadFont(const std::string& fontPath);
	void loadPuzzles();
	void startNewGame();

	void draw(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event);
};