#version 460 core

in vec4 vs_Color;

out vec4 fs_Color;

void main(void)
{
    fs_Color = vs_Color;
}