#pragma once
#include <SFML/Graphics.hpp>
#include "src/player/buildModeType/buildModeType.hpp"

class WorldManager;

class Player {
public:
    Player();

    // POSIION
    void setPosition(const sf::Vector2f position) noexcept { m_position = position; }
    sf::Vector2f getPosition() const noexcept { return m_position; }

    // SPEED
    float getSpeed() const noexcept { return m_speed; }
    void setSpeed(float speed) noexcept { m_speed = speed; }

    // OTHER
    void handleEvent(const sf::Event& event, sf::RenderWindow& window, WorldManager& worldManager) noexcept;
    void update(float deltaTime) noexcept;
    void interpolate(float interpolationFactor) noexcept;
    
    void draw(sf::RenderTarget& target) const;

    // MEHANICS
    void placeTile(sf::RenderWindow& window, WorldManager& worldManager) noexcept;
    void breakTile(sf::RenderWindow& window, WorldManager& worldManager) noexcept;
private:
    sf::Vector2f m_previousPosition;
    sf::Vector2f m_position;
    sf::Vector2f m_visualPosition;
    float m_speed{900.0f};

    mutable sf::RectangleShape m_shape;
    
    static constexpr float HEIGHT = 55.0f;
    static constexpr float WIDTH = 25.0f;

    BuildModeType buildMode = BuildModeType::Block;
};
