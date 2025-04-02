#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 projectionMat;
uniform mat4 modelMat;

void main()
{
    gl_Position = projectionMat * modelMat * vec4(aPos, 1.0);
    ourColor = aColor;
}
