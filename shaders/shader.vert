#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in float a_tex_type;

out vec2 tex_coord;
out vec3 normal;
out float tex_type;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(a_pos, 1.0);
	tex_coord = a_tex_coord;
	normal = a_normal;
	tex_type = a_tex_type;
}