#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexPoints;

out vec2 TexCoord;
out vec4 myColor;

uniform mat4 p;

void main()
{
   gl_Position = p*vec4(aPos,1.0f);
   TexCoord = aTexPoints;
   myColor = aColor;
}