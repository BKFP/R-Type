/**
 * @file connection.cpp
 * @brief Handles the rendering of the connection scene
 * @date 09-01-2024
 */

#include "client.hpp"

/**
 * @brief Draws the connection screen
 *
 */
void rtype::Client::drawConnection()
{
    _graphical->clear();
    _graphical->drawSprite("connection", 0, 0, 0, 0, 1, 0, 1920, 1080);
    _graphical->drawText("CONNECTION", 750, 325, 50);
    _graphical->drawText("IP ADDRESS", 130, 600, 50);
    _graphical->drawSprite("input_frame", 640, 580, 0, 0, 1, 0, 865, 90);
    if (_input_frames_state.at(1).first)
        _graphical->drawText(_input_frames_state.at(1).second + "_", 660, 600, 50);
    else
        _graphical->drawText(_input_frames_state.at(1).second, 660, 600, 50);
    _graphical->drawText("USERNAME", 140, 740, 50);
    _graphical->drawSprite("input_frame", 640, 720, 0, 0, 1, 0, 865, 90);
    if (_input_frames_state.at(0).first)
        _graphical->drawText(_input_frames_state.at(0).second + "_", 660, 740, 50);
    else
        _graphical->drawText(_input_frames_state.at(0).second, 660, 740, 50);
    _graphical->drawText("ENTER", 850, 900, 50);
    _graphical->display();
}
