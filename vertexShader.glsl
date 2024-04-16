#version 330 core
layout (location = 0) in vec4 position;
layout(location=1) in vec4 color;
smooth out vec4 outputColor;
void main()
{
   gl_Position = position;
   outputColor=color;
}