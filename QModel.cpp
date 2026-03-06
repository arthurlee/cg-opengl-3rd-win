#include "QModel.h"

QModel::QModel() 
	: m_location(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_positions(nullptr) 
	, m_positionCount(0)
	, m_bufferIndex(0) {
}

QModel::~QModel() {
}

// position
const glm::vec3& QModel::getLocation() const {
	return m_location; 
}

void QModel::setLocation(float x, float y, float z) { 
	m_location = glm::vec3(x, y, z); 
}

const float* QModel::getPositions() const {
	return m_positions;
}

int QModel::getPositionCount() const {
	return m_positionCount; 
}

// todo: allocate memory for positions and copy the data, instead of just storing the pointer
void QModel::setPositions(const float* positions, int positionCount) {
	m_positionCount = positionCount;
	m_positions = positions;
}

// buffer index
int QModel::getBufferIndex() const { 
	return m_bufferIndex; 
}

void QModel::setBufferIndex(int bufferIndex) { 
	m_bufferIndex = bufferIndex; 
}
