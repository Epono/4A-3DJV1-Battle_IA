#pragma once

#include "Point.h"

static const float g_cubeVertices[] = {
	-0.5f, -0.5f, 1.f,		// 0
	0.5f, -0.5f, 1.f,  		// 1	
	0.5f, 0.5f, 1.f,		// 2
	-0.5f, 0.5f, 1.f,		// 3	
	-0.5f, -0.5f, -1.f,		// 4
	0.5f, -0.5f, -1.f,		// 5
	0.5f, 0.5f, -1.f,		// 6
	-0.5f, 0.5f, -1.f		// 7
};

static const unsigned short g_cubeIndices[] = {
	0, 1, 2, // avant
	2, 3, 0,
	3, 2, 6, // haut
	6, 7, 3,
	7, 6, 5, // arriere
	5, 4, 7,
	1, 5, 6, // droite
	6, 2, 1,
	4, 0, 3, // gauche
	3, 7, 4,
	4, 5, 1, // bas
	1, 0, 4
};

static const float g_groundVertices[] = {
	X_MIN, Y_MIN, 0.f,		// 0
	X_MAX, Y_MIN, 0.f,  	// 1	
	X_MAX, Y_MAX, 0.f,		// 2
	X_MIN, Y_MAX, 0.f,		// 3	
	X_MIN, Y_MIN, -1.f,		// 4
	X_MAX, Y_MIN, -1.f,		// 5
	X_MAX, Y_MAX, -1.f,		// 6
	X_MIN, Y_MAX, -1.f		// 7
};