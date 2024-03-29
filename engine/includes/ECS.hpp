/**
 * @file ECS.hpp
 * @brief Declaration of the ECS manager class
 * @date 05-12-2023
 */

#pragma once

#include "components.hpp"

#include <map>
#include <memory>
#include <typeindex>
#include <vector>
#include <iostream>

class ECS;

/**
 * @brief The interface for a system
 */
class ISystem {
    public:
        virtual void update(ECS& ecs) = 0;
};

/**
 * @brief The Entity Component System manager class
 */
class ECS {
    public:
        using Entity = size_t;

    private:
        std::vector<std::map<Entity, std::shared_ptr<void>>> _components;
        std::map<std::type_index, size_t> _componentTypeToIndex;
        std::vector<std::unique_ptr<ISystem>> _systems;
        Entity _nextEntityID = 0;

    public:

        /**
         * @brief Creates a new entity
         *
         * @return ECS::Entity
         */
        Entity createEntity()
        {
            return _nextEntityID++;
        };

        /**
         * @brief Registers a new component type and adds it to the component list
         *
         * @tparam T The component type, defined in components.hpp
         */
        template<typename T>
        void registerComponent()
        {
            _componentTypeToIndex[std::type_index(typeid(T))] = _components.size();
            _components.emplace_back();
        };

        /**
         * @brief Adds a component to an entity
         *
         * @tparam T The component type, defined in components.hpp
         * @param entity The entity to add the component to
         * @param component The component to add
         */
        template<typename T>
        void addComponent(Entity entity, T component)
        {
            auto componentIndex = _componentTypeToIndex[std::type_index(typeid(T))];
            _components[componentIndex][entity] = std::make_shared<T>(component);
        };
        template<typename ...T> void addComponent(Entity entity, T... components)
        {
            (addComponent(entity, components), ...);
        }

        /**
         * @brief Removes a component from an entity
         *
         * @tparam T The component type, defined in components.hpp
         * @param entity The entity to remove the component from
         */
        template <typename T>
        void removeComponent(Entity entity)
        {
            auto index = _componentTypeToIndex[std::type_index(typeid(T))];
            _components[index].erase(entity);
        }

        /**
         * @brief Removes a component from an entity
         *
         * @tparam T The component type, defined in components.hpp
         * @param entity The entity to remove the component from
         */
        template<typename T>
        bool hasComponent(Entity entity)
        {
            auto index = _componentTypeToIndex[std::type_index(typeid(T))];
            auto it = _components[index].find(entity);
            return it != _components[index].end();
        };

        /**
         * @brief Gets a component from an entity
         *
         * @tparam T The component type, defined in components.hpp
         * @param entity The entity to get the component from
         * @return T* A pointer to the component
         */
        template<typename T>
        T* getComponent(Entity entity)
        {
            auto index = _componentTypeToIndex[std::type_index(typeid(T))];
            auto it = _components[index].find(entity);
            if (it != _components[index].end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        };

        /**
         * @brief Gets all the entities in the ECS
         *
         * @return std::vector<Entity> A vector of all the entities
         */
        std::vector<Entity> getEntities() {
            std::vector<Entity> entities;
            for (Entity i = 0; i < _nextEntityID; i++) {
                entities.push_back(i);
            }
            return entities;
        }

        /**
         * @brief Removes an entity from the ECS
         *
         * @param entity The entity to remove
         */
        void removeEntity(Entity entity) {
            for (auto& component : _components) {
                component.erase(entity);
            }
        }

        /**
         * @brief Gets all the entities that are drawable
         *
         * @return std::vector<Entity> A vector of all the entities
         */
        std::vector<Entity> getDrawableEntities() {
            std::vector<Entity> entities = getEntities();
            std::vector<Entity> drawableEntities;
            for (int i = 0; i < entities.size(); i++) {
                if (hasComponent<Position>(entities[i]) && hasComponent<Sprite>(entities[i])) {
                    drawableEntities.push_back(entities[i]);
                }
            }
            return drawableEntities;
        }

        /**
         * @brief Gets all the entities that are enemies
         *
         * @return std::vector<Entity> A vector of all the entities
         */
        std::vector<Entity> getEnemies() {
            std::vector<Entity> entities = getEntities();
            std::vector<Entity> enemies;
            for (int i = 0; i < entities.size(); i++) {
                if (hasComponent<Enemy>(entities[i])) {
                    enemies.push_back(entities[i]);
                }
            }
            return enemies;
        }

        /**
         * @brief Gets all the entities that are collidable
         *
         * @return std::vector<Entity> A vector of all the entities
         */
        std::vector<Entity> getCollidableEntities() {
            std::vector<Entity> entities = getEntities();
            std::vector<Entity> collidableEntities;
            for (int i = 0; i < entities.size(); i++) {
                if (hasComponent<Position>(entities[i]) && hasComponent<Sprite>(entities[i]) && hasComponent<Collision>(entities[i])) {
                    collidableEntities.push_back(entities[i]);
                }
            }
            return collidableEntities;
        }

        /**
         * @brief Registers a new system
         *
         * @tparam T The system type
         * @tparam Args The arguments to pass to the system constructor
         * @param args The arguments to pass to the system constructor
         */
        template<typename T, typename... Args>
        void registerSystem(Args&&... args)
        {
            _systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        /**
         * @brief Updates all the registered systems
         *
         */
        void updateSystems()
        {
        for (auto& system : _systems) {
            system->update(*this);
        }
    }

    /**
     * @brief Updates a component, if it exists
     *
     * @tparam T The system type
     * @tparam Func The function type
     * @param entity The entity to update
     * @param updater The function to call on the component. eg: { position.x += 1; position.y *= 2;}
     */
    template<typename T, typename Func>
    void updateComponent(Entity entity, Func updater) {
        T* component = getComponent<T>(entity);
        if (component) {
            updater(*component);
        }
    }
};

/**
 * @brief The movement system. Updates the position of based on their velocity
 */
class MovementSystem : public ISystem {
    public:
        void update(ECS& ecs) override
        {
            for (auto entity: ecs.getEntities()) {
                if ((ecs.hasComponent<Position>(entity)) && (ecs.hasComponent<Velocity>(entity)) && (!ecs.hasComponent<Freeze>(entity))) {
                    auto position = ecs.getComponent<Position>(entity);
                    auto velocity = ecs.getComponent<Velocity>(entity);
                    position->x += velocity->x;
                    position->y += velocity->y;
                }
            }
        }
};

/**
 * @brief The immunity system. Updates the immunity frames of entities
 */
class ImmunitySystem : public ISystem {
    public:
        void update(ECS& ecs) override
        {
            for (auto entity: ecs.getEntities()) {
                if (ecs.hasComponent<Immunity>(entity) && (!ecs.hasComponent<Freeze>(entity))) {
                    auto immunity = ecs.getComponent<Immunity>(entity);
                    if (immunity->frames > 0) {
                        immunity->frames--;
                    }
                    if (immunity->frames == 0) {
                        ecs.removeComponent<Immunity>(entity);
                    }
                }
            }
        }
};

/**
 * @brief The damage system. Updates the health of entities based on the damage they take
 */
class DamageSystem : public ISystem {
    public:
        void update(ECS& ecs) override
        {
            for (auto entity: ecs.getEntities()) {
                if ((ecs.hasComponent<Health>(entity)) && (ecs.hasComponent<Damage>(entity)) && (!ecs.hasComponent<Immunity>(entity)) && (!ecs.hasComponent<Freeze>(entity))) {
                    auto health = ecs.getComponent<Health>(entity);
                    auto damage = ecs.getComponent<Damage>(entity);
                    if (!ecs.hasComponent<Immunity>(entity))
                    {
                        health->hp -= damage->damage;
                        ecs.addComponent<Immunity>(entity, {60});
                        ecs.removeComponent<Damage>(entity);
                    }
                }
            }
        }
};

/**
 * @brief The collision system. Checks for collisions between entities
 */
class CollisionSystem : public ISystem {
public:
    void update(ECS& ecs) override {
        auto entities = ecs.getCollidableEntities();
        for (size_t i = 0; i < entities.size(); ++i) {
            if (ecs.hasComponent<Freeze>(entities[i])) {
                continue;
            }
            for (size_t j = i + 1; j < entities.size(); ++j) {
                if (ecs.hasComponent<Freeze>(entities[j])) {
                    continue;
                }
                checkCollision(ecs, entities[i], entities[j]);
            }
        }
    }

private:
    /**
     * @brief checks the collision of collidable entities
     * 
     * @param ecs 
     * @param entity1 
     * @param entity2 
     */
    void checkCollision(ECS& ecs, ECS::Entity entity1, ECS::Entity entity2) {
        auto pos1 = ecs.getComponent<Position>(entity1);
        auto sprite1 = ecs.getComponent<Sprite>(entity1);
        auto pos2 = ecs.getComponent<Position>(entity2);
        auto sprite2 = ecs.getComponent<Sprite>(entity2);

        if (pos1->x < pos2->x + sprite2->width * sprite2->scale &&
            pos1->x + sprite1->width * sprite1->scale > pos2->x &&
            pos1->y < pos2->y + sprite2->height * sprite2->scale &&
            pos1->y + sprite1->height * sprite1->scale > pos2->y) {
                handleCollision(ecs, entity1, entity2);
        }
    }

    void print_all_ecs_entity(ECS& ecs)
    {
        std::cout << "--- ECS ---" << std::endl;
        for (auto& entity : ecs.getEntities()) {
            std::cout << "Entity: " << entity << std::endl;
            if (ecs.hasComponent<Bullet>(entity)) {
                std::cout << "Bullet: " << ecs.getComponent<Bullet>(entity)->id << " " << ecs.getComponent<Bullet>(entity)->team << std::endl;
            }
            if (ecs.hasComponent<Attack>(entity)) {
                std::cout << "Attack: " << ecs.getComponent<Attack>(entity)->damage << std::endl;
            }
            if (ecs.hasComponent<Freeze>(entity)) {
                std::cout << "Freeze: " << ecs.getComponent<Freeze>(entity)->frames << std::endl;
            }
            if (ecs.hasComponent<Position>(entity)) {
                std::cout << "Position: " << ecs.getComponent<Position>(entity)->x << " " << ecs.getComponent<Position>(entity)->y << std::endl;
            }
            if (ecs.hasComponent<Rotation>(entity)) {
                std::cout << "Rotation: " << ecs.getComponent<Rotation>(entity)->angle << std::endl;
            }
            if (ecs.hasComponent<Velocity>(entity)) {
                std::cout << "Velocity: " << ecs.getComponent<Velocity>(entity)->x << " " << ecs.getComponent<Velocity>(entity)->y << std::endl;
            }
            if (ecs.hasComponent<Health>(entity)) {
                std::cout << "Health: " << ecs.getComponent<Health>(entity)->hp << std::endl;
            }
            if (ecs.hasComponent<Player>(entity)) {
                std::cout << "Player: " << ecs.getComponent<Player>(entity)->id << " " << ecs.getComponent<Player>(entity)->name << std::endl;
            }
            if (ecs.hasComponent<Sprite>(entity)) {
                std::cout << "Sprite: " << ecs.getComponent<Sprite>(entity)->texture << " " << ecs.getComponent<Sprite>(entity)->width << " " << ecs.getComponent<Sprite>(entity)->height << " " << ecs.getComponent<Sprite>(entity)->scale << std::endl;
            }
            if (ecs.hasComponent<Enemy>(entity)) {
                std::cout << "Enemy: " << ecs.getComponent<Enemy>(entity)->name << std::endl;
            }
            if (ecs.hasComponent<SpawnTime>(entity)) {
                std::cout << "SpawnTime: " << ecs.getComponent<SpawnTime>(entity)->time << std::endl;
            }

            std::cout << std::endl;
        }
        std::cout << "-----------" << std::endl;
    }
    void handleCollision(ECS& ecs, ECS::Entity entity1, ECS::Entity entity2) {
        print_all_ecs_entity(ecs);
        if (ecs.hasComponent<Enemy>(entity1) && ecs.hasComponent<Player>(entity2)) {
           std::cout << "Enemy collided with player" << std::endl;
        } else if (ecs.hasComponent<Enemy>(entity2) && ecs.hasComponent<Bullet>(entity1)) {
            std::cout << "Bullet collided with enemy" << std::endl;
        } else {
            std::cout << "Unknown collision between entity " << entity1 << " and " << entity2 << std::endl;
        }
    }
};
