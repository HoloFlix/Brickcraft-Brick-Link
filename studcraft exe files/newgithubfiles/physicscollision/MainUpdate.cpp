// Each frame:
physicsWorld.Step(deltaTime);

// To sync player position for rendering:
btTransform trans;
playerPhysics.GetBody()->getMotionState()->getWorldTransform(trans);
player.position.x = trans.getOrigin().getX();
player.position.y = trans.getOrigin().getY();
player.position.z = trans.getOrigin().getZ();