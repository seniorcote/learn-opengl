#version 460 core

uniform mat4 mvp;

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec4 in_Color;

out vec4 vs_Color;

void main()
{
    gl_Position = mvp * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0f);
    vs_Color = in_Color;
}