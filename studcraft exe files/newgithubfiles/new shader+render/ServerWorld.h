#pragma once
#include "StudcraftBrickCatalog.h"
#include <vector>

class ServerWorld {
public:
    // Store all bricks in world
    std::vector<Brick> bricks;

    // Validate & place a brick (server-authoritative)
    bool TryPlaceBrick(const Brick& candidate) {
        // Example: Check for overlap
        for (const auto& b : bricks) {
            if (b.x == candidate.x && b.y == candidate.y && b.z == candidate.z) {
                return false; // Already a brick here
            }
        }
        bricks.push_back(candidate);
        // TODO: Send update to clients via ReplicaManager3
        return true;
    }
};