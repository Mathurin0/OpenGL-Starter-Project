#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 ourColor;
out vec3 ourPos;

uniform float offsetX;
uniform float offsetY;

void main()
{
    ourColor = color;
    ourPos = vec3(pos.x * 0.5f + offsetX, pos.y * 0.5f + offsetY, pos.z);
    gl_Position = vec4(ourPos, 1.0);
}
