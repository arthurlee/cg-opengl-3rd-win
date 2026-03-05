#define _USE_MATH_DEFINES
#include <cmath>

#include "QRunnable.h"
#include "QProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
using namespace std;

// VAO : Vertex Array Object
#define numVAOs 1
static GLuint vao[numVAOs];

// VBO : Vertex Buffer Object
#define numVBOs 2
static GLuint vbo[numVBOs];

static unique_ptr<QProgram> renderingProgram;

static float cameraX, cameraY, cameraZ;
static float cubeLocX, cubeLocY, cubeLocZ;

// allocate variables used in display() function, so that they don't need to be allocated during rendering
static GLuint vLoc, pLoc, tfLoc;
static int width, height;
static float aspect;
static glm::mat4 pMat, vMat, mMat, mvMat;
static glm::mat4 tMat, rMat;

void multi_setupVertices() {
	// 36 vertices, 12 triangles, makes up  2x2x2 cube placed at origin
	float vertexPositions[108] = {
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}


int multi_cube_init(GLFWwindow* window) {
	renderingProgram.reset(new QProgram());
	if (!renderingProgram->attachShaderFromFile("vertShader-multi-cube.glsl", "fragShader-multi-cube.glsl")) {
		cout << "Failed to create shader program" << endl;
		return -1;
	}

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 420.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;	// shift down Y to reveall perpective distortion

	multi_setupVertices();

	return 0;
}

void multi_cube_deinit() {
	renderingProgram.release();
}

void multi_cube_display(GLFWwindow* window, double currentTime, double deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderingProgram->Use();

	// Get the locations of the uniform variables in the shader program
	vLoc = glGetUniformLocation(renderingProgram->id(), "v_matrix");
	pLoc = glGetUniformLocation(renderingProgram->id(), "p_matrix");
	tfLoc = glGetUniformLocation(renderingProgram->id(), "tf");

	// build the perspective projection matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;

	pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// builld the view matrix, and model matrix, then calculate the model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	//mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	float timeFactor = (float)currentTime;

	// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform1f(tfLoc, timeFactor);

	// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw the cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
}

QRunnable multi_cube_runnable() {
	return QRunnable(multi_cube_display, "Chapter4 - cube", multi_cube_init, multi_cube_deinit);
}
