#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include "SudokuGame.h"

class SudokuBoard
{
private:
	int grid[9][9]{};
	bool solved[9][9]{};
	bool solution[9][9]{};
	sf::RectangleShape cellShapes[9][9];
	sf::Text* cellTexts[9][9];
	sf::Vector2i selectedCell{-1, -1};

	// Audio "Engine"
	sf::SoundBuffer completionBuffer;
	std::optional<sf::Sound> completionSound;

	sf::SoundBuffer victoryBuffer;
	std::optional<sf::Sound> victorySound;

public:
	SudokuBoard(float posX, float posY, float size, sf::Font& font);
	~SudokuBoard();

	void draw(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event);
	void setCell(int row, int col, int value);
	int getCell(int row, int col) const;
	bool isValidMove(int row, int col, int value) const;
	void checkRowComplete();
	void loadPuzzle(const PuzzleData& data);
};