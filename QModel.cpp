#include "QModel.h"

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

void QModel::bindBufferData(GLuint vbo[]) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[m_bufferIndex]);
	glBufferData(GL_ARRAY_BUFFER, getPositionCount(), getPositions(), GL_STATIC_DRAW);
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
