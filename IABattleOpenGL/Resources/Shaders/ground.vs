attribute vec4 a_position;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

uniform vec3 u_offset;
uniform vec3 u_color;

varying vec2 v_texCoords;
varying vec4 v_color;

void main(void)
{
	v_texCoords = (a_position / 2.0) + 0.5;
	v_color = vec4(u_color, 1.0);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * (a_position + vec4(u_offset, 0.0));
}