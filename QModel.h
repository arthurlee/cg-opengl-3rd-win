#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "QBuffer.h"
#include "QCamera.h"

class QModel
{
public:
	QModel();
	~QModel();

public:
	// position
	const glm::vec3& getLocation() const;
	QModel& setLocation(float x, float y, float z);

	// positions
	// positions are stored as an array of floats, with 3 floats per vertex (x, y, z)
	const float* getPositions() const;
	int vertexCount() const;
	int getPositionCount() const;
	QModel& setPositions(const float* positions, int positionCount);
	QModel& bindBufferData(GLuint vbo[]);

	// buffer index
	int getBufferIndex() const;
	QModel& setBufferIndex(int bufferIndex);

	const glm::mat4 modelMatrix() const;

	void display(GLuint vbo[], QCamera &camera, GLuint mvLoc, GLuint pLoc);

private:
	glm::vec3 m_location;
	QBuffer m_positionBuffer;
	//const float* m_positions;
	//int m_positionCount;
	int m_bufferIndex;	// index of the VBO buffer that contains the position data for this model

	glm::mat4 m_mMat;	// model matrix
};

