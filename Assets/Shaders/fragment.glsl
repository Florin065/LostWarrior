#version 460

out vec4 FragColor;

in vec3 objectColor;

void main()
{
    FragColor = vec4(objectColor, 1.0f);
}

