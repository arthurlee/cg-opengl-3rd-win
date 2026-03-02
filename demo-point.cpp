#define _USE_MATH_DEFINES
#include <cmath>

#include "QRunnable.h"
#include "QProgram.h"

#include <iostream>
#include <memory>
using namespace std;

// VAO : Vertex Array Object
#define numVAOs 1
static GLuint vao[numVAOs];

float x = 0.0f;
float rad = 0.0f;
//float inc = 0.01f;

unique_ptr<QProgram> renderingProgram;

int point_init(GLFWwindow* window) {
	renderingProgram.reset(new QProgram());

	if (!renderingProgram->attachShaderFromFile("vertShader.glsl", "fragShader.glsl")) {
		cout << "Failed to create shader program" << endl;
		return -1;
	}

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	return 0;
}

void point_deinit() {
	renderingProgram.release();
}

void point_display(GLFWwindow* window, double currentTime, double deltaTime) {
	//cout << "point_display: " << currentTime << endl;

	if (renderingProgram->isValid()) {
		renderingProgram->Use();
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (deltaTime <= 0.5f) {
		x = float(deltaTime * 4 - 1.0);
	} else {
		x = float( -((deltaTime * 4) - 3.0));
	}
	rad = float(deltaTime * 2.0f * M_PI);

	//cout << "deltaTime: " << deltaTime << ", x: " << x << endl;

	GLuint offsetLoc = glGetUniformLocation(renderingProgram->id(), "offset");
	glProgramUniform1f(renderingProgram->id(), offsetLoc, x);

	GLuint radLoc = glGetUniformLocation(renderingProgram->id(), "rad");
	glProgramUniform1f(renderingProgram->id(), radLoc, rad);


	//glPointSize(40.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

QRunnable point_runnable() {
	return QRunnable(point_display, "Chapter2 - program2", point_init, point_deinit);
}
