#include "Chunk.hpp"
#include "src/worldManager/TileData/TileData.hpp"

float Chunk::getGlobalX() const noexcept {
    float globalX{static_cast<float>(m_x * static_cast<int>(CHUNK_W)) * TILE_SIZE};
    return globalX;
}



BlockType Chunk::getLocalBlock(unsigned int x, unsigned int y) noexcept {
    if (x < CHUNK_W && y < CHUNK_H) {
        return blocks[x + (y * CHUNK_W)];
    }
    return BlockType::Air;
}

void Chunk::setLocalBlock(unsigned int x, unsigned int y, BlockType type) noexcept {
    if (x < CHUNK_W && y < CHUNK_H) {
        blocks[x + (y * CHUNK_W)] = type;
    }
}


WallType Chunk::getLocalWall(unsigned int x, unsigned int y) noexcept {
    if (x < CHUNK_W && y < CHUNK_H) {
        return walls[x + (y * CHUNK_W)];
    }
    return WallType::None;
}

void Chunk::setLocalWall(unsigned int x, unsigned int y, WallType type) noexcept {
    if (x < CHUNK_W && y < CHUNK_H) {
        walls[x + (y * CHUNK_W)] = type;
    }
}


void Chunk::updateGeometry() {
    m_mesh.setPrimitiveType(sf::PrimitiveType::Triangles);

    unsigned int solidTile = 0;

    for (unsigned int index = 0; index < CHUNK_SIZE; ++index) {
        if (blocks[index] != BlockType::Air) {
            solidTile += 1;
        } 
        else if (walls[index] != WallType::None) {
            solidTile += 1;
        }
    }

    m_mesh.resize(solidTile * 6);
    
    unsigned int vertexCount = 0;
    constexpr int TEX_SIZE = 16;

    auto addTileToMesh = [&](unsigned int x, unsigned int y, unsigned int texIndex, sf::Color color) {
        float posX0 = getGlobalX() + x * TILE_SIZE;
        float posX1 = posX0 + TILE_SIZE;
        float posY0 = static_cast<float>(y) * TILE_SIZE;
        float posY1 = posY0 + TILE_SIZE;

        float texX0 = static_cast<float>(texIndex * TEX_SIZE);
        float texX1 = texX0 + TEX_SIZE;
        float texY0 = 0.0f;
        float texY1 = static_cast<float>(TEX_SIZE);

        m_mesh[vertexCount + 0].position = sf::Vector2f(posX0, posY0);
        m_mesh[vertexCount + 0].texCoords = sf::Vector2f(texX0, texY0);
        m_mesh[vertexCount + 0].color = color;

        m_mesh[vertexCount + 1].position = sf::Vector2f(posX0, posY1);
        m_mesh[vertexCount + 1].texCoords = sf::Vector2f(texX0, texY1);
        m_mesh[vertexCount + 1].color = color;

        m_mesh[vertexCount + 2].position = sf::Vector2f(posX1, posY0);
        m_mesh[vertexCount + 2].texCoords = sf::Vector2f(texX1, texY0);
        m_mesh[vertexCount + 2].color = color;

        m_mesh[vertexCount + 3].position = sf::Vector2f(posX0, posY1);
        m_mesh[vertexCount + 3].texCoords = sf::Vector2f(texX0, texY1);
        m_mesh[vertexCount + 3].color = color;

        m_mesh[vertexCount + 4].position = sf::Vector2f(posX1, posY1);
        m_mesh[vertexCount + 4].texCoords = sf::Vector2f(texX1, texY1);
        m_mesh[vertexCount + 4].color = color;

        m_mesh[vertexCount + 5].position = sf::Vector2f(posX1, posY0);
        m_mesh[vertexCount + 5].texCoords = sf::Vector2f(texX1, texY0);
        m_mesh[vertexCount + 5].color = color;

        vertexCount += 6;
    };

    for (unsigned int y = 0; y < CHUNK_H; ++y) {
        for (unsigned int x = 0; x < CHUNK_W; ++x) {
            unsigned int index = x + (y * CHUNK_W);
            
            if (blocks[index] == BlockType::Air && walls[index] != WallType::None) {
                const auto& data = WallRegistry[static_cast<size_t>(walls[index])];
                addTileToMesh(x, y, data.m_texIndex, sf::Color(175, 175, 175));
            }
        }
    }

    for (unsigned int y = 0; y < CHUNK_H; ++y) {
        for (unsigned int x = 0; x < CHUNK_W; ++x) {
            unsigned int index = x + (y * CHUNK_W);
            
            if (blocks[index] != BlockType::Air) {
                const auto& data = BlockRegistry[static_cast<size_t>(blocks[index])];
                addTileToMesh(x, y, data.m_texIndex, sf::Color::White);
            }
        }
    }
}