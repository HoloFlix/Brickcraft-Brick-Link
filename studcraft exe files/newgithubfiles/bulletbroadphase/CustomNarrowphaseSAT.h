#pragma once
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <glm/glm.hpp>

// Convert Bullet shape to your Polygon struct (for SAT)
struct Polygon {
    std::vector<glm::vec3> vertices;
};

bool SATCollision(const Polygon& polyA, const Polygon& polyB);

// Custom collision callback
class CustomNarrowphaseSAT : public btCollisionWorld::ContactResultCallback {
public:
    btRigidBody* bodyA;
    btRigidBody* bodyB;
    Polygon polyA, polyB;

    CustomNarrowphaseSAT(btRigidBody* a, btRigidBody* b, const Polygon& pa, const Polygon& pb)
        : bodyA(a), bodyB(b), polyA(pa), polyB(pb) {}

    btScalar addSingleResult(btManifoldPoint& cp,
                            const btCollisionObjectWrapper* colObj0Wrap,
                            int partId0, int index0,
                            const btCollisionObjectWrapper* colObj1Wrap,
                            int partId1, int index1) override {
        // After Bullet reports a contact, run SAT for confirmation
        if (SATCollision(polyA, polyB)) {
            // SAT says they really collide, allow contact
            return 1.0f;
        }
        // SAT says no collision, ignore contact
        return 0.0f;
    }
};