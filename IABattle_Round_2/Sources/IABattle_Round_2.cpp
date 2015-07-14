#include <iostream>
#include <algorithm>
#include <ctime>
#include <tuple>
#include <thread>
#include <chrono>
#include <map>

#include "Army.h"
#include "UnitAI.h"

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

int previousTime = 0;

GLuint cubeVBO;
GLuint cubeIBO;
GLuint textureObj;
GLuint sceneFBO;

#include "Utils/Cube.h"
/******************************************** Trucs OpenGL ***********************************************/

// Prototypes : 
void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void KeyboardFunc(unsigned char, int, int);
void MouseWheelFunc(int, int, int, int);
void Initialize();
void CreateFBO(int, int);
void Terminate();
void Identity(float);
void Orthographic(float, float, float, float, float, float, float);
void Perspective(float, float, float, float, float, float);
void MatrixProduct_4x4(float, const float, const float);
void Translate(float, float, float, float);
void RotateX(float, float);
void RotateY(float, float);
void RotateZ(float, float);
void Render();
void Update();
void DrawCube(GLuint);

// Global variables to control the scene
float zoom = 15.0f;
float rotx = 0.0;
float rotz = 0.0f;
float tx = 0;
float ty = 0;
int lastx = 0;
int lasty = 0;
unsigned char Buttons[3] = {0};

float translationX = 0.f;
float translationY = 0.f;
float translationZ = 0.f;

float step = 0.5f;

/*
* Function in charge of handling mouse events (clicking only, not dragging)
*/
void MouseFunc(int button, int state, int x, int y) {
	lastx = x;
	lasty = y;
	switch(button) {
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN == state) ? 1 : 0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

/*
* Function in charge of handling mouse events (dragging only, not clicking)
*/
void MotionFunc(int x, int y) {
	int diffx = x - lastx;
	int diffy = y - lasty;
	lastx = x;
	lasty = y;

	if(Buttons[0] && Buttons[1]) {
		zoom -= (float) 0.05f * diffx;
	}
	else {

		if(Buttons[0]) {
			if(rotx + (float) 0.5f * diffy <= 0 && rotx + (float) 0.5f * diffy >= -60) {
				rotx += (float) 0.5f * diffy;
			}
			rotz += (float) 0.5f * diffx;
			if(rotz > 125) {
				rotz -= 125;
			}
			else if(rotz < 0) {
				rotz += 125;
			}

			float yaww = ((rotz / 125) * 360) - 90;
			float pitchh = (rotx * -3);

			float yaw = yaww * (3.14 / 180);
			float pitch = pitchh * (3.14 / 180);
			std::cout << "yaww : " << yaww << " - pitchh : " << pitchh << std::endl;


			float xx = cos(yaw) * cos(pitch);
			float yy = sin(yaw) * cos(pitch);
			float zz = sin(pitch);
			//std::cout << "(" << xx << ", " << yy << ", " << zz << ")" << std::endl;
		}
		else {

			if(Buttons[1]) {
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
		}
	}

	glutPostRedisplay();
}

/*
* Function in charge of handling keyboard events
* key : key pressed
* x, y : coordinates of the pointer when the key was pressed ?
*/
void KeyboardFunc(unsigned char key, int x, int y) {
	switch(key) {
	case 'a':
		translationZ += step;
		break;
	case 'z':
		translationY -= step;
		break;
	case 'e':
		translationZ -= step;
		break;
	case 'q':
		translationX += step;
		break;
	case 's':
		translationY += step;
		break;
	case 'd':
		translationX -= step;
		break;
	case '+':
		step += 0.1f;
		break;
	case '-':
		step -= 0.1f;
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}

/*
* Function in charge of handling mouse scroll
*/
void MouseWheelFunc(int button, int dir, int x, int y) {
	if(dir > 0) {
		// Zoom in
		translationZ += step;
	}
	else {
		// Zoom out
		translationZ -= step;
	}
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
	//glFrontFace(GL_CW);

	basicShader.LoadVertexShader("Resources/Shaders/basic.vs");
	basicShader.LoadFragmentShader("Resources/Shaders/basic.fs");
	basicShader.Create();

	textureShader.LoadVertexShader("Resources/Shaders/texture.vs");
	textureShader.LoadFragmentShader("Resources/Shaders/texture.fs");
	textureShader.Create();

	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, g_cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * 2 * 3, g_cubeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	previousTime = glutGet(GLUT_ELAPSED_TIME);

	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = glutGet(GLUT_WINDOW_WIDTH);

	CreateFBO(width, height);
}

void CreateFBO(int width, int height) {
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureObj, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Terminate() {
	glDeleteTextures(1, &textureObj);
	glDeleteBuffers(1, &cubeIBO);
	glDeleteBuffers(1, &cubeVBO);

	basicShader.Destroy();
	textureShader.Destroy();

	glDeleteFramebuffers(1, &sceneFBO);
}

void Identity(float *matrix) {
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 1.0f; matrix[5] = 1.0f; matrix[10] = 1.0f; matrix[15] = 1.0f;
}

void Orthographic(float *matrix, float L, float R, float T, float B, float N, float F) {
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 2.f / (R - L);
	matrix[5] = 2.f / (T - B);
	matrix[10] = -2.f / (F - N);
	matrix[12] = -(R + L) / (R - L);
	matrix[13] = -(T + B) / (T - B);
	matrix[14] = -(F + N) / (F - N);
	matrix[15] = 1.f;
}

void Perspective(float *matrix, float FOV, float width, float height, float N, float F) {
	memset(matrix, 0, sizeof(float) * 16);
	float aspect = width / height;

	float xymax = N * tan(FOV * (3.141592f / 180.f) * 0.5f);
	float ymin = -xymax;
	float xmin = -xymax;
	width = xymax - xmin;
	height = xymax - ymin;
	float depth = F - N;
	float q = -(F + N) / depth;
	float qn = -2.0f * (F*N) / depth;

	float w = 2.0f * N / width;
	w = w / aspect;
	float h = 2.0f * N / height;

	matrix[0] = w;
	matrix[5] = h;
	matrix[10] = q;
	matrix[11] = -1.f;
	matrix[14] = qn;
}

void MatrixProduct_4x4(float *matrix, const float *A, const float *B) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			matrix[i * 4 + j] = 0;
			for(int k = 0; k < 4; ++k) {
				matrix[i * 4 + j] += A[i * 4 + k] * B[k * 4 + j];
			}
		}
	}
}

void Translate(float *matrix, float tx, float ty, float tz = 0.f) {
	Identity(matrix);
	matrix[12] = tx;
	matrix[13] = ty;
	matrix[14] = tz;
}

void RotateX(float *matrix, float angle) {
	Identity(matrix);
	matrix[5] = cos(angle);	matrix[9] = -sin(angle);
	matrix[6] = sin(angle);	matrix[10] = cos(angle);
}

void RotateY(float *matrix, float angle) {
	Identity(matrix);
	matrix[0] = cos(angle);	matrix[8] = sin(angle);
	matrix[2] = -sin(angle); matrix[10] = cos(angle);
}

void RotateZ(float *matrix, float angle) {
	Identity(matrix);
	matrix[0] = cos(angle); matrix[4] = -sin(angle);
	matrix[1] = sin(angle); matrix[5] = cos(angle);
}

void Render() {
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint program = basicShader.GetProgram();
	glUseProgram(program);

	//DrawCube(program);

	glUseProgram(0);

#if COPY_FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, sceneFBO);
	glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program = textureShader.GetProgram();
	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture");
	glUniform1i(textureLocation, 0);
	DrawCube(program);
#endif

	glutSwapBuffers();
	glutPostRedisplay();
}

void Update() {
	//printf("Je m'affiche a un rythme de 60 fps ! (je crois)\n");
}

void DrawCube(GLuint program) {
	float w = (float) glutGet(GLUT_WINDOW_WIDTH);
	float h = (float) glutGet(GLUT_WINDOW_HEIGHT);

	// passage des attributs de sommet au shader

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	// variables uniformes (constantes) durant le rendu de la primitive

	float projection[16];
	//Orthographic(projection, 0, w, h, 0, -1.f, 1.f);
	Perspective(projection, 45.f, w, h, 0.1f, 1000.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int delta = currentTime - previousTime;
	previousTime = currentTime;
	static float time = 1.f;
	time += delta / 1000.f;
	GLint timeLocation = glGetUniformLocation(program, "u_time");
	glUniform1f(timeLocation, time);

	float viewTransform[16];
	Identity(viewTransform);
	viewTransform[14] = -7.0f;
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	float worldTransform[16];
	Identity(worldTransform);

	float A[16], B[16], C[16], worldTransformTemp[16];

	Translate(A, translationX, translationY, translationZ);
	RotateZ(B, rotz / 20);
	RotateX(C, rotx / 20);
	//RotateY(B, rotx / 10); //Comme X, dans un autre sens

	MatrixProduct_4x4(worldTransformTemp, A, B);
	MatrixProduct_4x4(worldTransform, worldTransformTemp, C);

	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

	GLint offsetLocation = glGetUniformLocation(program, "u_offset");
	glUniform3f(offsetLocation, 0, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);

	for(int i = -10; i <= 10; i++) {
		for(int j = -10; j <= 10; j++) {
			glUniform3f(offsetLocation, (i * 3) + time, j * 3, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
			glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);
		}
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
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
	glutIdleFunc(Update);

	glutMainLoop();

	Terminate();

	return 0;
}