#version 330 core

in vec4 myColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D currentTexture;

void main()
{
   vec4 color = texture(currentTexture,TexCoord);
   FragColor = myColor * color;
}