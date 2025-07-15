# Bullet Physics: Optimized Collision (AABB + SAT)

## Workflow

1. Use Bullet’s collision world as usual. Add standard bricks as box shapes.
2. For special bricks (slopes, custom polygons), add as convex hull or mesh shape.
3. To further optimize, use a custom ContactResultCallback for those special bricks:
    - When Bullet reports a contact, run a SAT test on the polygons.
    - Only treat as a collision if SAT returns true.

4. For bricks with simple shapes, rely on Bullet’s fast box checks.

## Benefits

- Very fast broad phase (AABB) for 99% of objects.
- Accurate narrow phase (SAT) only for special shapes.
- Easy to extend for more complex collisions in the future.
