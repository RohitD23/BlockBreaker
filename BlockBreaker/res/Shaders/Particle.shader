#shader vertex

#version 330 core
layout(location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 u_projection;
uniform vec2 u_offset;
uniform vec4 u_color;

void main() {
	float scale = 10.0f;
	TexCoords = vertex.zw;
	ParticleColor = u_color;
	gl_Position = u_projection * vec4((vertex.xy * scale) + u_offset, 0.0, 1.0);
}

#shader fragment

#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 color;

uniform sampler2D u_sprite;

void main()
{
	color = (texture(u_sprite, TexCoords) * ParticleColor);
}