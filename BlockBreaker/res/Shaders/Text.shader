#shader vertex

#version 330 core
layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}

#shader fragment

#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_text;
uniform vec3 u_textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, TexCoords).r);
    color = vec4(u_textColor, 1.0) * sampled;
}
