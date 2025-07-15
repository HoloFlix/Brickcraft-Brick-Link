#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void Step(float dt);

    btDiscreteDynamicsWorld* GetWorld() { return dynamicsWorld.get(); }

private:
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};