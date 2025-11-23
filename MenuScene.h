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
	std::optional<sf::Text> easyText;
	std::optional<sf::Text> mediumText;
	std::optional<sf::Text> hardText;
	std::function<void(int)> startGameCallback;

public:
	MenuScene(std::function<void(int)> callback) : startGameCallback(callback)
	{
		if (!font.openFromFile("fonts/arialbd.ttf"))
		{
			std::cout << "Failed to load font in MenuScene." << std::endl;
			return;
		}

		easyText.emplace("Easy", font, 48);
		easyText->setPosition({ 600, 300 });

		mediumText.emplace("Medium", font, 48);
		mediumText->setPosition({ 600, 400 });

		hardText.emplace("Hard", font, 48);
		hardText->setPosition({ 600, 500 });
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
		if (event.is(std::optional<sf::)
		{

		}
	}
};