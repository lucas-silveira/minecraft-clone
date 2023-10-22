#version 330 core

in vec2 tex_coord;
in vec3 normal;

out vec4 FragColor;

uniform sampler2D a_tex;

void main()
{
	vec3 light_color = vec3(1.0, 1.0, 1.0);
	vec3 obj_color = vec3(1.0, 1.0, 1.0);
	vec3 light_dir1 = vec3(0.0, 1.0, 0.0);
	vec3 light_dir2 = vec3(1.0, 1.0, 1.0);

	float ambient_strength = 0.4;
	vec3 ambient_light = ambient_strength * light_color;

	vec3 norm = normalize(normal);

	float ds1 = 0.7;
	float diffuse_dot1 = max(dot(norm, light_dir1), 0.0);
	vec3 diffuse_light1 = ds1 * diffuse_dot1 * light_color;

	float ds2 = 0.2;
	float diffuse_dot2 = max(dot(norm, light_dir2), 0.0 );
	vec3 diffuse_light2 = ds2 * diffuse_dot2 * light_color;

	vec3 light = (ambient_light + diffuse_light1 + diffuse_light2) * obj_color;
	FragColor = texture(a_tex, tex_coord) * vec4(light, 1.0);;
}