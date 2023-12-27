/**
 * @file client.hpp
 * @brief Client class header file
 * @date 29-11-2023
 */

# pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "network.hpp"
#include "../../engine/includes/ECS.hpp"
#include <ctime> // for time()


namespace rtype
{
    const std::string WINDOW_NAME = "R-Type";
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    class Client {
        public:
            Client();
            ~Client();

            void run(Network::Sender sender, Network::Receive& receive, int port);
            void loadTextures();
            void drawParallax(sf::RenderWindow &window);
            ECS initECS();

        private:
            sf::Texture playerTexture;
            sf::Texture parallaxTexture1;
            sf::Texture parallaxTexture2;
            sf::Texture parallaxTexture3;

            std::vector<sf::Sprite> playersSprites;
            sf::Sprite planeSprite;
            sf::Sprite parallaxSprite1;
            sf::Sprite parallaxSprite1b;
            sf::Sprite parallaxSprite2;
            sf::Sprite parallaxSprite2b;
            sf::Sprite parallaxSprite3;
            sf::Sprite parallaxSprite3b;
    };
}
