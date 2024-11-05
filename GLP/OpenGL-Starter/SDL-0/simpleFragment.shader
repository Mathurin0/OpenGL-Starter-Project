#version 330 core
in vec3 ourColor;
in vec3 ourPos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
}
