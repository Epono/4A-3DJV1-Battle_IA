uniform sampler2D u_texture;

varying vec2 v_texCoords;

void main(void) 
{
	gl_FragColor = texture(u_texture, v_texCoords);
}