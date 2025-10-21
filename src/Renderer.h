#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "Image.hpp"

class ImageHandler {
private:
    sf::Image *image;
    sf::Vector2u imageSize;
public:
    explicit ImageHandler(sf::Vector2u imageSize);
    ~ImageHandler();
    sf::Image *getImage();
    void updateImage(RayTracing::Image* imageSrc);
    bool saveImage(std::string path, RayTracing::Image *imageSrc = nullptr);
};

class Renderer {
private:
    ImageHandler *imageHandler;
    sf::RenderWindow *window{};
    sf::Font *font{};
    sf::Text *statusText{};
    sf::Texture *texture{};
    sf::Sprite *sprite{};

    unsigned fps = 0;
    unsigned sps = 0;

    sf::Vector2u windowSize;

    void init();

public:
    explicit Renderer(sf::Vector2u windowSize);

    Renderer(sf::Vector2u windowSize, ImageHandler *imageHandler);

    [[nodiscard]] bool isOpen() const;

    void processEvents();

    void draw(RayTracing::Image *image);

    void updateFPS(unsigned fps);

    bool saveWindow(const std::string& path);
};
