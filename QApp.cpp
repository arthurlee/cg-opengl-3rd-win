#include "QApp.h"

#include <iostream>
#include <sstream>
using namespace std;

int QApp::Init() {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
		return -1;
	}

	m_initialized = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minorVersion);

	m_window = glfwCreateWindow(m_runnable.width, m_runnable.height, m_runnable.title, nullptr, nullptr);
	if (!m_window) {
		cerr << "Failed to create GLFW window" << endl;
		return -1;
	}

	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}

	glfwSwapInterval(1); // Enable vsync

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	cout << "current OpenGL render: " << vendor << " - " << renderer << endl;

	if (m_runnable.init != nullptr) {
		return m_runnable.init(m_window);
	}

	return 0;
}


int QApp::Run() {

	if (m_runnable.display == nullptr) {
		return -1;
	}

	double lastTime = glfwGetTime();
	double deltaTime = 0.0;
	int frameCount = 0;

	while (!glfwWindowShouldClose(m_window)) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		frameCount++;
		
		m_runnable.display(m_window, currentTime, deltaTime);

		if (deltaTime >= 1.0) {		
			std::stringstream ss;
			ss << m_runnable.title << " - [" << frameCount << " fps]";

			//cout << "FPS: " << ss.str() << endl;
			glfwSetWindowTitle(m_window, ss.str().c_str());

			frameCount = 0;
			lastTime = currentTime;
		}

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	return 0;
}
