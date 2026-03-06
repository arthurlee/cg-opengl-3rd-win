#define _USE_MATH_DEFINES
#include <cmath>

#include "QRunnable.h"
#include "QProgram.h"
#include "QModel.h"

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
//static float cubeLocX, cubeLocY, cubeLocZ;
//static float pyramidLocX, pyramidLocY, pyramidLocZ;
static QModel modelCube, modelPyramid;

// allocate variables used in display() function, so that they don't need to be allocated during rendering
static GLuint mvLoc, pLoc;
static int width, height;
static float aspect;
static glm::mat4 pMat, vMat, mMat, mvMat;
static glm::mat4 tMat, rMat;

static void multi_setupVertices() {
	// 36 vertices, 12 triangles, makes up  2x2x2 cube placed at origin
	float cubePositions[108] = {
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

	float pyramidPositions[54] =
	{ -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,    //front
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,    //right
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  //back
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //left
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, //LF
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f  //RR
	};

	modelCube.setPositions(cubePositions, sizeof(cubePositions));
	modelCube.setBufferIndex(0);
	modelPyramid.setPositions(pyramidPositions, sizeof(pyramidPositions));
	modelPyramid.setBufferIndex(1);

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelCube.getBufferIndex()]);
	glBufferData(GL_ARRAY_BUFFER, modelCube.getPositionCount(), modelCube.getPositions(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelPyramid.getBufferIndex()]);
	glBufferData(GL_ARRAY_BUFFER, modelPyramid.getPositionCount(), modelPyramid.getPositions(), GL_STATIC_DRAW);
}


int multi_model_init(GLFWwindow* window) {
	renderingProgram.reset(new QProgram());
	if (!renderingProgram->attachShaderFromFile("vertShader-multi-model.glsl", "fragShader-multi-model.glsl")) {
		cout << "Failed to create shader program" << endl;
		return -1;
	}

	// build the perspective projection matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;

	pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;

	modelCube.setLocation(-2.0f, -2.0f, 0.0f);
	modelPyramid.setLocation(0.0f, 2.0f, 0.0f);

	multi_setupVertices();

	return 0;
}

void multi_model_deinit() {
	renderingProgram.release();
}

void multi_model_display(GLFWwindow* window, double currentTime, double deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderingProgram->Use();

	// Get the locations of the uniform variables in the shader program
	mvLoc = glGetUniformLocation(renderingProgram->id(), "mv_matrix");
	pLoc = glGetUniformLocation(renderingProgram->id(), "p_matrix");

	// builld the view matrix, and model matrix, then calculate the model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	// draw the cube using buffer #0
	// ----------------------------------

	mMat = glm::translate(glm::mat4(1.0f), modelCube.getLocation());
	mvMat = vMat * mMat;

	// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelCube.getBufferIndex()]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw the cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, modelCube.getPositionCount() / 3);

	// draw the cube using buffer #1
	// ----------------------------------

	mMat = glm::translate(glm::mat4(1.0f), modelPyramid.getLocation());
	mvMat = vMat * mMat;

	// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelPyramid.getBufferIndex()]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw the cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, modelPyramid.getPositionCount() / 3);
}

QRunnable multi_model_runnable() {
	return QRunnable(multi_model_display, "Chapter4 - multi model", multi_model_init, multi_model_deinit);
}
