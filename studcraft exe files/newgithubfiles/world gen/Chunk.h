#pragma once
#include <vector>
#include "StudcraftBrickCatalog.h"

enum class BiomeType { Plains, Hills, Forest, Desert };

struct Chunk {
    int chunk_x, chunk_z; // coordinates in chunk grid
    BiomeType biome;
    std::vector<Brick> bricks; // All bricks in this chunk
};