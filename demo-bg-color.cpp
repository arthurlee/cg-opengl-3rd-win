
#include "QRunnable.h"

void bg_color_display(GLFWwindow* window, double currentTime, double deltaTime) {
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

QRunnable bg_color_runnable() {
	return QRunnable(bg_color_display, "Chapter2 - program1");
}
