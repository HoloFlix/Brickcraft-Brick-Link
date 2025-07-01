#version 130
uniform sampler2D diffuseTexture;
uniform vec3 lightDir;
uniform vec4 floorColor;
uniform vec3 viewPosition;

in vec3 ex_viewDir;
in vec2 ex_texCoord;
in vec3 ex_Normal;
in vec3 ex_worldPosition;
in float ex_fog;
out vec4 out_Color;

vec2 phongLightning(vec3 normal, vec3 light, vec3 viewDir){
	vec3 N = normalize(normal);
	vec3 L = normalize(-light);
	float diffuse = max(dot(N, L), 0.0);
	vec3 hV = normalize(L + normalize(viewDir));
	float specular = pow(clamp(dot(N, hV), 0.0, 1.0), 4.0);
	return vec2(diffuse, specular);
}

void main(void) {
	vec2 lightning = phongLightning(ex_Normal, lightDir, ex_viewDir);
	float light = lightning.x * 0.65 + 0.4 + lightning.y * 0.25;
	vec3 bitColor = texture2D(diffuseTexture, ex_texCoord).rgb * floorColor.rgb;
	float fog = clamp(pow(distance(viewPosition, ex_worldPosition) * 0.01, 2.0), 0.0, 1.0);
	vec3 color = mix(bitColor * light + vec3(lightning.y*0.15), vec3(0.75, 0.85, 0.95), fog);
	out_Color = vec4(color, 1.0);
}