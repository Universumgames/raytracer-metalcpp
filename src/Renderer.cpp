#include "Renderer.h"
#include <iostream>
#include <sstream>

ImageHandler::ImageHandler(sf::Vector2u imageSize) {
    this->imageSize = imageSize;
    // schwarzes Bild erstellen
    this->image = new sf::Image(imageSize, sf::Color::Black);
}

sf::Image* ImageHandler::getImage() {
    return image;
}

ImageHandler::~ImageHandler() {
    delete image;
}

bool ImageHandler::saveImage(std::string path, RayTracing::Image *imageSrc) {
    if (imageSrc != nullptr) updateImage(imageSrc);
    return image->saveToFile(path);
}

void ImageHandler::updateImage(RayTracing::Image *imageSrc) {
    for (unsigned x = 0; x < imageSize.x; ++x)
        for (unsigned y = 0; y < imageSize.y; ++y)
            image->setPixel({x, y}, imageSrc->getPixel(x, y).toSFMLColor());
}


Renderer::Renderer(sf::Vector2u windowSize) {
    this->windowSize = windowSize;
    this->imageHandler = new ImageHandler(windowSize);

    init();
}

Renderer::Renderer(sf::Vector2u windowSize, ImageHandler *imageHandler) {
    this->windowSize = windowSize;
    this->imageHandler = imageHandler;
    init();
}

void Renderer::init() {
    window = new sf::RenderWindow(sf::VideoMode(windowSize), std::string("Raytracer"));
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

    // Texture initialisieren
    texture = new sf::Texture(windowSize);
    texture->update(*imageHandler->getImage());
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

void Renderer::draw(RayTracing::Image *imageSrc) {
    imageHandler->updateImage(imageSrc);

    texture->update(*imageHandler->getImage());
    window->clear(sf::Color::Black);
    window->draw(*sprite);

    std::ostringstream ss;
    statusText->setString(ss.str());
    window->draw(*statusText);
    window->display();
}

bool Renderer::saveWindow(const std::string& path) {
    return imageHandler->saveImage(path);
}
