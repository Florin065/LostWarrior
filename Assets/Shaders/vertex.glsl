# version 460

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

out vec3 objectColor;

void main()
{
    objectColor = aColor;
    gl_Position = vec4(aPosition, 1.0f);
}
