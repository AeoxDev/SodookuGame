#pragma once
#include <SFML/Graphics.hpp>

class Scene
{
private:

public:
	virtual ~Scene() = default;

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
};