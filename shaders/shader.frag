#version 330 core

in vec2 tex_coord;
in vec3 normal;

out vec4 FragColor;

uniform sampler2D a_tex;
uniform vec3 light_color;
uniform vec3 light_dir;

void main()
{
	vec3 obj_color = vec3(1.0, 1.0, 1.0);

	float ambient_strength = 0.3;
	vec3 ambient_light = ambient_strength * light_color;

	vec3 norm = normalize(normal);

	float diffuse_strength = max(dot(norm, light_dir), 0.0);
	vec3 diffuse_light = diffuse_strength * light_color;

	vec3 light = (ambient_light + diffuse_light) * obj_color;
	FragColor = texture(a_tex, tex_coord) * vec4(light, 1.0);;
}