//v
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinatesInput;

uniform mat4 projection;
uniform mat4 world;
uniform mat4 view;

uniform mat4 rota;

out vec2 textureCoordinates;	//Out value should have same name as in fragment shader.

void main()
{
	textureCoordinates = textureCoordinatesInput;

	vec4 final = projection * view * world * rota * vec4(position, 1);
	gl_Position = final;
}