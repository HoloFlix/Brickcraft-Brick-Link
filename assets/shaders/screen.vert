#version 130
in vec3 in_Vertex;
in vec2 in_TexCoord;

out vec2 ex_texCoord;

void main(void) {
   ex_texCoord = in_TexCoord;
   vec4 outPosition = vec4(in_Vertex, 1.0);
   gl_Position = outPosition;
}