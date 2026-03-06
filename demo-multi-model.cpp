#define _USE_MATH_DEFINES
#include <cmath>

#include "QRunnable.h"
#include "QProgram.h"
#include "QModel.h"
#include "QCamera.h"

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

QCamera camera;
//static float cameraX, cameraY, cameraZ;
//static float cubeLocX, cubeLocY, cubeLocZ;
//static float pyramidLocX, pyramidLocY, pyramidLocZ;
static QModel modelCube, modelPyramid;

// allocate variables used in display() function, so that they don't need to be allocated during rendering
static GLuint mvLoc, pLoc;
static int width, height;
static float aspect;
static glm::mat4 pMat, vMat, mMat, mvMat;
static glm::mat4 tMat, rMat;

static void multi_setupModels() {
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

	modelCube.setPositions(cubePositions, sizeof(cubePositions))
			.setBufferIndex(0)
			.setLocation(-2.0f, -2.0f, 0.0f)
			.bindBufferData(vbo);

	modelPyramid.setPositions(pyramidPositions, sizeof(pyramidPositions))
				.setBufferIndex(1)
				.setLocation(0.0f, 2.0f, 0.0f)
				.bindBufferData(vbo);
}


int multi_model_init(GLFWwindow* window) {
	renderingProgram.reset(new QProgram());
	if (!renderingProgram->attachShaderFromFile("vertShader-multi-model.glsl", "fragShader-multi-model.glsl")) {
		cout << "Failed to create shader program" << endl;
		return -1;
	}

	// build the perspective projection matrix
	//glfwGetFramebufferSize(window, &width, &height);
	//aspect = (float)width / (float)height;

	//pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	camera.setLocation(0.0f, 0.0f, 8.0f)
		.perspective(window, glm::radians(60.0f), 0.1f, 1000.0f);
	//cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);

	multi_setupModels();

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
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	// draw the cube using buffer #0
	// ----------------------------------

	mMat = glm::translate(glm::mat4(1.0f), modelCube.getLocation());
	mvMat = camera.viewMatrix() * mMat;

	// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(camera.perspectiveMatrix()));

	// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelCube.getBufferIndex()]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw the cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, modelCube.vertexCount());

	// draw the cube using buffer #1
	// ----------------------------------

	mMat = glm::translate(glm::mat4(1.0f), modelPyramid.getLocation());
	mvMat = camera.viewMatrix() * mMat;

	// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(camera.perspectiveMatrix()));

	// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelPyramid.getBufferIndex()]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw the cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, modelPyramid.vertexCount());
}

QRunnable multi_model_runnable() {
	return QRunnable(multi_model_display, "Chapter4 - multi model", multi_model_init, multi_model_deinit);
}
