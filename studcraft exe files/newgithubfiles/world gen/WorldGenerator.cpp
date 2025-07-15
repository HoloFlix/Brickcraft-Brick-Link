#include "WorldGenerator.h"
#include <cmath>
#include <cstdlib>

WorldGenerator::WorldGenerator() {
    // Seed for reproducibility, or use std::random_device
    std::srand(12345);
}

// Simple deterministic biome assignment by position
BiomeType WorldGenerator::GetBiome(int cx, int cz) const {
    int v = (std::abs(cx) + std::abs(cz)) % 4;
    switch (v) {
        case 0: return BiomeType::Plains;
        case 1: return BiomeType::Hills;
        case 2: return BiomeType::Forest;
        case 3: return BiomeType::Desert;
        default: return BiomeType::Plains;
    }
}

// Example: Perlin/simplex noise recommended, but here is a basic "fake" noise
float WorldGenerator::GetHeight(int x, int z, BiomeType biome) const {
    float base = 10;
    float noise = (std::sin(x * 0.1f) + std::cos(z * 0.1f)) * 3;
    switch (biome) {
        case BiomeType::Plains: return base + noise * 0.5f;
        case BiomeType::Hills: return base + noise * 2.0f + 6;
        case BiomeType::Forest: return base + noise;
        case BiomeType::Desert: return base + noise * 0.2f - 2;
    }
    return base;
}

Chunk WorldGenerator::GenerateChunk(int cx, int cz) {
    Chunk chunk;
    chunk.chunk_x = cx;
    chunk.chunk_z = cz;
    chunk.biome = GetBiome(cx, cz);

    int chunkSize = 16;

    for (int dx = 0; dx < chunkSize; ++dx) {
        for (int dz = 0; dz < chunkSize; ++dz) {
            int world_x = cx * chunkSize + dx;
            int world_z = cz * chunkSize + dz;
            float height = GetHeight(world_x, world_z, chunk.biome);

            for (int y = 0; y < int(height); ++y) {
                Brick b;
                b.x = float(world_x);
                b.y = float(y);
                b.z = float(world_z);

                // Example: assign different brick_id/color_id per biome
                switch (chunk.biome) {
                    case BiomeType::Plains: b.brick_id = 1; b.color_id = 7; break; // grey
                    case BiomeType::Hills: b.brick_id = 1; b.color_id = 2; break; // green
                    case BiomeType::Forest: b.brick_id = 1; b.color_id = 3; break; // brown
                    case BiomeType::Desert: b.brick_id = 1; b.color_id = 14; break; // tan
                }
                chunk.bricks.push_back(b);
            }
        }
    }
    return chunk;
}