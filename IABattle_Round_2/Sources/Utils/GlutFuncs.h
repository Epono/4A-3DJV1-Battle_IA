#include <iostream>

//#include <GL/freeglut.h>

void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void KeyboardFunc(unsigned char, int, int);
void MouseWheelFunc(int, int, int, int);

// Global variables to control the scene
float zoom = -15.0f;
float rotx = 0.0;
float rotz = 0.0f;
float tx = 0;
float ty = 0;
int lastx = 0;
int lasty = 0;
unsigned char Buttons[3] = {0};

//float translationX = 0.f;
//float translationY = 0.f;
//float translationZ = 0.f;

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
		//std::cout << "yaww : " << yaww << " - pitchh : " << pitchh << std::endl;


		float xx = cos(yaw) * cos(pitch);
		float yy = sin(yaw) * cos(pitch);
		float zz = sin(pitch);
		//std::cout << "(" << xx << ", " << yy << ", " << zz << ")" << std::endl;
	}
	else if(Buttons[1]) {
		tx += (float) 0.05f * diffx;
		ty -= (float) 0.05f * diffy;
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
		zoom += step;
		break;
	case 'z':
		ty -= step;
		break;
	case 'e':
		zoom -= step;
		break;
	case 'q':
		tx += step;
		break;
	case 's':
		ty += step;
		break;
	case 'd':
		tx -= step;
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
		zoom += step;
	}
	else {
		// Zoom out
		zoom -= step;
	}
}