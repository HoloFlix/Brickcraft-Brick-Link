// We now need matrices from the application
uniform mat4 u_mvp_matrix;    // Model-View-Projection Matrix
uniform mat4 u_model_matrix;  // Model Matrix (transforms vertex to world space)
uniform mat3 u_normal_matrix; // Normal Matrix

varying vec3 v_world_normal;
varying vec3 v_world_pos;

void main()
{
    // Final position of the vertex
    gl_Position = u_mvp_matrix * gl_Vertex;
    
    // Pass the world-space position and normal to the fragment shader
    v_world_pos = (u_model_matrix * gl_Vertex).xyz;
    v_world_normal = normalize(u_normal_matrix * gl_Normal);
}