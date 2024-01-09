/**
 * @file textures.cpp
 * @brief
 * @date 09-01-2024
 */

#include "sfml.hpp"

#include "player_red.h"
#include "parallax100.h"
#include "parallax80.h"
#include "parallax60.h"
#include "outerSpace.h"
#include "spaceGarage.h"
#include "logo.h"
#include "r_type_font.h"

/**
 * @brief Initialises the textures for the client's sprites
 *
 * @param name std::string name of the sprite
 * @param sheet unsigned char const * sprite sheet
 * @param size unsigned int size of the sprite sheet
 */
void rtype::SFML::initTextures(std::string name, unsigned char const *sheet, unsigned int size)
{
    textures.push_back(std::make_pair(name, new sf::Texture()));
    if (!textures.back().second->loadFromMemory(sheet, size)) {
        std::cerr << "Error loading texture" << std::endl;
        exit(84);
    }
    sprites.push_back(std::make_pair(name, new sf::Sprite(*textures.back().second)));
}

/**
 * @brief Loads the textures for the client's sprites
 */
void rtype::SFML::loadAssets()
{
    initTextures("logo", logo, logo_len);
    initTextures("outerSpace", outerSpace, outerSpace_len);
    initTextures("spaceGarage", spaceGarage, spaceGarage_len);
    initTextures("parallax100", parallax100, parallax100_len);
    initTextures("parallax80", parallax80, parallax80_len);
    initTextures("parallax60", parallax60, parallax60_len);
    initTextures("player_red", player_red, player_red_len);

    if (!font.loadFromMemory(r_type_font, r_type_font_len)) {
        std::cerr << "Error loading font" << std::endl;
        exit(84);
    }
    text.setFont(font);
}
