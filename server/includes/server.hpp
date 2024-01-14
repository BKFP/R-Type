/**
 * @file server.hpp
 * @brief Server class header file
 * @date 29-11-2023
 */

#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <functional>
#include <unordered_map>

#include "../../engine/includes/network_library/boost_udp.hpp"
#include "../../engine/includes/parser.hpp"
#include "../../engine/includes/ECS.hpp"

/**
 * @brief Server class
 */
class Server {
    public:
        Server(int port) : server_receive(port), _ecs(ECS()) {
            _ecs.registerComponent<Position>();
            _ecs.registerComponent<Rotation>();
            _ecs.registerComponent<Velocity>();
            _ecs.registerComponent<Health>();
            _ecs.registerComponent<Player>();
            _ecs.registerComponent<Enemy>();
            _ecs.registerComponent<Sprite>();
            _ecs.registerComponent<Bullet>();
            _ecs.registerSystem<MovementSystem>();
        }
        ~Server() = default;

        int run();
        void init_entity(std::string data);
        void delete_entity(std::string data);
        int find_free_id_player();
        void init_player(std::vector<std::string> data);
        void parse_data_received();
        std::vector<std::string> split(const std::string& str, const std::string& delim);

        //* Enemies
        void init_enemies();

        //* Levels
        std::shared_ptr<ECS> loadLevel(const std::string& levelConfig, std::shared_ptr<ECS> ecs);
        static std::unordered_map<std::string, std::function<void(ECS::Entity, const std::unordered_map<std::string, std::string>&, std::shared_ptr<ECS>&)>> componentFactories;
        std::shared_ptr<ECS> processJsonObject(const nlohmann::json& jsonObj, std::shared_ptr<ECS> ecs);

    private:
        UDPBoostNetwork::UDPReceiver server_receive;
        std::vector<UDPBoostNetwork::UDPSender> clients_send;
        std::vector<int> clients_send_id;
        ECS _ecs;
        Parser parser;
        std::vector<ECS::Entity> _enemies;
};
