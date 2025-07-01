#version 130
uniform mat4 matViewProjection;
uniform mat4 matWorld;

in vec3 in_Vertex;
in vec2 in_Normal;
in vec2 in_TexCoord;
in vec2 in_Color;

out vec3 ex_Normal;
out float ex_colorIndex;
out vec2 ex_texCoord;
out vec3 ex_viewDir;

void main(void) {
   vec4 worldPos = matWorld * vec4(in_Vertex,1.0);
   vec3 realNormal = vec3(in_Normal.xy, 1.0 - length(in_Normal));
   ex_Normal = gl_NormalMatrix * realNormal;
   ex_texCoord = in_TexCoord;
   ex_colorIndex = in_Color.x;
   gl_Position = worldPos * matViewProjection;
}
