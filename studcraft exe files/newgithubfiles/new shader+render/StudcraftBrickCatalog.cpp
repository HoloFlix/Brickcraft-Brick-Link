#include "StudcraftBrickCatalog.h"

// Example registration function called at startup
void RegisterDefaultBricks() {
    StudcraftBrickCatalog& catalog = StudcraftBrickCatalog::Instance();

    // Example: Register a 2x4 brick
    BrickDefinition brick2x4;
    brick2x4.brick_id = 1;
    brick2x4.name = "2x4";
    brick2x4.subparts = {
        {"stud_left"}, {"stud_right"}, {"baseplate"}
        // Add geometry details as needed
    };
    catalog.RegisterBrick(brick2x4);

    // Add other brick types...
}