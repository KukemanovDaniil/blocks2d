#pragma once 
#include "src/worldManager/chunk/Chunk.hpp" 
#include "src/worldManager/WorldManager.hpp"
#include "libs/noise/FastNoiseLite.h"

class DefaultGenerator {
public:
    DefaultGenerator(int seed = 0);

    void generate(Chunk& chunk, int chunkX);
    
    void generateTrees(WorldManager& world, int size);

private:
    FastNoiseLite m_landscapeNoise;
    FastNoiseLite m_cavesNoise;

    unsigned int calculateSurfaceY(int globalX) {
        float noiseVal = m_landscapeNoise.GetNoise(static_cast<float>(globalX), 0.0f);
        return BASE_LEVEL - static_cast<int>(noiseVal * 35.0f);
    }

    float calculateCaveValue(int globalX, int globalY) { return m_cavesNoise.GetNoise(static_cast<float>(globalX), static_cast<float>(globalY)); }

    void generateLandscape(Chunk& chunk, int chunkX);
    void generateVegetation(Chunk& chunk, int chunkX);
    void generateCaves(Chunk& chunk, int chunkX);

    static constexpr int BASE_LEVEL = 186;
    static constexpr int SEA_LEVEL = 256;
};
