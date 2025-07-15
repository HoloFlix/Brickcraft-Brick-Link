varying vec3 v_world_normal;
varying vec3 v_world_pos;

// Camera position passed in from the application
uniform vec3 u_camera_pos;

void main()
{
    // Fixed world-space light
    vec3 light_pos = vec3(50.0, 100.0, 50.0);
    vec3 light_dir = normalize(light_pos - v_world_pos);

    // Ambient light
    float ambient = 0.3;
    
    // Diffuse light
    float diffuse = max(dot(v_world_normal, light_dir), 0.0);

    // The final color
    vec3 brick_color = vec3(0.8, 0.8, 0.8);
    vec3 final_color = brick_color * (ambient + diffuse);
    
    gl_FragColor = vec4(final_color, 1.0);
}