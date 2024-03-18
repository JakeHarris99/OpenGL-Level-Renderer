#version 140

in vec2 out_uv;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

void main() {
	//fragmentColor = vec4( out_uv.x, out_uv.y, 1.0, 1.0 );
	fragmentColor = texture(textureSampler, out_uv).rgba;
}