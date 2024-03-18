#version 330

in vec3 position;
in vec2 textureCoords;

out vec2 pass_textureCoords;

uniform vec3 translation;

void main(void)
{
	gl_Position = vec4(position.xy + translation.xy + vec2(-1.0, 1.0), 0.0, 1.0);
	pass_textureCoords = textureCoords;
}