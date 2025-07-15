#pragma once
#include "Chunk.h"
#include <map>

class WorldGenerator {
public:
    WorldGenerator();

    // Generate a chunk at (cx, cz)
    Chunk GenerateChunk(int cx, int cz);

private:
    BiomeType GetBiome(int cx, int cz) const;
    float GetHeight(int x, int z, BiomeType biome) const;
};