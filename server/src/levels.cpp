/**
 * @file levels.cpp
 * @brief Declaration of the levels class methods
 * @date 08-01-2024
 */

#include "levels.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

/**
 * @brief Create a Spawn Time Component object
 * 
 * @param entity
 * @param params
 * @param ecs
 */
void createSpawnTimeComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("time") != params.end()) {
        float time = std::stof(params.at("time"));
        ecs->addComponent(entity, SpawnTime{time});
    } else {
        std::cerr << "SpawnTime component requires 'Time' parameter." << std::endl;
    }
}

/**
 * @brief Create a Player Component object
 *
 * @param entity
 * @param params
 * @param ecs
 */
void createPlayerComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("id") != params.end() && params.find("name") != params.end()) {
        int id = std::stoi(params.at("id"));
        std::string name = params.at("name");
        ecs->addComponent(entity, Player{id, name});
    } else {
        std::cerr << "Position component requires both 'X' and 'Y' parameters." << std::endl;
    }
}

/**
 * @brief Create an enemy component
 * 
 */
void createEnemyComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("name") != params.end()) {
        std::string name = params.at("name");
        ecs->addComponent(entity, Enemy{name});
    } else {
        std::cerr << "Enemy component requires 'Name' parameter." << std::endl;
    }
}

/**
 * @brief Create a Position Component object
 *
 * @param entity
 * @param params
 * @param ecs
 */
void createPositionComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("x") != params.end() && params.find("y") != params.end()) {
        float x = std::stof(params.at("x"));
        float y = std::stof(params.at("y"));
        ecs->addComponent(entity, Position{x, y});
    } else {
        std::cerr << "Position component requires both 'X' and 'Y' parameters." << std::endl;
    }
}

/**
 * @brief Create a Sprite Component object
 *
 * @param entity
 * @param params
 * @param ecs
 */
void createSpriteComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("texture") != params.end() && params.find("width") != params.end() && params.find("height") != params.end() && params.find("startX") != params.end() && params.find("startY") != params.end() && params.find("scale") != params.end()) {
        std::string texture = params.at("texture");
        int width = std::stoi(params.at("width"));
        int height = std::stoi(params.at("height"));
        int startX = std::stoi(params.at("startX"));
        int startY = std::stoi(params.at("startY"));
        float scale = std::stof(params.at("scale"));
        ecs->addComponent(entity, Sprite{texture, width, height, startX, startY, scale});
    } else {
        std::cerr << "Sprite component requires 'Texture', 'StartX', 'StartY', 'Width' and 'Height' parameters." << std::endl;
    }
}


/**
 * @brief Create a Velocity Component object
 *
 * @param entity
 * @param params
 * @param ecs
 */
void createVelocityComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("x") != params.end() && params.find("y") != params.end() && params.find("speed") != params.end()) {
        float x = std::stof(params.at("x"));
        float y = std::stof(params.at("y"));
        float speed = std::stof(params.at("speed"));
        ecs->addComponent(entity, Velocity{x, y, speed});
    } else {
        std::cerr << "Velocity component requires 'X', 'Y' and 'Speed' parameters." << std::endl;
    }
}

/**
 * @brief Create a Rotation Component object
 *
 * @param entity
 * @param params
 * @param ecs
 */
void createRotationComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("angle") != params.end()) {
        float angle = std::stof(params.at("angle"));
        ecs->addComponent(entity, Rotation{angle});
    } else {
        std::cerr << "Rotation component requires 'Angle' parameter." << std::endl;
    }
}

/**
 * @brief Create a Health Component object
 *
 * @param entity
 * @param params
 * @param ecs
 */
void createHealthComponent(ECS::Entity entity, const std::unordered_map<std::string, std::string>& params, std::shared_ptr<ECS>& ecs) {
    if (params.find("hp") != params.end()) {
        int hp = std::stoi(params.at("hp"));
        ecs->addComponent(entity, Health{hp});
    } else {
        std::cerr << "Health component requires 'HP' parameter." << std::endl;
    }
}

/**
 * @brief Loads a JSON file
 *
 * @param filename
 * @return nlohmann::json
 */
nlohmann::json loadJsonData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    nlohmann::json jsonArray;
    file >> jsonArray;
    return jsonArray;
}

/**
 * @brief Processes a JSON object and creates the corresponding entity and components
 *
 * @param jsonObj
 * @param ecs
 * @return std::shared_ptr<ECS>
 */
std::shared_ptr<ECS> processJsonObject(const nlohmann::json& jsonObj, std::shared_ptr<ECS> ecs) {
    auto entity = ecs->createEntity();
    for (const auto& element : jsonObj.items()) {
        const auto& componentType = element.key();
        const auto& componentData = element.value();
        if (Levels::componentFactories.find(componentType) != Levels::componentFactories.end()) {
            std::unordered_map<std::string, std::string> params;
            if (componentData.is_primitive()) {
                params[componentType] = componentData.get<std::string>();
            } else {
                for (const auto& nestedElement : componentData.items()) {
                    params[nestedElement.key()] = nestedElement.value().get<std::string>();
                }
            }
            Levels::componentFactories[componentType](entity, params, ecs);
        }
    }
    return (ecs);
}

/**
 * @brief Loads a level from a JSON file
 *
 * @param levelConfig
 * @param ecs
 * @return std::shared_ptr<ECS>
 */
std::shared_ptr<ECS> Levels::loadLevel(const std::string& levelConfig, std::shared_ptr<ECS> ecs) {
    try {
        auto jsonArray = loadJsonData(levelConfig);
        for (const auto& jsonObj : jsonArray) {
            ecs = processJsonObject(jsonObj, ecs);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading level: " << e.what() << std::endl;
    }
    return (ecs);
}

/**
 * @brief Component factories
 *
 * @note This is a map of component types to functions that create the component
 */
std::unordered_map<std::string, std::function<void(ECS::Entity, const std::unordered_map<std::string, std::string>&, std::shared_ptr<ECS>&)>> Levels::componentFactories = {
    {"Player", createPlayerComponent},
    {"Position", createPositionComponent},
    {"Velocity", createVelocityComponent},
    {"Rotation", createRotationComponent},
    {"Health", createHealthComponent},
    {"Sprite", createSpriteComponent},
    {"Enemy", createEnemyComponent},
    {"SpawnTime", createSpawnTimeComponent}
};
