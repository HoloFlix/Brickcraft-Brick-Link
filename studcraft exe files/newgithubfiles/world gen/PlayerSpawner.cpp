#include "WorldGenerator.h"

void SpawnPlayer(Player& player, int spawn_x, int spawn_z, WorldGenerator& worldGen) {
    BiomeType biome = worldGen.GetBiome(spawn_x / 16, spawn_z / 16);
    float groundY = worldGen.GetHeight(spawn_x, spawn_z, biome);
    player.position.x = float(spawn_x);
    player.position.y = groundY + 1.0f; // stand on top
    player.position.z = float(spawn_z);

    player.velocity = {0, 0, 0}; // reset velocity
}