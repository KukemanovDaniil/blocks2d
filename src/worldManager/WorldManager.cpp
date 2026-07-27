#include "src/worldManager/WorldManager.hpp"
#include "src/worldGenerator/default/DefaultGenerator.hpp"
#include "src/math/random/RandomInRange.hpp"
#include "src/player/Player.hpp"
#include <print>

void WorldManager::createWorld(int size) {
    DefaultGenerator generator(randomInRange(-999, 999));
    for (int i = -size; i < size; ++i) {
        Chunk chunk;
        chunk.setLocalX(i);
        generator.generate(chunk, chunk.getLocalX()); 
        
        m_activeChunks[i] = std::move(chunk);
    }

    generator.generateTrees(*this, size);

    for (auto& [id, chunk] : m_activeChunks) {
        chunk.updateGeometry();
    }
}

void WorldManager::loadAtlasTexture(const char* path) {
    if (!m_blockAtlas.loadFromFile(path)) {
        std::println("Failed load tex: blockAtlas");
        return;
    }
    m_blockAtlas.setSmooth(false);
    (void)m_blockAtlas.generateMipmap();
    
    sf::Texture::bind(&m_blockAtlas);
    
    GLint maxLevel = 4; 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
    
    sf::Texture::bind(nullptr);
}

BlockType WorldManager::getGlobalBlockInPixels(float globalX, float globalY) {
    if (globalY < 0.0f || globalY >= CHUNK_H_PIXELS) return BlockType::Air;
    
    int chunkX = static_cast<int>(std::floor(globalX / CHUNK_W_PIXELS));
        
    auto it = m_activeChunks.find(chunkX);
    if (it == m_activeChunks.end()) {
        return BlockType::Air;
    }

    Chunk& chunk = it->second;
        
    float localPixelX = globalX - chunk.getGlobalX();
    unsigned int tileX = static_cast<unsigned int>(localPixelX / TILE_SIZE);
    unsigned int tileY = static_cast<unsigned int>(globalY / TILE_SIZE);

    return chunk.getLocalBlock(tileX, tileY);
} 



BlockType WorldManager::getGlobalBlock(int blockX, int blockY) {
    if (blockY < 0 || blockY >= static_cast<int>(CHUNK_H)) return BlockType::Air;
    int chunkX = static_cast<int>(std::floor(static_cast<float>(blockX) / CHUNK_W));
    auto it = m_activeChunks.find(chunkX);
    if (it == m_activeChunks.end()) return BlockType::Air;

    int localX = blockX % static_cast<int>(CHUNK_W);
    if (localX < 0) localX += CHUNK_W;
    return it->second.getLocalBlock(static_cast<unsigned int>(localX), static_cast<unsigned int>(blockY));
}

bool WorldManager::setGlobalBlock(int blockX, int blockY, BlockType type) {
    if (blockY < 0 || blockY >= static_cast<int>(CHUNK_H)) return false;
    int chunkX = static_cast<int>(std::floor(static_cast<float>(blockX) / CHUNK_W));
    auto it = m_activeChunks.find(chunkX);
    if (it == m_activeChunks.end()) return false;

    int localX = blockX % static_cast<int>(CHUNK_W);
    if (localX < 0) localX += CHUNK_W;
    it->second.setLocalBlock(static_cast<unsigned int>(localX), static_cast<unsigned int>(blockY), type);
    it->second.updateGeometry();
    return true;
}



WallType WorldManager::getGlobalWall(int blockX, int blockY) {
    if (blockY < 0 || blockY >= static_cast<int>(CHUNK_H)) return WallType::None;
    int chunkX = static_cast<int>(std::floor(static_cast<float>(blockX) / CHUNK_W));
    auto it = m_activeChunks.find(chunkX);
    if (it == m_activeChunks.end()) return WallType::None;

    int localX = blockX % static_cast<int>(CHUNK_W);
    if (localX < 0) localX += CHUNK_W;
    return it->second.getLocalWall(static_cast<unsigned int>(localX), static_cast<unsigned int>(blockY));
}

bool WorldManager::setGlobalWall(int blockX, int blockY, WallType type) {
    if (blockY < 0 || blockY >= static_cast<int>(CHUNK_H)) return false;
    int chunkX = static_cast<int>(std::floor(static_cast<float>(blockX) / CHUNK_W));
    auto it = m_activeChunks.find(chunkX);
    if (it == m_activeChunks.end()) return false;

    int localX = blockX % static_cast<int>(CHUNK_W);
    if (localX < 0) localX += CHUNK_W;
    it->second.setLocalWall(static_cast<unsigned int>(localX), static_cast<unsigned int>(blockY), type);
    it->second.updateGeometry();
    return true;
}



void WorldManager::draw(sf::RenderTarget& target, const Player& player, const Camera& camera) const {
    float halfWidth = (camera.getBaseSize().x * camera.getCurrentZoom()) / 2.0f;
    float cameraLeft = player.getPosition().x - halfWidth;
    float cameraRight = player.getPosition().x + halfWidth;

    int startChunkX = static_cast<int>(std::floor(cameraLeft / CHUNK_W_PIXELS));
    int endChunkX = static_cast<int>(std::floor(cameraRight / CHUNK_W_PIXELS));

    startChunkX -= 1; 
    endChunkX += 1;

    for (int x = startChunkX; x <= endChunkX; ++x) {
        auto it = m_activeChunks.find(x);
        
        if (it != m_activeChunks.end()) {
            it->second.draw(target, m_blockAtlas);
        }
    }
}