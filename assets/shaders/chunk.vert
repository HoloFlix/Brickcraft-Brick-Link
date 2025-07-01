#version 130
uniform mat4 matViewProjection;
uniform mat4 matWorld;
uniform vec3 viewPosition;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec2 in_Color;

out vec3 ex_Normal;
out float ex_colorIndex;
out vec3 ex_viewDir;
out vec2 ex_texCoord;
out float ex_depth;
out float ex_isPeg;
out float ex_fog;

void main(void) {
	ex_Normal = gl_NormalMatrix * in_Normal;
	ex_texCoord = in_TexCoord;
	ex_colorIndex = in_Color.x;
	ex_isPeg = in_Color.y;
	vec4 worldPos = matWorld * vec4(in_Vertex,1.0);
	float depth = distance(worldPos.xyz, viewPosition);
	// (PegHeight + 0.5) * 1 / distance
	// (0.17f + 0.5) * (1/2 = 0.335
	worldPos.y -= (clamp(depth, 22.0, 24.0) - 22.0) * 0.111666667 * ex_isPeg;
	ex_viewDir = viewPosition -  worldPos.xyz;
	vec4 screenPostion = worldPos * matViewProjection;
	ex_depth = screenPostion.z;
	// 1 / 150 = 0.005
	ex_fog = clamp(pow(ex_depth * 0.01, 2.0), 0.0, 1.0);
	gl_Position = screenPostion;
}