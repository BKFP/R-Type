/**
 * @file IGraphics.hpp
 * @brief Declaration of the IGraphics interface
 * @date 18-12-2023
 */


/**
 * @brief The IGraphics interface
 * 
 */
#include "components.hpp"

class IGraphics {
public:
    virtual ~IGraphics() {}
    virtual void initWindow(int width, int height, const std::string& title) = 0;
    virtual void drawSprite(const Sprite& sprite) = 0;
    virtual void renderText(const Text& text) = 0;
    // Add other necessary methods
};
