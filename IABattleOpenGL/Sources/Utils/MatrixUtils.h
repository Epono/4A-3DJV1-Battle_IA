#include <algorithm>

void Identity(float);
void Orthographic(float, float, float, float, float, float, float);
void Perspective(float, float, float, float, float, float);
void MatrixProduct_4x4(float, const float, const float);
void Translate(float, float, float, float);
void RotateX(float, float);
void RotateY(float, float);
void RotateZ(float, float);
void Transformations(GLuint program, float rotx, float rotz, float tx, float ty, float tz);

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

void Transformations(GLuint program, float rotx, float rotz, float tx, float ty, float tz) {
	float w = (float) glutGet(GLUT_WINDOW_WIDTH);
	float h = (float) glutGet(GLUT_WINDOW_HEIGHT);

	// variables uniformes (constantes) durant le rendu de la primitive
	float projection[16];
	Perspective(projection, 45.f, w, h, 1.f, 300.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	float viewTransform[16];
	Identity(viewTransform);
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	float worldTransform[16];
	Identity(worldTransform);

	float A[16], B[16], C[16], worldTransformTemp[16];

	Translate(A, tx, ty, tz);
	RotateZ(B, rotz);
	RotateX(C, rotx);

	MatrixProduct_4x4(worldTransformTemp, A, B);
	MatrixProduct_4x4(worldTransform, worldTransformTemp, C);

	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);
}