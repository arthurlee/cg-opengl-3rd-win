#pragma once

#include <glm/glm.hpp>

class QModel
{
public:
	QModel();
	~QModel();

public:
	// position
	const glm::vec3& getLocation() const;
	void setLocation(float x, float y, float z);

	// positions
	// positions are stored as an array of floats, with 3 floats per vertex (x, y, z)
	const float* getPositions() const;
	int getPositionCount() const;
	void setPositions(const float* positions, int positionCount);

	// buffer index
	int getBufferIndex() const;
	void setBufferIndex(int bufferIndex);

private:
	glm::vec3 m_location;
	const float* m_positions;
	int m_positionCount;
	int m_bufferIndex;	// index of the VBO buffer that contains the position data for this model
};

