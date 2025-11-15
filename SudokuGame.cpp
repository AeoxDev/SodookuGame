#include "SudokuGame.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

SudokuGame::SudokuGame()
{
	if (!loadFont("fonts/arialbd.ttf"))
	{
		std::cerr << "Failed to load font." << std::endl;
	}

	leftBoard.emplace(95.f, 260.f, 400.f, font);
    rightBoard.emplace(1100.f, 265.f, 400.f, font);

	loadPuzzles();
	startNewGame();
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
    // For simplicity, we hardcode some puzzles here.
    // In a real application, you might want to load these from a file.
    puzzles = {
        {
            {
                {5,3,0,0,7,0,0,0,0},
                {6,0,0,1,9,5,0,0,0},
                {0,9,8,0,0,0,0,6,0},
                {8,0,0,0,6,0,0,0,3},
                {4,0,0,8,0,3,0,0,1},
                {7,0,0,0,2,0,0,0,6},
                {0,6,0,0,0,0,2,8,0},
                {0,0,0,4,1,9,0,0,5},
                {0,0,0,0,8,0,0,7,9}
            },
            {
                {0,5,0,0,7,0,0,0,2},
                {0,0,0,1,9,5,0,0,0},
                {0,9,8,0,0,0,6,0,0},
                {8,0,0,0,6,0,0,0,3},
                {4,0,0,8,0,3,0,0,1},
                {7,0,0,0,2,0,0,0,6},
                {0,6,0,0,0,0,2,8,0},
                {0,0,0,4,1,9,0,0,0},
                {2,0,0,0,8,0,0,7,0}
            }
        },
        // Add more puzzles as needed
    };
}

void SudokuGame::startNewGame()
{
    if (puzzles.empty())
        return;
    // Randomly select a puzzle
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<std::size_t> dist(0, puzzles.size() - 1);
    std::size_t leftIndex = dist(rng);
    std::size_t rightIndex = dist(rng);
    leftBoard->loadPuzzle(puzzles[leftIndex].first, puzzles[leftIndex].second);
    rightBoard->loadPuzzle(puzzles[rightIndex].first, puzzles[rightIndex].second);
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

