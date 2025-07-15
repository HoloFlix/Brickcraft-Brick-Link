# Physics & Collision Integration (Bullet Physics)

## Steps

1. Install Bullet Physics (`libbullet-dev` or vcpkg).
2. Add `PhysicsWorld.h/cpp`, `PlayerPhysics.h/cpp` to your project.
3. At startup, create a `PhysicsWorld` instance.
4. For each brick, add a static box collider to the physics world.
5. When spawning a player, create a `PlayerPhysics` instance at the correct position.
6. Each frame, call `physicsWorld.Step(deltaTime)`.
7. Sync player’s rendering position from Bullet’s motion state.
8. To move the player, set velocity or use impulses. For best results, use Bullet’s character controller.

## Tips

- For large maps, batch static brick colliders into one mesh.
- Always spawn the player above the terrain using your worldgen height.
- Update brick colliders when bricks are destroyed/placed.

## Resources

- Bullet Physics: https://github.com/bulletphysics/bullet3
- Character Controller example: https://github.com/bulletphysics/bullet3/blob/master/examples/CharacterDemo/CharacterDemo.cpp
