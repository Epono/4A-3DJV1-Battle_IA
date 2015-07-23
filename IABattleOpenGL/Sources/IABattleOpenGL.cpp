#include <iostream>
#include <algorithm>
#include <ctime>
#include <tuple>
#include <thread>
#include <chrono>
#include <map>

/******************************************** OpenGL ***********************************************/
// Windows specific
#if _WIN32
#include <Windows.h>
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#endif

// OpenGL 
#define GLEW_STATIC 1
#include <GL/glew.h>

// FreeGLUT
#include <GL/freeglut.h>

// STB
#define STB_IMAGE_IMPLEMENTATION
#include "Common/stb_image.h"

// Shader Util
#include "Common/EsgiShader.h"
EsgiShader basicShader;
EsgiShader textureShader;
EsgiShader groundShader;
EsgiShader skyBoxShader;
EsgiShader characterShader;

int previousTime = 0;

GLuint cubeVBO;
GLuint cubeIBO;
GLuint groundVBO;
GLuint groundIBO;
GLuint skyBoxVBO;
GLuint skyBoxIBO;

GLuint groundTextureObj;
GLuint skyBoxTextureObj;
GLuint lucasTextureObj;
GLuint obiwanTextureObj;

#include "Utils/Cube.h"
/******************************************** Fin OpenGL ***********************************************/
#include "Army.h"
#include "UnitAI.h"
#include "Utils/MatrixUtils.h"
#include "Utils/GlutFuncs.h"

// Prototypes : 
void Initialize();
void CreateFBO(int, int);
void Terminate();
void Render();
void Update(int value);
void DrawUnitsAsCubes(GLuint);
void DrawGround(GLuint program);
void DrawSkyBox(GLuint program);

Army* armyTempA;
Army* armyTempB;

void Initialize() {
	printf("OpenGL Driver Version : %s\n", glGetString(GL_VERSION));
	printf("GPU type : %s\n", glGetString(GL_RENDERER));
	printf("Manufacturer : %s\n", glGetString(GL_VENDOR));
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	int numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	GLenum error = glewInit();
	if(error != GL_NO_ERROR) {
		std::cout << "There was an error during the glewInit(à function : " << std::endl << glewGetErrorString(error) << std::endl;
	}

	//for(int index = 0; index < numExtensions; ++index) {
	//	printf("Extension[%d] : %s\n", index, glGetStringi(GL_EXTENSIONS, index));
	//}

	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	basicShader.LoadVertexShader("Resources/Shaders/basic.vs");
	basicShader.LoadFragmentShader("Resources/Shaders/basic.fs");
	basicShader.Create();

	textureShader.LoadVertexShader("Resources/Shaders/texture.vs");
	textureShader.LoadFragmentShader("Resources/Shaders/texture.fs");
	textureShader.Create();

	groundShader.LoadVertexShader("Resources/Shaders/ground.vs");
	groundShader.LoadFragmentShader("Resources/Shaders/ground.fs");
	groundShader.Create();

	skyBoxShader.LoadVertexShader("Resources/Shaders/skyBox.vs");
	skyBoxShader.LoadFragmentShader("Resources/Shaders/skyBox.fs");
	skyBoxShader.Create();

	characterShader.LoadVertexShader("Resources/Shaders/cube.vs");
	characterShader.LoadFragmentShader("Resources/Shaders/cube.fs");
	characterShader.Create();

	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, g_cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * 2 * 3, g_cubeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &groundVBO);
	glGenBuffers(1, &groundIBO);
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, g_groundVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * 2 * 3, g_cubeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &skyBoxVBO);
	glGenBuffers(1, &skyBoxIBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, g_skyBoxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyBoxIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * 2 * 3, g_cubeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	previousTime = glutGet(GLUT_ELAPSED_TIME);

	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = glutGet(GLUT_WINDOW_WIDTH);


	// Textures creation
	int x, y, n;
	unsigned char *data;

	data = stbi_load("Resources/Textures/ground_2048x2048.jpg", &x, &y, &n, STBI_rgb_alpha);

	glGenTextures(1, &groundTextureObj);
	glBindTexture(GL_TEXTURE_2D, groundTextureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Lucas texture
	const char* lucasImages[6] = {
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
	};

	glGenTextures(6, &lucasTextureObj);
	glBindTexture(GL_TEXTURE_2D, lucasTextureObj);

	for(int i = 0; i < 6; i++) {
		data = stbi_load(lucasImages[i], &x, &y, &n, STBI_rgb_alpha);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Obiwan texture
	const char* obiwanTextures[6] = {
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
	};

	glGenTextures(6, &obiwanTextureObj);
	glBindTexture(GL_TEXTURE_2D, obiwanTextureObj);

	for(GLuint i = 0; i < 6; i++) {
		data = stbi_load(obiwanTextures[i], &x, &y, &n, STBI_rgb_alpha);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Skybox texture
	const char* skyBoxTextures[6] = {
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
		"Resources/Textures/obiwan.jpg",
	};

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &skyBoxTextureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureObj);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for(GLuint i = 0; i < 6; i++) {
		data = stbi_load(skyBoxTextures[i], &x, &y, &n, STBI_rgb_alpha);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terminate() {
	glDeleteTextures(1, &groundTextureObj);
	glDeleteTextures(1, &skyBoxTextureObj);
	glDeleteTextures(1, &lucasTextureObj);
	glDeleteTextures(1, &obiwanTextureObj);
	glDeleteBuffers(1, &cubeIBO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &groundIBO);
	glDeleteBuffers(1, &groundVBO);
	glDeleteBuffers(1, &skyBoxIBO);
	glDeleteBuffers(1, &skyBoxVBO);

	basicShader.Destroy();
	textureShader.Destroy();
	groundShader.Destroy();
	characterShader.Destroy();
	skyBoxShader.Destroy();
}

void Render() {
	// Sky color (temp)
	glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);

	// Characters rendering
	GLuint program = characterShader.GetProgram();
	glUseProgram(program);
	DrawUnitsAsCubes(program);

	// Ground rendering
	program = groundShader.GetProgram();
	glUseProgram(program);
	glFrontFace(GL_CCW);
	DrawGround(program);

	// SkyBlox rendering
	program = skyBoxShader.GetProgram();
	glUseProgram(program);
	glFrontFace(GL_CW);
	DrawSkyBox(program);

	//RenderString(20, 20, (unsigned char*) "HELLO", writingColor);
	//write();

	glutSwapBuffers();
	glutPostRedisplay();
}

void DrawUnitsAsCubes(GLuint program) {
	Transformations(program, rotx, rotz, tx, ty, tz);

	// passage des attributs de sommet au shader
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lucasTextureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);

	GLint offsetLocation = glGetUniformLocation(program, "u_offset");
	glUniform3f(offsetLocation, 0, 0, 0);

	// Display units of armyTempA
	GLint colorLocation = glGetUniformLocation(program, "u_color");
	glUniform3f(colorLocation, 0.1f, 0.1f, 0.1f);
	for(auto & u : armyTempA->getUnitList()) {
		glUniform3f(offsetLocation, u->getPosition().getX() - 0.5f, u->getPosition().getY() + 0.5f, 1.f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obiwanTextureObj);
	textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);
	// Display units of armyTempB
	glUniform3f(colorLocation, 0.5f, 0.5f, 0.5f);
	for(auto & u : armyTempB->getUnitList()) {
		glUniform3f(offsetLocation, u->getPosition().getX() - 0.5f, u->getPosition().getY() + 0.5f, 1.f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawGround(GLuint program) {
	Transformations(program, rotx, rotz, tx, ty, tz);

	// passage des attributs de sommet au shader
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTextureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIBO);
	glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawSkyBox(GLuint program) {
	Transformations(program, rotx, rotz, tx, ty, tz);

	// passage des attributs de sommet au shader
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyBoxIBO);
	glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

UnitAI ia;
int nbTours = 0;
std::vector<FighterWrapper> unitesCombat;

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("BATTLE !");

#ifdef FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
				  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	Initialize();

	glutDisplayFunc(Render);
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutMouseWheelFunc(MouseWheelFunc);
	glutTimerFunc(0, Update, 0);

	armyTempA = new Army(100, 20);
	armyTempB = new Army(100, 20);

	glutMainLoop();

	Terminate();
	return 0;
}

// Mainloop
void Update(int value) {
	nbTours++;

	//Récupération des unités pour le tour
	unitesCombat.clear();
	for(auto & u : armyTempA->getUnitList())
		unitesCombat.push_back(FighterWrapper(*u, *armyTempA, *armyTempB));
	for(auto & u : armyTempB->getUnitList())
		unitesCombat.push_back(FighterWrapper(*u, *armyTempB, *armyTempA));

	while(unitesCombat.size() > 0 && armyTempA->size() > 0 && armyTempB->size() > 0) {
		std::random_shuffle(unitesCombat.begin(), unitesCombat.end());
		FighterWrapper fighter = unitesCombat.back();
		Action* a = &ia(fighter.m_fighter, fighter.m_allies, fighter.m_ennemies);
		a->execute();
		delete (a);
		fighter.m_ennemies->purge();
		fighter.m_allies->purge();
		unitesCombat.pop_back();
	}
	if(armyTempA->size() == 0 || armyTempB->size() == 0) {
		glutLeaveMainLoop();
	}
	else {
		glutTimerFunc(500, Update, 0);
	}
}


// Unused

/*
typedef struct color_rgb {
color_rgb(float r, float g, float b) :_r(r), _g(g), _b(b) {
}
float _r;
float _g;
float _b;
} color_rgb;

color_rgb writingColor = color_rgb(1.f, 1.f, 0.f);

void write();
void write() {
char* truc = "Selecting point";
//glRasterPos2f(100, 100);
glWindowPos2d(10, 10);

for(int i = 0; truc[i] != '\0'; i++) {
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc[i]);
}
}

void RenderString(float x, float y, const unsigned char* string, color_rgb const& rgb);
void RenderString(float x, float y, const unsigned char* string, color_rgb const& rgb) {
glColor3f(rgb._r, rgb._g, rgb._b);
//glWindowPos2d(x, y);
glRasterPos2f(x, y);

glutBitmapString(GLUT_BITMAP_HELVETICA_18, string);
}
*/