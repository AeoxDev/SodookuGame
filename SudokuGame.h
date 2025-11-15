#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <vector>
#include <utility>
#include <random>

#include "SudokuBoard.h"

class SudokuGame
{
private:
	sf::Font font;
	std::optional<SudokuBoard> leftBoard;
	std::optional<SudokuBoard> rightBoard;
	std::vector<std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>> puzzles;
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