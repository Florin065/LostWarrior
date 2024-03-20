# version 460

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 Model;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = Model * vec4(aPosition, 1.0f);
}
