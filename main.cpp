#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>

int main()
{

    sf::RenderWindow window(sf::VideoMode({ 1600u, 900u }), "Test Window");

    sf::Texture texture;
    if (!texture.loadFromFile("count_dooku.png"))
    {
        std::cerr << "Failed to load texture." << std::endl;
        return 1;
    }

    // Create Sprite with "Textrure"
    sf::Sprite sprite(texture);

    // Set Sprite Scale
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = (window.getSize().x * 0.25f) / textureSize.x;
    float scaleY = scaleX;
    sprite.setScale({ scaleX, scaleY });

    /*
    // Place Sprite in the middle of the window
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect spriteBounds = sprite.getLocalBounds();
    sprite.setPosition(sf::Vector2f(
      windowSize.x / 2.f - spriteBounds.size.x / 2.f,
      windowSize.y / 2.f - spriteBounds.size.y / 2.f
    ));

    */
    // Main loop
    while (window.isOpen())
    {
        std::optional<sf::Event> event;
        while ((event = window.pollEvent()))
        {
            // Only handle window close events
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

        }

        // Clear the window with black color
        window.clear(sf::Color::Black);
        window.draw(sprite);

        // No drawing to avoid shader/graphics exceptions

        // Display the frame
        window.display();
    }

    return 0;
}