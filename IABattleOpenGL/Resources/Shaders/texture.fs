uniform sampler2D u_texture;

varying vec2 v_texCoords;

void main(void) 
{
	gl_FragColor = tex2D(u_texture, v_texCoords);
}