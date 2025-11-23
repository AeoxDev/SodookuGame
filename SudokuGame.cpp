#include "SudokuGame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include "SudokuBoard.h"
#include "External/nlohmann/json.hpp"

SudokuGame::SudokuGame()
{
	if (!loadFont("fonts/arialbd.ttf"))
	{
		std::cerr << "Failed to load font." << std::endl;
	}
	leftBoard = std::make_unique<SudokuBoard>(95.f, 260.f, 400.f, font);
	rightBoard = std::make_unique<SudokuBoard>(1100.f, 265.f, 400.f, font);

    leftBoard->SetInputMode(InputMode::Player);

	loadPuzzles();
	startNewGame(DifficultyLevel::Easy);
}

SudokuGame::~SudokuGame()
{
}

void SudokuGame::run()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 1600u, 900u }), "Sudooku Game");

    // Load Textures
    sf::Texture background;
    if (!background.loadFromFile("Images/background3.png"))
    {
        std::cerr << "Failed to load background." << std::endl;
        //return 1;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("Images/count_dooku2.png"))
    {
        std::cerr << "Failed to load texture." << std::endl;
        //return 1;
    }

    // Create Background Sprite
    sf::Sprite backgroundSprite(background);

    // Create Dooku Sprite
    sf::Sprite sprite(texture);

    // Get Sprite Size, Window Size, Sprite Bounds
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = texture.getSize();
    sf::FloatRect spriteBounds = sprite.getLocalBounds();

    backgroundSprite.setPosition({ 0.f, 0.f });

    // Set scale to 25% of window size
    sprite.setScale({ 0.95f * (window.getSize().x * 0.25f) / textureSize.x, (window.getSize().x * 0.25f) / textureSize.x });
    // Set Sprite origin to center of texture
    sprite.setOrigin({ textureSize.x / 2.f, textureSize.y / 2.f });
    // Place Sprite in the middle of the window
    sprite.setPosition({ windowSize.x / 2.f, windowSize.y / 2.f });
    
    // Game loop
    while (window.isOpen())
    {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent()))
        {
            //Only handle window close events
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                if (auto keyEvent = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyEvent->code == sf::Keyboard::Key::Escape) // close on Escape
                    {
                        window.close();
                    }
                }
            }

            leftBoard->handleEvent(*event);
            rightBoard->handleEvent(*event);
        }

        // Clear the window with black color
        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);
        window.draw(sprite);
        leftBoard->draw(window);
        rightBoard->draw(window);

        // Display the frame
        window.display();
    }
}

bool SudokuGame::loadFont(const std::string& fontPath)
{
    if (!font.openFromFile(fontPath))
    {
        return false;
    }
    return true;
}

void SudokuGame::loadPuzzles()
{
    puzzles.clear();

    std::ifstream fileReader("Assets/Puzzles/Puzzles.json");
    if (!fileReader.is_open())
    {
        std::cerr << "Failed to open Puzzles.json" << std::endl;
        return;
    }

    nlohmann::json j;

    try {
        fileReader >> j;
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return;
    }



    for (const auto& item : j)
    {
        PuzzleData data;

        std::string difficulty = item.value("difficulty", "easy");
        if (difficulty == "easy")
        {
            data.difficulty = DifficultyLevel::Easy;
        }
        else if (difficulty == "medium")
        {
            data.difficulty = DifficultyLevel::Medium;
        }
        else if (difficulty == "hard")
        {
            data.difficulty = DifficultyLevel::Hard;
        }
        else
        {
            data.difficulty = DifficultyLevel::Easy; // Default to Easy if unknown
        }

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                data.puzzle[row][col] = item["puzzle"][row][col].get<int>();
            }
        }

        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                data.solution[row][col] = item["solution"][row][col].get<int>();
            }
        }


        puzzles.push_back(data);
    }

    std::cout << "Loaded " << puzzles.size() << " puzzles." << std::endl;
}

void SudokuGame::startNewGame(DifficultyLevel selectedDifficulty)
{
    std::vector<size_t> selectedPuzzles;
    for (int i = 0; i < puzzles.size(); ++i)
    {
        if (puzzles[i].difficulty == selectedDifficulty)
        {
            selectedPuzzles.push_back(i);
        }
    }

    if (selectedPuzzles.size() < 2)
    {
        std::cerr << "No Puzzles found for the selected difficulty." << std::endl;
        return;
    }

    // Generate a random seed based on current elapsed seconds since 1970
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

    // Set the uniformly distributed random indices
    std::uniform_int_distribution<std::size_t> dist(0, selectedPuzzles.size() - 1);

    // Generate random indices for both boards
    std::size_t leftIndex = selectedPuzzles[dist(rng)];
    std::size_t rightIndex = selectedPuzzles[dist(rng)];

    // Reroll until rightindex is different from left
    while (rightIndex == leftIndex)
    {
        rightIndex = selectedPuzzles[dist(rng)];
    }

    leftBoard->loadPuzzle(puzzles[leftIndex]);
    rightBoard->loadPuzzle(puzzles[rightIndex]);
}

void SudokuGame::draw(sf::RenderWindow& window)
{
    leftBoard->draw(window);
    rightBoard->draw(window);
}

void SudokuGame::handleEvent(const sf::Event& event)
{
    leftBoard->handleEvent(event);
    rightBoard->handleEvent(event);
}

