#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1600u, 900u }), "Sudoku Game");
    SceneManager manager;

    // Create menu scene with callback to switch to game
    auto menu = std::make_shared<MenuScene>([&manager, &window](int difficulty)
        {
            manager.setScene(std::make_shared<GameScene>(difficulty, window));
        });

    manager.setScene(menu);

    sf::Clock clock;
    while (window.isOpen())
    {
        // Poll events using new SFML 3 API (returns std::optional<sf::Event>)
        while (std::optional event = window.pollEvent())
        {
            // Access the event via *event or event->xxxx
            if (event->is<sf::Event::Closed>())
                window.close();

            manager.handleEvent(*event); // pass the event to your Scene manager
        }

        float dt = clock.restart().asSeconds();
        manager.update(dt);

        window.clear();
        manager.draw(window);
        window.display();
    }
}