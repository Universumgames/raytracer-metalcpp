#include "Renderer.h"
#include <iostream>
#include <sstream>

Renderer::Renderer(sf::Vector2u windowSize) {
    this->windowSize = windowSize;

    window = new sf::RenderWindow(sf::VideoMode(windowSize), std::string("Game of Life"));
    window->setFramerateLimit(50);

    // Font
    font = new sf::Font();
    if (!font->openFromFile("cnr.otf")) {
        std::cerr << "Failed to load font.\n";
    }
    this->statusText = new sf::Text(*font);
    statusText->setFont(*font);
    statusText->setFillColor(sf::Color::Red);
    statusText->setPosition({10.f, static_cast<float>(windowSize.y - 50)});

    // schwarzes Bild erstellen
    image = new sf::Image(windowSize, sf::Color::Black);

    // Texture initialisieren
    texture = new sf::Texture(windowSize);
    texture->update(*image);
    sprite = new sf::Sprite(*texture);
}

bool Renderer::isOpen() const {
    return window->isOpen();
}

void Renderer::processEvents() {
    std::optional<sf::Event> eventOpt;
    while ((eventOpt = window->pollEvent()).has_value()) {
        sf::Event event = eventOpt.value();
        if (event.is<sf::Event::Closed>())
            window->close();
    }
}

void Renderer::updateFPS(unsigned fps) {
    this->fps = fps;
}



void Renderer::draw() {

}