if (forwardKey) {
    btVector3 v = btVector3(0,0,1); // forward in world
    playerPhysics.GetBody()->setLinearVelocity(v * 5.0f);
}