#pragma once

#include "QRunnable.h"

class QApp
{
public:
	QApp(const QRunnable& runnable, int major = 4, int minor = 3)
		: m_runnable(runnable), m_majorVersion(major), m_minorVersion(minor) {}

	~QApp() {
		if (m_runnable.deinit != nullptr) {
			m_runnable.deinit();
		}

		if (m_window != nullptr) {
			glfwDestroyWindow(m_window);
		}

		if (m_initialized) {
			glfwTerminate();
		}
	}

public:

	int Init();
	int Run();


private:
	int m_majorVersion = 4;
	int m_minorVersion = 3;
	QRunnable m_runnable;
	bool m_initialized = false;
	GLFWwindow* m_window = nullptr;
};

