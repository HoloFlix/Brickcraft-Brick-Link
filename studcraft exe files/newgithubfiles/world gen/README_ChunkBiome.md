# Advanced Chunk & Biome World Generation

## How It Works

- The world is split into 16x16 brick "chunks."
- Each chunk is assigned a biome (Plains, Hills, Forest, Desert, ...).
- Terrain height for each (x,z) is generated using noise functions.
- Each chunk contains a list of bricks, with type and color determined by the biome.
- Player spawn uses the terrain height to avoid falling/floating.

## Implementing

1. Add `Chunk.h`, `WorldGenerator.h/cpp`.
2. In your world class, store a map of loaded chunks.
3. On server startup, generate chunks around spawn using `WorldGenerator::GenerateChunk`.
4. Spawn the player at the correct y-position using `GetHeight`.
5. As player moves, dynamically load/generate more chunks.

## Improving

- Swap the fake noise for a real Perlin/Simplex noise library.
- Add more biomes and biome transitions.
- Place structures (trees, rocks) based on biome rules.

## Fixing Player Falling/Floating

- Always use the world generator to set the player’s y-position above ground.
- Ensure your physics/collision system is active and bricks are collidable.
