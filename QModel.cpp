#include "QModel.h"


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

QModel::QModel() 
	: m_location(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_bufferIndex(0) {
}

QModel::~QModel() {
}

// position
const glm::vec3& QModel::getLocation() const {
	return m_location; 
}

QModel& QModel::setLocation(float x, float y, float z) {
	m_location = glm::vec3(x, y, z);
	m_mMat = glm::translate(glm::mat4(1.0f), m_location);
	return *this;
}

const float* QModel::getPositions() const {
	return (const float *)m_positionBuffer.data();
}

int QModel::getPositionCount() const {
	return m_positionBuffer.length() / sizeof(float);
}

int QModel::vertexCount() const {
	return getPositionCount() / 3;
}

QModel& QModel::bindBufferData(GLuint vbo[]) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[m_bufferIndex]);
	glBufferData(GL_ARRAY_BUFFER, getPositionCount(), getPositions(), GL_STATIC_DRAW);
	return *this;
}

// todo: allocate memory for positions and copy the data, instead of just storing the pointer
QModel& QModel::setPositions(const float* positions, int positionCount) {
	m_positionBuffer.append(positions, positionCount * sizeof(float));
	return *this;
}

// buffer index
int QModel::getBufferIndex() const { 
	return m_bufferIndex; 
}

QModel& QModel::setBufferIndex(int bufferIndex) {
	m_bufferIndex = bufferIndex; 
	return *this;
}

const glm::mat4 QModel::modelMatrix() const {
	return m_mMat;
}

void QModel::display(GLuint vbo[], QCamera& camera, GLuint mvLoc, GLuint pLoc) {
	glm::mat4 mvMat = camera.viewMatrix() * m_mMat;

	// copy the projection and model-view matrices to the corresponding uniform variables in the shader program
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(camera.perspectiveMatrix()));

	// associate the vertex data with the corresponding attribute variable in the shader program, and enable the generic vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, vbo[m_bufferIndex]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw the cube
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount());
}