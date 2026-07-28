#include <SFML/Graphics.hpp>
#include "src/player/Player.hpp"
#include "src/worldManager/WorldManager.hpp"
#include <cmath>

Player::Player() {
    m_shape.setSize({WIDTH, HEIGHT});
    m_shape.setOrigin({WIDTH / 2.0f, HEIGHT / 2.0f});
    m_shape.setFillColor(sf::Color::Red);
}

void Player::placeTile(sf::RenderWindow& window, WorldManager& worldManager) noexcept {
    if (buildMode == BuildModeType::Block || buildMode == BuildModeType::Wall) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        int tileX = static_cast<int>(std::floor(worldPos.x / 32.0f));
        int tileY = static_cast<int>(std::floor(worldPos.y / 32.0f));

        if (buildMode == BuildModeType::Block) {
            worldManager.setGlobalBlock(tileX, tileY, BlockType::Limestone);
        }
        else {
            worldManager.setGlobalWall(tileX, tileY, WallType::Limestone);
        } 
    }
}

void Player::breakTile(sf::RenderWindow& window, WorldManager& worldManager) noexcept {
    if (buildMode == BuildModeType::Block || buildMode == BuildModeType::Wall) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        int tileX = static_cast<int>(std::floor(worldPos.x / 32.0f));
        int tileY = static_cast<int>(std::floor(worldPos.y / 32.0f));

        if (buildMode == BuildModeType::Block) {
            worldManager.setGlobalBlock(tileX, tileY, BlockType::Air);
        }
        else {
            worldManager.setGlobalWall(tileX, tileY, WallType::None);
        } 
    }
}

void Player::handleEvent(const sf::Event& event, sf::RenderWindow& window, WorldManager& worldManager) noexcept {
    if (event.is<sf::Event::KeyPressed>()) {
        auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        if (keyEvent->scancode == sf::Keyboard::Scancode::R) {
            if (buildMode == BuildModeType::Block) {
                buildMode = BuildModeType::Wall;
            } else if (buildMode == BuildModeType::Wall) {
                buildMode = BuildModeType::Block;
            }
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            placeTile(window, worldManager);
        }
        if (mouseEvent->button == sf::Mouse::Button::Right) {
            breakTile(window, worldManager);
        }
    }
}

void Player::update(float deltaTime) noexcept {
    m_previousPosition = m_position; 

    sf::Vector2f direction{0.0f, 0.0f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) direction.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) direction.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) direction.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) direction.x += 1.0f;

    float length = std::hypot(direction.x, direction.y);
    if (length > 0.0f) {
        direction /= length;
    }

    m_position += direction * m_speed * deltaTime;
}

void Player::interpolate(float interpolationFactor) noexcept {
    m_visualPosition = m_previousPosition + 
        (m_position - m_previousPosition) * interpolationFactor;

    m_shape.setPosition(m_visualPosition); 
}

void Player::draw(sf::RenderTarget& target) const {
    target.draw(m_shape);
}