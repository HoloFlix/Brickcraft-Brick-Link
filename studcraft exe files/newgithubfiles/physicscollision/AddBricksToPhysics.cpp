#include <btBulletDynamicsCommon.h>
#include "StudcraftBrickCatalog.h"

void AddBrickToPhysics(btDiscreteDynamicsWorld* world, const Brick& b) {
    // For a simple brick, use a box shape
    btCollisionShape* brickShape = new btBoxShape(btVector3(0.8f, 0.4f, 0.8f)); // Adjust to brick size
    btTransform brickTransform;
    brickTransform.setIdentity();
    brickTransform.setOrigin(btVector3(b.x, b.y, b.z));
    btScalar mass = 0.0f; // static
    btDefaultMotionState* motionState = new btDefaultMotionState(brickTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, brickShape, btVector3(0, 0, 0));
    btRigidBody* brickBody = new btRigidBody(rbInfo);
    world->addRigidBody(brickBody);
    // Store pointers for cleanup if needed!
}