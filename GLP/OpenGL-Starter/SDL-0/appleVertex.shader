#version 330 core
layout (location = 0) in vec3 pos;

uniform float offsetX;
uniform float offsetY;

void main()
{
    gl_Position = vec4(pos.x + offsetX, pos.y + offsetY, pos.z, 1.0);
}
