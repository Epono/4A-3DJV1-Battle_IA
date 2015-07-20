#include <iostream>
#include <algorithm>
#include <ctime>
#include <tuple>
#include <thread>
#include <chrono>
#include <map>

/******************************************** Trucs OpenGL ***********************************************/
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
EsgiShader cubeShader;

int previousTime = 0;

GLuint cubeVBO;
GLuint cubeIBO;
GLuint groundVBO;
GLuint groundIBO;
GLuint groundTextureObj;

GLuint characterTextureObj;

#include "Utils/Cube.h"
/******************************************** Trucs OpenGL ***********************************************/
#include "Army.h"
#include "UnitAI.h"
#include "Utils/MatrixUtils.h"
#include "Utils/GlutFuncs.h"

typedef struct color_rgb {
	color_rgb(float r, float g, float b) :_r(r), _g(g), _b(b) {
	}
	float _r;
	float _g;
	float _b;
} color_rgb;

// Prototypes : 
void Initialize();
void CreateFBO(int, int);
void Terminate();
void Render();
void Update(int value);
void DrawUnitsAsCubes(GLuint);
void DrawGround(GLuint program);
void RenderString(float x, float y, const unsigned char* string, color_rgb const& rgb);

color_rgb writingColor = color_rgb(1.f, 1.f, 0.f);

Army* armyTempA;
Army* armyTempB;

void write() {
	char* truc = "Selecting point";
	//glRasterPos2f(100, 100);
	glWindowPos2d(10, 10);

	for(int i = 0; truc[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc[i]);
	}
}

void RenderString(float x, float y, const unsigned char* string, color_rgb const& rgb) {
	glColor3f(rgb._r, rgb._g, rgb._b);
	//glWindowPos2d(x, y);
	glRasterPos2f(x, y);

	glutBitmapString(GLUT_BITMAP_HELVETICA_18, string);
}

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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

	cubeShader.LoadVertexShader("Resources/Shaders/cube.vs");
	cubeShader.LoadFragmentShader("Resources/Shaders/cube.fs");
	cubeShader.Create();

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

	previousTime = glutGet(GLUT_ELAPSED_TIME);

	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = glutGet(GLUT_WINDOW_WIDTH);





	int x, y, n;
	unsigned char *data = stbi_load("Resources/Textures/ground_2048x2048.jpg", &x, &y, &n, STBI_rgb_alpha);

	glGenTextures(1, &groundTextureObj);
	glBindTexture(GL_TEXTURE_2D, groundTextureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Character texture
	const char* images[] = {
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
		"Resources/Textures/lucas.jpg",
	};

	glGenTextures(1, &characterTextureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, characterTextureObj);

	x = -1, y = -1, n = -1;
	for(GLuint i = 0; i < 6; i++) {
		data = stbi_load(images[i], &x, &y, &n, STBI_rgb_alpha);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Terminate() {
	glDeleteTextures(1, &groundTextureObj);
	glDeleteTextures(1, &characterTextureObj);
	glDeleteBuffers(1, &cubeIBO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &groundIBO);
	glDeleteBuffers(1, &groundVBO);

	basicShader.Destroy();
	textureShader.Destroy();
	groundShader.Destroy();
	cubeShader.Destroy();
}

void Render() {
	// Sky color (temp)
	glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);

	// Characters rendering
	GLuint program = cubeShader.GetProgram();
	glUseProgram(program);
	DrawUnitsAsCubes(program);

	// Ground rendering
	program = groundShader.GetProgram();
	glUseProgram(program);
	glFrontFace(GL_CCW);
	DrawGround(program);

	//RenderString(20, 20, (unsigned char*) "HELLO", writingColor);
	//write();

	glutSwapBuffers();
	glutPostRedisplay();
}

void DrawUnitsAsCubes(GLuint program) {
	float w = (float) glutGet(GLUT_WINDOW_WIDTH);
	float h = (float) glutGet(GLUT_WINDOW_HEIGHT);

	// passage des attributs de sommet au shader
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, characterTextureObj);
	// glBindTexture(GL_TEXTURE_2D, groundTextureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);

	// variables uniformes (constantes) durant le rendu de la primitive
	float projection[16];
	Perspective(projection, 45.f, w, h, 1.f, 150.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	float viewTransform[16];
	Identity(viewTransform);
	viewTransform[14] = -7.0f;
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	float worldTransform[16];
	Identity(worldTransform);

	float A[16], B[16], C[16], worldTransformTemp[16];

	Translate(A, tx, ty, tz);
	RotateZ(B, rotz / 20);
	RotateX(C, rotx / 20);

	MatrixProduct_4x4(worldTransformTemp, A, B);
	MatrixProduct_4x4(worldTransform, worldTransformTemp, C);

	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

	GLint offsetLocation = glGetUniformLocation(program, "u_offset");
	glUniform3f(offsetLocation, 0, 0, 0);

	// Display units of armyTempA
	GLint colorLocation = glGetUniformLocation(program, "u_color");
	glUniform3f(colorLocation, 0.1f, 0.1f, 0.1f);
	for(auto & u : armyTempA->getUnitList()) {
		glUniform3f(offsetLocation, u->getPosition().getX() - 0.5f, u->getPosition().getY() - 0.5f, 1.f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);
	}
	// Display units of armyTempB
	glUniform3f(colorLocation, 0.5f, 0.5f, 0.5f);
	for(auto & u : armyTempB->getUnitList()) {
		glUniform3f(offsetLocation, u->getPosition().getX() - 0.5f, u->getPosition().getY() - 0.5f, 1.f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void DrawGround(GLuint program) {
	float w = (float) glutGet(GLUT_WINDOW_WIDTH);
	float h = (float) glutGet(GLUT_WINDOW_HEIGHT);

	// passage des attributs de sommet au shader
	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTextureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);

	// variables uniformes (constantes) durant le rendu de la primitive
	float projection[16];
	Perspective(projection, 45.f, w, h, 0.1f, 150.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	float viewTransform[16];
	Identity(viewTransform);
	viewTransform[14] = -7.0f;
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	float worldTransform[16];
	Identity(worldTransform);

	float A[16], B[16], C[16], worldTransformTemp[16];

	Translate(A, tx, ty, tz);
	RotateZ(B, rotz / 20);
	RotateX(C, rotx / 20);
	//RotateY(B, rotx / 10); //Comme X, dans un autre sens

	MatrixProduct_4x4(worldTransformTemp, A, B);
	MatrixProduct_4x4(worldTransform, worldTransformTemp, C);

	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundIBO);
	glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
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

void Update(int value) {
	//printf("Je m'affiche a un rythme de 60 fps ! (je crois)\n");
	nbTours++;
	//std::cout << "========== Tour " << nbTours << " ==========" << std::endl;
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