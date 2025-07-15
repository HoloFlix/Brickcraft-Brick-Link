#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

// Basic Brick struct
struct Brick {
    float x, y, z;
    int brick_id;
    int color_id;
};

// Basic SubPart definition
struct LegoSubPart {
    std::string name;
    // Add geometry data as needed (vertices, shape parameters, etc.)
    // For demo, just placeholder
};

struct BrickDefinition {
    int brick_id;
    std::string name;
    std::vector<LegoSubPart> subparts;
};

class StudcraftBrickCatalog {
public:
    static StudcraftBrickCatalog& Instance() {
        static StudcraftBrickCatalog instance;
        return instance;
    }

    // Register a new brick type
    void RegisterBrick(const BrickDefinition& def) {
        catalog_[def.brick_id] = def;
        name_lookup_[def.name] = def.brick_id;
    }

    // Lookup by ID
    const BrickDefinition* GetBrickDef(int id) const {
        auto it = catalog_.find(id);
        if (it != catalog_.end()) return &it->second;
        return nullptr;
    }

    // Lookup by name
    const BrickDefinition* GetBrickDef(const std::string& name) const {
        auto it = name_lookup_.find(name);
        if (it != name_lookup_.end()) return GetBrickDef(it->second);
        return nullptr;
    }

private:
    StudcraftBrickCatalog() = default;
    std::unordered_map<int, BrickDefinition> catalog_;
    std::unordered_map<std::string, int> name_lookup_;
};