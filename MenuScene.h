#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include <iostream>

class MenuScene : public Scene
{
private:
	sf::Font font;
	/*
	std::optional<sf::Text> easyText;
	std::optional<sf::Text> mediumText;
	std::optional<sf::Text> hardText;
	*/
	/*
	std::unique_ptr<sf::Text> easyText;
	std::unique_ptr<sf::Text> mediumText;
	std::unique_ptr<sf::Text> hardText;
	*/

	sf::Text* easyText;
	sf::Text* mediumText;
	sf::Text* hardText;

	std::function<void(int)> startGameCallback;

public:
	MenuScene(std::function<void(int)> callback) : startGameCallback(callback)
	{
		if (!font.openFromFile("fonts/arialbd.ttf"))
		{
			std::cout << "Failed to load font in MenuScene." << std::endl;
			return;
		}

		easyText = new sf::Text(font);
		mediumText = new sf::Text(font);
		hardText = new sf::Text(font);

		/*
		cellTexts[row][col] = new sf::Text(font);
		cellTexts[row][col]->setFont(font);
		cellTexts[row][col]->setString("");             // empty at start
		cellTexts[row][col]->setCharacterSize(24);      // size in pixels
		cellTexts[row][col]->setFillColor(sf::Color::White);
		*/
	}

	void update(float dt) override {}
	void draw(sf::RenderWindow& window) override
	{
		window.draw(*easyText);
		window.draw(*mediumText);
		window.draw(*hardText);
	}
	void handleEvent(const sf::Event& event) override
	{
		if (auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>())
		{
			sf::Vector2f mousePos((float)mouseEvent->position.x, (float)mouseEvent->position.y);

			// Change color on hover
			easyText->setFillColor(easyText->getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
			mediumText->setFillColor(mediumText->getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);
			hardText->setFillColor(hardText->getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::White);

			if (easyText->getGlobalBounds().contains(mousePos))
				startGameCallback(0); // Easy
			else if (mediumText->getGlobalBounds().contains(mousePos))
				startGameCallback(1); // Medium
			else if (hardText->getGlobalBounds().contains(mousePos))
				startGameCallback(2); // Hard
		}
	}

	void onEnter() override {};
	void onExit() override {};
};