#version 130
uniform sampler1D lookUpTexture;
uniform vec3 lightDir;

in float ex_colorIndex;
in vec3 ex_viewDir;
in vec2 ex_texCoord;
in vec3 ex_Normal;
in float ex_depth;
in float ex_isPeg;
in float ex_fog;
out vec4 out_Color;

float getEdge(vec2 texCoord){
   float edgeColorX = abs(texCoord.x - 0.5);
   edgeColorX = clamp(edgeColorX - 0.48, 0.0, 1.0);
   float edgeColorY = abs(texCoord.y - 0.5);
   edgeColorY = clamp(edgeColorY - 0.48, 0.0, 1.0);
   return max(edgeColorY, edgeColorX) * 10.0;
}
vec2 phongLightning(vec3 normal, vec3 light, vec3 viewDir){
	vec3 N = normalize(normal);
	vec3 L = normalize(-light);
	float diffuse = max(dot(N, L), 0.0);
	vec3 hV = normalize(L + viewDir);
	float specular = pow(clamp(dot(N, hV), 0.0, 1.0), 4.0);
	return vec2(diffuse, specular);
}
void main(void) {
	float maxEdge = getEdge(ex_texCoord);
	float fadePeg = clamp(ex_depth, 12.0, 24.0);
	vec3 viewDir = normalize(ex_viewDir);
	// (500 / (24 - 12)) / 500 = 0.0833333333
	fadePeg = (fadePeg - 12.0)*0.0833333333 * ex_isPeg;// * max(dot(viewDir, vec3(0.0,1.0,0.0)), 0.0);
	vec2 lightning = phongLightning(ex_Normal, lightDir, viewDir);
	vec2 lightningUp = phongLightning(vec3(0.0,1.0,0.0), lightDir, viewDir);
	vec2 mixedLightning =  mix(lightning, lightningUp, fadePeg);
	float light = mixedLightning.x * 0.65 + 0.4 + mixedLightning.y * 0.25 - maxEdge;
	vec3 color = texture1D(lookUpTexture, ex_colorIndex).rgb;
	out_Color = vec4(mix(color * light + vec3(mixedLightning.y*0.15), vec3(0.75, 0.85, 0.95), ex_fog), 1.0);
}