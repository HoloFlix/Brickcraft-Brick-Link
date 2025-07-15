#include "PlayerPhysics.h"

PlayerPhysics::PlayerPhysics(btDiscreteDynamicsWorld* world, float x, float y, float z)
    : world(world) {
    // Capsule is best for "character" movement
    shape = new btCapsuleShape(0.4f, 1.2f); // radius, height
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(x, y, z));
    motionState = new btDefaultMotionState(startTransform);

    btScalar mass = 80.0f;
    btVector3 inertia(0, 0, 0);
    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    body = new btRigidBody(rbInfo);
    body->setAngularFactor(0); // No tipping over!

    world->addRigidBody(body);
}

PlayerPhysics::~PlayerPhysics() {
    world->removeRigidBody(body);
    delete body;
    delete motionState;
    delete shape;
}