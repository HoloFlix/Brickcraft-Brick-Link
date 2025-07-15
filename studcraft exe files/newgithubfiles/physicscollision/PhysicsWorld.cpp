#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() {
    collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}

PhysicsWorld::~PhysicsWorld() {
    // Bullet cleans up pointers internally, no manual delete needed for unique_ptrs
}

void PhysicsWorld::Step(float dt) {
    dynamicsWorld->stepSimulation(dt, 10); // up to 10 substeps per frame
}