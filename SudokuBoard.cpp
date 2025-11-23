#include "SudokuBoard.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

SudokuBoard::SudokuBoard(float posX, float posY, float size, sf::Font& font)
{
	if (!completionBuffer.loadFromFile("Audio/soundeffect_completion.wav"))
	{
		std::cerr << "Failed to load completion soundfile." << std::endl;
	}

	if (!victoryBuffer.loadFromFile("Audio/soundeffect_victory.wav"))
	{
		std::cerr << "Failed to load victory sound file" << std::endl;
	}

	completionSound.emplace(completionBuffer);
	victorySound.emplace(victoryBuffer);

	float cellSize = size / 9.f;
	for (int row = 0; row < 9; ++row)
	{
		for (int col = 0; col < 9; ++col)
		{
			grid[row][col] = 0;
			solved[row][col] = false;

			sf::RectangleShape& cell = cellShapes[row][col];
			cell.setSize({ cellSize - 2.f, cellSize - 2.f });
			cell.setPosition({ posX + col * cellSize, posY + row * cellSize });
			cell.setFillColor(sf::Color(50, 50, 50));
			cell.setOutlineThickness(1.f);
			cell.setOutlineColor(sf::Color::White);
			
			cellTexts[row][col] = new sf::Text(font);
			cellTexts[row][col]->setFont(font);
			cellTexts[row][col]->setString("");             // empty at start
			cellTexts[row][col]->setCharacterSize(24);      // size in pixels
			cellTexts[row][col]->setFillColor(sf::Color::White);

			// Get the text's local bounds
			sf::FloatRect textBounds = cellTexts[row][col]->getLocalBounds();
			// Center the origin based on the bounds
			cellTexts[row][col]->setOrigin({
			textBounds.position.x + textBounds.size.x / 2.f,
			textBounds.position.y + textBounds.size.y / 2.f
			});

			// Get the cell's global bounds
			sf::FloatRect cellBounds = cellShapes[row][col].getGlobalBounds();
			// Position the text at the center of the cell
			cellTexts[row][col]->setPosition({
			cellBounds.position.x + cellBounds.size.x / 2.f,
			cellBounds.position.y + cellBounds.size.y / 2.f
			});
		}
	}
}

SudokuBoard::~SudokuBoard()
{
	for (int row = 0; row < 9; ++row)
	{
		for (int col = 0; col < 9; ++col)
		{
			delete cellTexts[row][col];
		}
	}
}

void SudokuBoard::loadPuzzle(const PuzzleData& data)
{
	for (int row = 0; row < 9; ++row)
	{
		for (int col = 0; col < 9; ++col)
		{
			grid[row][col] = data.puzzle[row][col];
			solution[row][col] = data.solution[row][col];
			solved[row][col] = (grid[row][col] != 0 && grid[row][col] == solution[row][col]);

			cellTexts[row][col]->setString(grid[row][col] == 0 ? "" : std::to_string(grid[row][col]));

			// recenter text
			sf::FloatRect textBounds = cellTexts[row][col]->getLocalBounds();
			cellTexts[row][col]->setOrigin({
				textBounds.position.x + textBounds.size.x / 2.f,
				textBounds.position.y + textBounds.size.y / 2.f
				});
			sf::FloatRect cellBounds = cellShapes[row][col].getGlobalBounds();
			cellTexts[row][col]->setPosition({
				cellBounds.position.x + cellBounds.size.x / 2.f,
				cellBounds.position.y + cellBounds.size.y / 2.f
				});
		}
	}

	// --- Initialize static solved trackers ---
	for (int i = 0; i < 9; ++i)
	{
		rowWasSolved[i] = true;
		colWasSolved[i] = true;
		blockWasSolved[i] = true;
		// Check if row/col/block really is solved
		for (int j = 0; j < 9; ++j)
		{
			if (!solved[i][j]) rowWasSolved[i] = false;
			if (!solved[j][i]) colWasSolved[i] = false;
		}
	}

	// Check 3x3 blocks
	for (int block = 0; block < 9; ++block)
	{
		int startRow = (block / 3) * 3;
		int startCol = (block % 3) * 3;
		for (int r = startRow; r < startRow + 3; ++r)
			for (int c = startCol; c < startCol + 3; ++c)
				if (!solved[r][c]) blockWasSolved[block] = false;
	}

	// Check full board
	boardWasSolved = true;
	for (int r = 0; r < 9 && boardWasSolved; ++r)
		for (int c = 0; c < 9 && boardWasSolved; ++c)
			if (!solved[r][c]) boardWasSolved = false;
}

void SudokuBoard::setCell(int col, int row, int number)
{
	// Check out of bounds
	if (col < 0 || col > 8 || row < 0 || row > 8)
		return;

	grid[row][col] = number;
	cellTexts[row][col]->setString(number == 0 ? "" : std::to_string(number));

	// Check if cell is solved based on final solution grid
	solved[row][col] = (number != 0 && number == solution[row][col]);

	// Center text in the middle of the cell
	sf::FloatRect textBounds = cellTexts[row][col]->getLocalBounds();
	cellTexts[row][col]->setOrigin({
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
		});

	sf::FloatRect cellBounds = cellShapes[row][col].getGlobalBounds();
	cellTexts[row][col]->setPosition({
		cellBounds.position.x + cellBounds.size.x / 2.f,
		cellBounds.position.y + cellBounds.size.y / 2.f
		});

	// Check row 
	bool rowNowSolved = true;
	for (int c = 0; c < 9; ++c) {
		if (!solved[row][c]) { rowNowSolved = false; break; }
	}
	if (rowNowSolved && !rowWasSolved[row]) {
		rowWasSolved[row] = true;
		std::cout << "ROW COMPLETE: " << row << std::endl;
		completionSound->play();
	}
	else if (!rowNowSolved) {
		rowWasSolved[row] = false;
	}

	// Check column
	bool colNowSolved = true;
	for (int r = 0; r < 9; ++r) {
		if (!solved[r][col]) { colNowSolved = false; break; }
	}
	if (colNowSolved && !colWasSolved[col]) {
		colWasSolved[col] = true;
		std::cout << "COLUMN COMPLETE: " << col << std::endl;
		completionSound->play();
	}
	else if (!colNowSolved) {
		colWasSolved[col] = false;
	}

	// Check 3x3 block
	int blockRow = row / 3;
	int blockCol = col / 3;
	int blockIndex = blockRow * 3 + blockCol;
	bool blockNowSolved = true;
	for (int r = blockRow * 3; r < blockRow * 3 + 3; ++r)
		for (int c = blockCol * 3; c < blockCol * 3 + 3; ++c)
			if (!solved[r][c]) blockNowSolved = false;

	if (blockNowSolved && !blockWasSolved[blockIndex]) {
		blockWasSolved[blockIndex] = true;
		std::cout << "BLOCK COMPLETE: " << blockIndex << std::endl;
		completionSound->play();
	}
	else if (!blockNowSolved) {
		blockWasSolved[blockIndex] = false;
	}

	// Check full board
	bool boardNowSolved = true;
	for (int r = 0; r < 9 && boardNowSolved; ++r)
		for (int c = 0; c < 9 && boardNowSolved; ++c)
			if (!solved[r][c]) boardNowSolved = false;

	if (boardNowSolved && !boardWasSolved) {
		boardWasSolved = true;
		std::cout << "BOARD COMPLETE!" << std::endl;
		victorySound->play();
	}
	else if (!boardNowSolved) {
		boardWasSolved = false;
	}
}

int SudokuBoard::getCell(int row, int col) const
{
	// Check out of bounds
	if (row < 0 || row > 8 || col < 0 || col > 8)
		return -1; 

	return grid[row][col];
}

bool SudokuBoard::isValidMove(int row, int col, int value) const
{
	if (row < 0 || row > 8 || col < 0 || col > 8)
		return false;
	if (value < 1 || value > 9)
		return false;

	// Check row
	for (int c = 0; c < 9; ++c)
	{
		if (grid[row][c] == value && c != col)
			return false;
	}

	// Check column
	for (int r = 0; r < 9; ++r)
	{
		if (grid[r][col] == value && r != row)
			return false;
	}

	// Check 3x3 subgrid
	int startRow = (row / 3) * 3;
	int startCol = (col / 3) * 3;

	for (int r = startRow; r < startRow + 3; ++r)
	{
		for (int c = startCol; c < startCol + 3; ++c)
		{
			if (grid[r][c] == value && (r != row || c != col))
				return false;
		}
	}

	return true;
}


void SudokuBoard::draw(sf::RenderWindow& window)
{
	for (int row = 0; row < 9; ++row)
	{
		for (int col = 0; col < 9; ++col)
		{
			window.draw(cellShapes[row][col]);
			int value = grid[row][col];
			if (value != 0)
			{
				window.draw(*cellTexts[row][col]);
			}
		}
	}

	// Draw thicker subgrid lines (after drawing cells)
	float cellSize = cellShapes[0][0].getSize().x + 2.f;  // add outline offset if needed
	sf::Vector2f boardPos = cellShapes[0][0].getPosition();

	sf::RectangleShape line;
	line.setFillColor(sf::Color::White);

	// Draw vertical lines at 3 and 6 (thick)
	for (int i : {3, 6})
	{
		line.setSize({ 5.f, 9 * cellSize });
		line.setPosition({ boardPos.x + i * cellSize - 2.f, boardPos.y });
		window.draw(line);
	}

	// Draw horizontal lines at 3 and 6 (thick)
	for (int i : {3, 6})
	{
		line.setSize({ 9 * cellSize, 5.f });
		line.setPosition({ boardPos.x, boardPos.y + i * cellSize - 2.f });
		window.draw(line);
	}

	// Draw border around the whole board (optional, thick)
	sf::RectangleShape border;
	border.setPosition(boardPos);
	border.setSize({ 9 * cellSize, 9 * cellSize });
	border.setOutlineThickness(5.f);
	border.setOutlineColor(sf::Color::White);
	border.setFillColor(sf::Color::Transparent);
	window.draw(border);

	if (selectedCell.x != -1)
	{
		sf::RectangleShape highlight;
		highlight.setSize(cellShapes[0][0].getSize());
		highlight.setPosition(cellShapes[selectedCell.y][selectedCell.x].getPosition());
		highlight.setFillColor(sf::Color(255, 255, 0, 50));
		window.draw(highlight);
	}
}

void SudokuBoard::handleEvent(const sf::Event& event)
{
	if (inputMode != InputMode::Player)
	{
		return;
	}

	// Check for mouse button press
	if (event.is<sf::Event::MouseButtonPressed>())
	{
		if (auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				// Convert to float coordinates
				sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x),
					static_cast<float>(mouseEvent->position.y));

				// Check which cell was clicked
				for (int row = 0; row < 9; ++row)
				{
					for (int col = 0; col < 9; ++col)
					{
						if (cellShapes[row][col].getGlobalBounds().contains(mousePos))
						{
							selectedCell = { col, row };
							return;
						}
					}
				}

				// If no cell was clicked
				selectedCell = { -1, -1 };
			}
		}
	}

	// --- Keyboard selection ---
	if (event.is<sf::Event::KeyPressed>())
	{
		if (auto keyEvent = event.getIf<sf::Event::KeyPressed>())
		{
			if (selectedCell.x == -1 || selectedCell.y == -1)
			{
				// If no cell selected yet, start at (0,0)
				selectedCell = { 0, 0 };
			}
			else
			{
				int col = selectedCell.x;
				int row = selectedCell.y;

				switch (keyEvent->code)
				{
				case sf::Keyboard::Key::Left:
					col = std::max(0, col - 1);
					break;
				case sf::Keyboard::Key::Right:
					col = std::min(8, col + 1);
					break;
				case sf::Keyboard::Key::Up:
					row = std::max(0, row - 1);
					break;
				case sf::Keyboard::Key::Down:
					row = std::min(8, row + 1);
					break;
				default:
					break;
				}

				selectedCell = { col, row };
			}
			
			// 1-9 Cell Input
			if (selectedCell.x != -1 && selectedCell.y != -1)
			{
				int num = 0;
				switch (keyEvent->code)
				{
				case sf::Keyboard::Key::Num1: case sf::Keyboard::Key::Numpad1: num = 1; break;
				case sf::Keyboard::Key::Num2: case sf::Keyboard::Key::Numpad2: num = 2; break;
				case sf::Keyboard::Key::Num3: case sf::Keyboard::Key::Numpad3: num = 3; break;
				case sf::Keyboard::Key::Num4: case sf::Keyboard::Key::Numpad4: num = 4; break;
				case sf::Keyboard::Key::Num5: case sf::Keyboard::Key::Numpad5: num = 5; break;
				case sf::Keyboard::Key::Num6: case sf::Keyboard::Key::Numpad6: num = 6; break;
				case sf::Keyboard::Key::Num7: case sf::Keyboard::Key::Numpad7: num = 7; break;
				case sf::Keyboard::Key::Num8: case sf::Keyboard::Key::Numpad8: num = 8; break;
				case sf::Keyboard::Key::Num9: case sf::Keyboard::Key::Numpad9: num = 9; break;
				case sf::Keyboard::Key::Backspace: 
				case sf::Keyboard::Key::Delete: num = 0;  break; // Clear Cell
				default: break;
				}

				if (num != 0)
				{
					if (isValidMove(selectedCell.y, selectedCell.x, num))
					{
						setCell(selectedCell.x, selectedCell.y, num);
					}
				}
				else if (keyEvent->code == sf::Keyboard::Key::Backspace || keyEvent->code == sf::Keyboard::Key::Delete)
				{
					setCell(selectedCell.x, selectedCell.y, 0);
				}
				
			}
		}
	}
}