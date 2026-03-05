#version 430

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 p_matrix;

in vec4 varyingColor;

void main(void)
{
	// color = vec4(0.75, 0.5, 0.25, 1.0);
	color = varyingColor;
}