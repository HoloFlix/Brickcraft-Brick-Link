#version 130
uniform sampler1D lookUpTexture;
uniform vec4 colorOffset;
uniform vec3 lightDir;

in float ex_colorIndex;
in vec3 ex_viewDir;
in vec2 ex_texCoord;
in vec3 ex_Normal;

out vec4 out_Color;

float getEdge(vec2 texCoord){
   float edgeColorX = abs(texCoord.x - 0.5);
   edgeColorX = clamp(edgeColorX - 0.47, 0.0, 1.0);
   float edgeColorY = abs(texCoord.y - 0.5);
   edgeColorY = clamp(edgeColorY - 0.47, 0.0, 1.0);
   return max(edgeColorY, edgeColorX) * 10.0;
}
vec2 phongLightning(vec3 normal, vec3 light, vec3 viewDir){
	vec3 N = normalize(normal);
	vec3 L = normalize(-light);
	float diffuse = max(dot(N, L), 0.0);
	vec3 hV = normalize(L + normalize(viewDir));
	float specular = pow(clamp(dot(N, hV), 0.0, 1.0), 4.0);
	return vec2(diffuse, specular);
}
void main(void) {
	float maxEdge = getEdge(ex_texCoord);
	vec2 lightning = phongLightning(ex_Normal, lightDir, ex_viewDir);
	float light =  lightning.x * 0.65 + 0.4 - maxEdge;
	vec3 color = texture1D(lookUpTexture, ex_colorIndex).rgb;
	out_Color = vec4(color * light, 1.0) + colorOffset;
}