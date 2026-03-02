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

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

// allocate variables used in display() function, so that they don't need to be allocated during rendering
GLuint mvLoc, pLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;
glm::mat4 tMat, rMat;

void setupVertices() {
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


int cube_init(GLFWwindow* window) {
	renderingProgram.reset(new QProgram());
	if (!renderingProgram->attachShaderFromFile("vertShader-cube.glsl", "fragShader-cube.glsl")) {
		cout << "Failed to create shader program" << endl;
		return -1;
	}

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;	// shift down Y to reveall perpective distortion

	setupVertices();
	
	return 0;
}

void cube_deinit() {
	renderingProgram.release();
}

void cube_display(GLFWwindow* window, double currentTime, double deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderingProgram->Use();

	// Get the locations of the uniform variables in the shader program
	mvLoc = glGetUniformLocation(renderingProgram->id(), "mv_matrix");
	pLoc = glGetUniformLocation(renderingProgram->id(), "p_matrix");

	// build the perspective projection matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;

	pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// builld the view matrix, and model matrix, then calculate the model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	//mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	for (int i = 0; i < 24; i++) {
		currentTime += i;
	
		tMat = glm::translate(glm::mat4(1.0f), 
			glm::vec3(
				sin(0.35f * currentTime) * 8.0f, 
				cos(0.52f * currentTime) * 8.0f, 
				sin(0.7f * currentTime) * 8.0f
			)
		);

		// cast angle to float so template deduction matches glm::mat4 (float)
		rMat = glm::rotate(glm::mat4(1.0f), 1.75f * static_cast<float>(currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
		rMat = glm::rotate(rMat, 1.75f * static_cast<float>(currentTime), glm::vec3(1.0f, 0.0f, 0.0f));
		rMat = glm::rotate(rMat, 1.75f * static_cast<float>(currentTime), glm::vec3(0.0f, 0.0f, 1.0f));

	
		mMat = tMat * rMat;	// combine the translation and rotation matrices with the model matrix
		mvMat = vMat * mMat;


		// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	
		// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		// adjust OpenGL settings and draw the cube
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

QRunnable cube_runnable() {
	return QRunnable(cube_display, "Chapter4 - cube", cube_init, cube_deinit);
}
