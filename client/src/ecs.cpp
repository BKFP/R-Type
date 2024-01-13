/**
 * @file ecs.cpp
 * @brief Implementation of the initialisation of the ECS and its entities
 * @date 09-01-2024
 */

#include "client.hpp"

/**
 * @brief Initialises the ECS
 *
 * @return ECS
 */
ECS rtype::Client::initECS()
{
    ECS ecs;
    ecs.registerComponent<Position>();
    ecs.registerComponent<Velocity>();
    ecs.registerComponent<Rotation>();
    ecs.registerComponent<Health>();
    ecs.registerComponent<Sprite>();
    ecs.registerComponent<Player>();
    ecs.registerComponent<Rotation>();
    return ecs;
}

/**
 * @brief Initialises the player and add it to the ECS
 *
 */
void rtype::Client::initPlayer(std::vector<std::string> data_split)
{
    if (data_split.at(1) == "player") {
        _ecs.createEntity();
        _ecs.addComponent<Position>(_ecs.getEntities().back(), {100, 100});
        _ecs.addComponent<Health>(_ecs.getEntities().back(), 100);
        _ecs.addComponent<Velocity>(_ecs.getEntities().back(), {1, 1, 2});
        std::string texture = "player_red";
        _ecs.addComponent<Sprite>(_ecs.getEntities().back(), {texture, 34, 34, 0, 0, 3});
        _ecs.addComponent<Player>(_ecs.getEntities().back(), {stoi(data_split.at(2)), data_split.at(3)});
        _ecs.addComponent<Rotation>(_ecs.getEntities().back(), {180});
        if (data_split.size() > 4) {
            _player = _ecs.getEntities().back();
            id = stoi(data_split.at(2));
        }
    }

}

/**
 * @brief delete entity player
 *
 */
void rtype::Client::deletePlayer(std::vector<std::string> data)
{
    if (data.at(1) == "player") {
        int playerIdToDelete = std::stoi(data.at(2));
        for (auto& entity : _ecs.getEntities()) {
            if (_ecs.hasComponent<Player>(entity)) {
                if (_ecs.getComponent<Player>(entity)->id == playerIdToDelete) {
                    _ecs.removeEntity(entity);
                    break;
                }
            }
        }
    }
}

/**
 * @brief nb of players
 *
 */
int rtype::Client::nbPlayersInRoom()
{
    int nb = 0;
    for (auto& entity : _ecs.getEntities()) {
        if (_ecs.hasComponent<Player>(entity)) {
            nb++;
        }
    }
    return nb;
}
