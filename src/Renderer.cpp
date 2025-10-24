#include "Renderer.h"
#include <iostream>
#include <sstream>

ImageHandler::ImageHandler(const RayTracing::Vec2u &imageSize) {
    this->imageSize = imageSize;
    // schwarzes Bild erstellen
    this->image = new sf::Image({imageSize.getX(), imageSize.getY()}, sf::Color::Black);
}

sf::Image* ImageHandler::getImage() {
    return image;
}

ImageHandler::~ImageHandler() {
    delete image;
}

bool ImageHandler::saveImage(const std::string &path, RayTracing::Image *imageSrc) {
    if (imageSrc != nullptr) updateImage(imageSrc);
    bool ret = image->saveToFile(path);
    std::cout << "Saved an image to " << path << (ret ? "" : "failed") << std::endl;
    return ret;
}

void ImageHandler::updateImage(RayTracing::Image *imageSrc) {
    for (unsigned x = 0; x < imageSize.getX(); ++x)
        for (unsigned y = 0; y < imageSize.getY(); ++y)
            image->setPixel({x, y}, imageSrc->getPixel(x, y).toSFMLColor());
}


Renderer::Renderer(const RayTracing::Vec2u &windowSize) {
    this->windowSize = windowSize;
    this->imageHandler = new ImageHandler(windowSize);

    init();
}

Renderer::Renderer(const RayTracing::Vec2u &windowSize, ImageHandler *imageHandler) {
    this->windowSize = windowSize;
    this->imageHandler = imageHandler;
    init();
}

void Renderer::init() {
    window = new sf::RenderWindow(sf::VideoMode({windowSize.getX(), windowSize.getX()}), std::string("Raytracer"));
    window->setFramerateLimit(50);

    // Font
    font = new sf::Font();
    if (!font->openFromFile("cnr.otf")) {
        std::cerr << "Failed to load font.\n";
    }
    this->statusText = new sf::Text(*font);
    statusText->setFont(*font);
    statusText->setFillColor(sf::Color::Red);
    statusText->setPosition({10.f, static_cast<float>(windowSize.getY() - 50)});

    // Texture initialisieren
    texture = new sf::Texture(sf::Vector2u{windowSize.getX(), windowSize.getY()});
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
