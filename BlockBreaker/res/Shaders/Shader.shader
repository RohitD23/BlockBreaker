#shader vertex

#version 330 core
layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
	TexCoords = vertex.zw;
	gl_Position = (u_projection * u_model * vec4(vertex.xy, 0.0, 1.0));
}

#shader fragment

#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D u_image;
uniform vec3 u_spriteColor;

void main()
{
	color = vec4(u_spriteColor, 1.0) * texture(u_image, TexCoords);
}
