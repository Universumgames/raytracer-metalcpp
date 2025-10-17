#pragma once

#include <SFML/Graphics.hpp>
#include <array>

class Renderer {
private:
    sf::RenderWindow* window;
    sf::Font* font;
    sf::Text* statusText;
    sf::Image* image;
    sf::Texture *texture;
    sf::Sprite* sprite;

    sf::Vector2u windowSize{0,0};

    unsigned fps = 0;
    unsigned sps = 0;

public:
    explicit Renderer(sf::Vector2u windowSize);
    [[nodiscard]] bool isOpen() const;

    void processEvents();
    void draw();
    void updateFPS(unsigned fps);

};
