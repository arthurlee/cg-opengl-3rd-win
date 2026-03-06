#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class QCamera
{
public:
	QCamera();
	~QCamera();

public:
	QCamera& perspective(GLFWwindow* window, float fovy, float zNear, float zFar);

	// position
	const glm::vec3& getLocation() const;
	QCamera& setLocation(float x, float y, float z);

	const glm::mat4 perspectiveMatrix() const;
	const glm::mat4 viewMatrix() const;

public:
	int m_width;
	int m_height;
	float m_aspect;
	glm::mat4 m_pMat;
	glm::mat4 m_vMat;

	glm::vec3 m_location;
};

