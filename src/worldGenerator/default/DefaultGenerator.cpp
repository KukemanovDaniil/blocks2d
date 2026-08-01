#include "DefaultGenerator.hpp"
#include "src/worldManager/TileType/TileType.hpp" 
#include "src/worldGenerator/default/structure/tree/TreeGenerator.hpp"
#include <cmath>
#include <print>

DefaultGenerator::DefaultGenerator(int seed) {
    m_landscapeNoise.SetSeed(seed);
    m_landscapeNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_landscapeNoise.SetFrequency(0.005f);

    m_cavesNoise.SetSeed(seed + 1); 
    m_cavesNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_cavesNoise.SetFrequency(0.019f);

    std::srand(static_cast<unsigned int>(seed));
}

void DefaultGenerator::generate(Chunk& chunk, int chunkX) {
    generateLandscape(chunk, chunkX);
    generateVegetation(chunk, chunkX);
    generateCaves(chunk, chunkX);

    chunk.updateGeometry();
}

void DefaultGenerator::generateLandscape(Chunk& chunk, int chunkX) {
    for (unsigned int x = 0; x < CHUNK_W; ++x) {
        const int globalX = chunkX * static_cast<int>(CHUNK_W) + static_cast<int>(x);
        const unsigned int surfaceY = calculateSurfaceY(globalX);
        
        for (unsigned int y = surfaceY + 1; y < CHUNK_H; ++y) {
           chunk.setLocalBlock(x, y, BlockType::Basalt);
            if (y >= CHUNK_H - 3) {
                chunk.setLocalBlock(x, y, BlockType::Basalt);
                chunk.setLocalWall(x, y, WallType::Basalt);
            }
            else if (y >= CHUNK_H - 50) {
                chunk.setLocalBlock(x, y, BlockType::Andesite);
                chunk.setLocalWall(x, y, WallType::Andesite);
            }
            else if (y > surfaceY && y <= surfaceY + 7) {
                chunk.setLocalBlock(x, y, BlockType::Grus);
                chunk.setLocalWall(x, y, WallType::Grus);
            }
            else if (y > surfaceY && y <= surfaceY + 9) {
                chunk.setLocalBlock(x, y, BlockType::Gravel);
                chunk.setLocalWall(x, y, WallType::Gravel);
            }
            else {
                chunk.setLocalBlock(x, y, BlockType::Limestone);    
                chunk.setLocalWall(x, y, WallType::Limestone);          
            }  
        }
    }
}


void DefaultGenerator::generateVegetation(Chunk& chunk, int chunkX) {
    for (unsigned int x = 0; x < CHUNK_W; ++x) {
        int globalX = chunkX * CHUNK_W + x;
        unsigned int surfaceY = calculateSurfaceY(globalX);
        for (unsigned int y = 0; y < CHUNK_H; ++y) {
            if (y == surfaceY) {
                chunk.setLocalBlock(x, y, BlockType::Grass);
                chunk.setLocalWall(x, y, WallType::Grass);
            }
            else if (y > surfaceY && y < surfaceY + 5) {
                chunk.setLocalBlock(x, y, BlockType::Loam);
                chunk.setLocalWall(x, y, WallType::Loam);
            }
        }
    }
}

void DefaultGenerator::generateCaves(Chunk& chunk, int chunkX) {
    for (unsigned int x = 0; x < CHUNK_W; ++x) {
        const int globalX = chunkX * static_cast<int>(CHUNK_W) + static_cast<int>(x);
        unsigned int surfaceY = calculateSurfaceY(globalX);

        for (unsigned int y = surfaceY; y < CHUNK_H - 3; ++y) {
            const float caveNoiseValue = calculateCaveValue(globalX, static_cast<int>(y));

            if (std::abs(caveNoiseValue) < 0.06f) {
                chunk.setLocalBlock(x, y, BlockType::Air);
            }
        }
    }
}

void DefaultGenerator::generateTrees(WorldManager& world, int size) {
    for (int i = -size; i < size; ++i) {
        for (unsigned int x = 0; x < CHUNK_W; ++x) {
            int globalBlockX = (i * static_cast<int>(CHUNK_W)) + static_cast<int>(x);
            unsigned int surfaceY = calculateSurfaceY(globalBlockX);

            if (std::rand() % 100 > 15) continue; 
            if (world.getGlobalBlock(globalBlockX, static_cast<int>(surfaceY)) != BlockType::Grass) continue;

            unsigned int treeHeight = 4 + (std::rand() % 10);
            for (unsigned int h = 1; h <= treeHeight; ++h) {
                int wallY = static_cast<int>(surfaceY) - h;
                if (wallY >= 0) world.setGlobalWall(globalBlockX, wallY, WallType::Oak);
            }

            int topY = static_cast<int>(surfaceY) - treeHeight;
            for (int leafY = topY - 4; leafY <= topY; ++leafY) {
                for (int leafX = globalBlockX - 2; leafX <= globalBlockX + 2; ++leafX) {
                    if (leafY >= 0) {
                        if (leafX == globalBlockX && leafY > topY - 1) continue;
                        world.setGlobalWall(leafX, leafY, WallType::Leaves);
                    }
                }
            }
        }
    }
}