#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "Image.hpp"

/// Handles image creation, updating, and saving
class ImageHandler {
private:
    sf::Image *image;
    RayTracing::Vec2u imageSize;

public:
    explicit ImageHandler(const RayTracing::Vec2u &imageSize);

    ~ImageHandler();

    /// Returns the SFML image
    sf::Image *getImage();

    /// Updates the SFML image with data from the given RayTracing::Image
    void updateImage(RayTracing::Image *imageSrc);

    /**
     * Save the image to the specified path
     * @param path path to save the image
     * @param imageSrc optional RayTracing::Image to save, if nullptr uses internal image
     * @return true if saved successfully, false otherwise
     */
    bool saveImage(const std::string &path, RayTracing::Image *imageSrc = nullptr);
};

/// Handles rendering the image to a window
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

    RayTracing::Vec2u windowSize;

    /// Initializes the renderer
    void init();

public:
    explicit Renderer(const RayTracing::Vec2u &windowSize);

    Renderer(const RayTracing::Vec2u &windowSize, ImageHandler *imageHandler);

    /// window is open
    [[nodiscard]] bool isOpen() const;

    /// process window events
    void processEvents();

    /**
     * Draws the given image to the window
     * @param image image to draw
     */
    void draw(RayTracing::Image *image);

    /// Updates the FPS display
    void updateFPS(unsigned fps);

    /// Save the current window content to the specified path
    bool saveWindow(const std::string &path);
};