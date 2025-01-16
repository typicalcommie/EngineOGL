//p
#version 460 core

out vec4 FragColor;
in vec2 textureCoordinates;

uniform sampler2D textureData;

void main()
{
	FragColor = texture(textureData, textureCoordinates);
//	FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}