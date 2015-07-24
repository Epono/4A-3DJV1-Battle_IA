uniform samplerCube u_texture;

varying vec3 v_texCoords;

void main(void) 
{
	gl_FragColor = textureCube(u_texture, v_texCoords);
}