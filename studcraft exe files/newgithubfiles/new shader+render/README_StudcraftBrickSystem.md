# Studcraft Brick System Quickstart

## 1. Adding Brick Types

- Edit `StudcraftBrickCatalog.cpp` and call `RegisterDefaultBricks()` at program startup.
- To add a new brick:
    1. Create a `BrickDefinition` with a unique `brick_id`.
    2. Add its subparts (define their names and geometry).
    3. Register with `StudcraftBrickCatalog::Instance().RegisterBrick()`.

## 2. Placing Bricks on Server

- Use the `ServerWorld` class.
- Call `TryPlaceBrick()` with a `Brick` struct. It will check for overlap and, if valid, add the brick to the world's brick vector.
- Broadcast changes to clients (implement via your network layer).

## 3. Rendering Bricks on Client

- Load GLSL shaders with `ShaderLoader::Load(vertexPath, fragmentPath)`.
- Use the palette texture (`LegoColors.bmp` loaded as 1D texture).
- For each brick, set the `color_id` uniform before drawing.
- Use the brick’s `brick_id` to select the correct mesh/model to render.

## 4. Adding New Bricks

- Extend the catalog in `RegisterDefaultBricks()`.
- Add new mesh/model data for rendering in the client.

## 5. Modularity

- All brick types, subparts, and color logic are centralized for easy modding and extension.

## 6. Shader Usage

- Place GLSL shaders in `assets/shaders/`.
- Vertex: `brick.vert`
- Fragment: `brick.frag`
- Compile and link at runtime using `ShaderLoader`.

## 7. Next Steps

- Implement advanced chunk/biome generation.
- Integrate LXFML importers for prefab structures.
- Expand the subpart system for more creative brick combinations.

---

For more advanced features or questions, see the official project documentation or open an issue!