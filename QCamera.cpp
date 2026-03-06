
#include "QCamera.h"


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

QCamera::QCamera() {

}

QCamera::~QCamera() {

}

// position
const glm::vec3& QCamera::getLocation() const {
	return m_location;
}

QCamera& QCamera::setLocation(float x, float y, float z) {
	m_location = glm::vec3(x, y, z);
	return *this;
}


QCamera& QCamera::perspective(GLFWwindow* window, float fovy, float zNear, float zFar) {
	glfwGetFramebufferSize(window, &m_width, &m_height);
	m_aspect = (float)m_width / (float)m_height;

	m_pMat = glm::perspective(fovy, m_aspect, zNear, zFar);
	m_vMat = glm::translate(glm::mat4(1.0f), -m_location);

	return *this;
}

const glm::mat4 QCamera::perspectiveMatrix() const {
		return m_pMat;
}

const glm::mat4 QCamera::viewMatrix() const {
	return m_vMat;
}
