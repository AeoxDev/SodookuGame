#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
//#include "GameScene.h"
//class GameScene;
struct PuzzleData;

enum class InputMode
{
	None,
	Player,
	Computer
};

class SudokuBoard
{
private:
	int grid[9][9]{};
	bool solved[9][9]{};
	int solution[9][9]{};

	InputMode inputMode = InputMode::None;

	// Solved trackers
	bool rowWasSolved[9]{false};
	bool colWasSolved[9]{false};
	bool blockWasSolved[9]{false};
	bool boardWasSolved = false;
	
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

	void loadPuzzle(const PuzzleData& data);
	
	void SetInputMode(InputMode mode) { inputMode = mode; }
	InputMode GetInputMode() const { return inputMode; }

	void setCell(int row, int col, int value);
	int getCell(int row, int col) const;

	void draw(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event);
	bool isValidMove(int row, int col, int value) const;

	
	
};