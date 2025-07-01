#version 130
uniform mat4 matViewProjection;
uniform mat4 matWorld;
uniform vec3 viewPosition;

in vec3 in_Vertex;
in vec2 in_Normal;
in vec2 in_TexCoord;
in float in_Color;

out vec3 ex_Normal;
out vec3 ex_viewDir;
out vec2 ex_texCoord;
out vec3 ex_worldPosition;
void main(void) {
   vec4 worldPos = matWorld * vec4(in_Vertex,1.0);
   ex_worldPosition = worldPos.xyz;
   ex_viewDir = viewPosition -  worldPos.xyz;
   vec3 realNormal = vec3(in_Normal.xy, 1.0 - length(in_Normal));
   ex_Normal = gl_NormalMatrix * realNormal;
   ex_texCoord = in_TexCoord;
   vec4 outPosition = worldPos * matViewProjection;
   gl_Position = outPosition;
}