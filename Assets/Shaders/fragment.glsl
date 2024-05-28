#version 460

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform int completion = 0;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
    int level = completion % 11;
    if (level == 0)
    {

    }
    else if (level == 1)
    {
        FragColor.r *= 1.1f;
        FragColor.g *= 0.9f;
        FragColor.b *= 0.9f;
    }
    else if (level == 2)
    {
        FragColor.r *= 1.2f;
        FragColor.g *= 0.8f;
        FragColor.b *= 0.8f;
    }
    else if (level <= 5)
    {
        FragColor.r *= 1.25f;
        FragColor.g *= 0.75f;
        FragColor.b *= 0.75f;
    }
    else if (level <= 7)
    {
        FragColor.g *= 1.25f;
        FragColor.r *= 0.75f;
        FragColor.b *= 0.75f;
    }
    else if (level <= 10)
    {
        FragColor.r *= 1.2f;
        FragColor.g *= 0.8f;
        FragColor.b *= 0.8f;
    }
}

