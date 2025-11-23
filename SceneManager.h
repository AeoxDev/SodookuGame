#pragma once
#include "Scene.h"
#include <SFML/Window.hpp>

class SceneManager
{
private:
	std::shared_ptr<Scene> currentScene;
public:
	void setScene(std::shared_ptr<Scene> scene)
	{
		if (currentScene)
		{
			currentScene->onExit();
		}

		currentScene = scene;

		if (currentScene)
		{
			currentScene->onEnter();
		}
	}

	void update(float dt)
	{
		if (currentScene)
		{
			currentScene->update(dt);
		}
	}
	void draw(sf::RenderWindow& window)
	{
		if (currentScene)
		{
			currentScene->draw(window);
		}
	}

	void handleEvent(const sf::Event event)
	{
		if (currentScene)
		{
			currentScene->handleEvent(event);
		}
	}


};