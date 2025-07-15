#include "CustomNarrowphaseSAT.h"

void ProjectOntoAxis(const Polygon& poly, const glm::vec3& axis, float& min, float& max) {
    min = max = glm::dot(poly.vertices[0], axis);
    for (const auto& v : poly.vertices) {
        float proj = glm::dot(v, axis);
        if (proj < min) min = proj;
        if (proj > max) max = proj;
    }
}

bool SATCollision(const Polygon& polyA, const Polygon& polyB) {
    std::vector<glm::vec3> axes;
    // For each edge in both polygons, get perpendicular axis (for 2D SAT, or face normals for 3D)
    // For 3D polygons, use face normals and cross products of edges
    // (Example assumes 2D polygons for simplicity)
    for (size_t i = 0; i < polyA.vertices.size(); ++i) {
        glm::vec3 edge = polyA.vertices[(i + 1) % polyA.vertices.size()] - polyA.vertices[i];
        glm::vec3 axis = glm::normalize(glm::vec3(-edge.y, edge.x, 0));
        axes.push_back(axis);
    }
    for (size_t i = 0; i < polyB.vertices.size(); ++i) {
        glm::vec3 edge = polyB.vertices[(i + 1) % polyB.vertices.size()] - polyB.vertices[i];
        glm::vec3 axis = glm::normalize(glm::vec3(-edge.y, edge.x, 0));
        axes.push_back(axis);
    }
    // Test all axes
    for (const glm::vec3& axis : axes) {
        float minA, maxA, minB, maxB;
        ProjectOntoAxis(polyA, axis, minA, maxA);
        ProjectOntoAxis(polyB, axis, minB, maxB);
        if (maxA < minB || maxB < minA)
            return false; // Separating axis found
    }
    return true; // No separating axis
}