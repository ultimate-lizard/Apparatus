#version 330 core

out vec4 color;
in vec4 vertColor;

uniform bool selected;
uniform vec4 selectionColor;

void main()
{
	if (selected)
	{
		color = selectionColor;
	}
	else
	{
		color = vertColor;
	}	
}
