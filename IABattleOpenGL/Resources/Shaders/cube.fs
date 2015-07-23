uniform sampler2D u_texture;

varying vec3 v_texCoords;

void main(void)
{
	gl_FragColor = texture2D(u_texture, v_texCoords);
}