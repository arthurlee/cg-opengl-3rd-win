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
#include <list>
using namespace std;

// VAO : Vertex Array Object
#define numVAOs 1
static GLuint vao[numVAOs];

// VBO : Vertex Buffer Object
#define numVBOs 2
static GLuint vbo[numVBOs];

static unique_ptr<QProgram> renderingProgram;

QCamera camera;
std::list<unique_ptr<QModel>> models;

// allocate variables used in display() function, so that they don't need to be allocated during rendering
static GLuint mvLoc, pLoc;

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

	QModel* pCube = new QModel();
		pCube->setBufferIndex(0)
		.setPositions(cubePositions, sizeof(cubePositions))
		.setLocation(-2.0f, -2.0f, 0.0f)
		.bindBufferData(vbo);

	models.push_back(unique_ptr<QModel>(pCube));

	QModel* pPyramid = new QModel();
	pPyramid->setBufferIndex(1)
		.setPositions(pyramidPositions, sizeof(pyramidPositions))
		.setLocation(0.0f, 2.0f, 0.0f)
		.bindBufferData(vbo);

	models.push_back(unique_ptr<QModel>(pPyramid));
}


int multi_model_init(GLFWwindow* window) {
	renderingProgram.reset(new QProgram());
	if (!renderingProgram->attachShaderFromFile("vertShader-multi-model.glsl", "fragShader-multi-model.glsl")) {
		cout << "Failed to create shader program" << endl;
		return -1;
	}

	camera.setLocation(0.0f, 0.0f, 8.0f)
		.perspective(window, glm::radians(60.0f), 0.1f, 1000.0f);

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

	// draw all models
	// ----------------------------------

	for (const auto& model : models) {
		model->display(vbo, camera, mvLoc, pLoc);
	}
}

QRunnable multi_model_runnable() {
	return QRunnable(multi_model_display, "Chapter4 - multi model", multi_model_init, multi_model_deinit);
}
