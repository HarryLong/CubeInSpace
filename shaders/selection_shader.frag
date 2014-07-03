#version 330

uniform int colorCode;

out vec4 outputCode;

void main()
{
    outputCode = vec4(colorCode/255.0);
}
