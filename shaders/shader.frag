#version 330

out vec4 outputColor;

in vec4 theColor;

uniform float brightnessIncrement;

void main()
{
    outputColor = vec4(theColor.x+brightnessIncrement, theColor.y+brightnessIncrement,theColor.z+brightnessIncrement, theColor.w);//vec4(1.0,.0,1.0,1.0);
}
