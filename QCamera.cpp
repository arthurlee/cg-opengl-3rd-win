
#include "QCamera.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using namespace std;

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

	cout << "Camera perspective: " << endl
		<< "  width: " << m_width << endl
		<< "  height: " << m_height << endl
		<< "  aspect: " << m_aspect << endl
		<< "  fovy: " << fovy << endl
		<< "  zNear: " << zNear << endl
		<< "  zFar: " << zFar << endl;

	cout << "Camera perspective matrix: " << endl;
	const float* p = (const float*)glm::value_ptr(m_pMat);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << p[j * 4 + i] << " ";
		}
		cout << endl;
	}

	cout << "Camera view matrix: " << endl;
	p = (const float*)glm::value_ptr(m_vMat);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << p[j * 4 + i] << " ";
		}
		cout << endl;
	}

	return *this;
}

const glm::mat4 QCamera::perspectiveMatrix() const {
		return m_pMat;
}

const glm::mat4 QCamera::viewMatrix() const {
	return m_vMat;
}
