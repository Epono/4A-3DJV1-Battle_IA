attribute vec4 a_position;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

uniform vec3 u_offset;
uniform vec3 u_color;

varying vec3 v_texCoords;

void main(void)
{
	v_texCoords = a_position;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * (a_position + vec4(u_offset, 0.0));
}