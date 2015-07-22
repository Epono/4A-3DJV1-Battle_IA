uniform sampler2D u_texture;

varying vec4 v_color;
varying vec3 v_texCoords;

void main(void)
{
	gl_FragColor = texture(u_texture, v_texCoords);
	//gl_FragColor = v_color;
}