// Add a custom mesh shape for the sloped brick
btConvexHullShape* slopeShape = new btConvexHullShape();
slopeShape->addPoint(btVector3(-1,0,-1));
slopeShape->addPoint(btVector3(1,0,-1));
slopeShape->addPoint(btVector3(1,0,1));
slopeShape->addPoint(btVector3(-1,0,1));
slopeShape->addPoint(btVector3(-1,1,-1));
slopeShape->addPoint(btVector3(1,1,-1));
// The "slope" is created by omitting the top verts at z=1

// Add rigid body as usual
// ... (similar to previous brick example)