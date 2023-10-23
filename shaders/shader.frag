#version 330 core

in vec2 tex_coord;
in vec3 normal;
in float tex_type;

out vec4 FragColor;

uniform sampler2D dirt;
uniform sampler2D grass;
uniform sampler2D water;
uniform sampler2D stone;
uniform sampler2D wood;
uniform sampler2D sand;
uniform sampler2D leaf;

vec4 GetTexture(float type)
{
	if (type < .5) return texture(dirt, tex_coord);
	if (type < 1.5) return texture(grass, tex_coord);
	if (type < 2.5) return texture(water, tex_coord);
	if (type < 3.5) return texture(stone, tex_coord);
	if (type < 4.5) return texture(wood, tex_coord);
	if (type < 5.5) return texture(sand, tex_coord);
	if (type < 6.5) return texture(leaf, tex_coord);
	return texture(dirt, tex_coord);
}

void main()
{
	vec3 light_color = vec3(1.0, 1.0, 1.0);
	vec3 obj_color = vec3(1.0, 1.0, 1.0);
	vec3 light_dir1 = vec3(0.0, 1.0, 0.0);
	vec3 light_dir2 = vec3(1.0, 1.0, 1.0);

	float ambient_strength = 0.5;
	vec3 ambient_light = ambient_strength * light_color;

	vec3 norm = normalize(normal);

	float ds1 = 0.4;
	float diffuse_dot1 = max(dot(norm, light_dir1), 0.0);
	vec3 diffuse_light1 = ds1 * diffuse_dot1 * light_color;

	float ds2 = 0.1;
	float diffuse_dot2 = max(dot(norm, light_dir2), 0.0 );
	vec3 diffuse_light2 = ds2 * diffuse_dot2 * light_color;

	vec3 light = (ambient_light + diffuse_light1 + diffuse_light2) * obj_color;
	FragColor = GetTexture(tex_type) * vec4(light, 1.0);
}