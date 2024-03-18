#version 330

in vec2 pass_textureCoords;

out vec4 out_color;

uniform vec3 color;
uniform sampler2D textureSampler;

void main(void)
{
	out_color = vec4(1.0, 1.0, 1.0, texture(textureSampler, pass_textureCoords).a);
}