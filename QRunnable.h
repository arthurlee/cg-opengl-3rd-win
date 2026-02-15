#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef int (*Q_INIT_FUNC)(GLFWwindow*);
typedef void (*Q_DEINIT_FUNC)();

typedef void (*Q_DISPLAY_FUNC)(GLFWwindow*, double);

struct QRunnable
{
	QRunnable(
		Q_DISPLAY_FUNC i_display
		, const char* i_title = "CG Example"
		, Q_INIT_FUNC i_init = nullptr
		, Q_DEINIT_FUNC i_deinit = nullptr
		, int i_width = 800
		, int i_height = 600
	): display(i_display), title(i_title), init(i_init), deinit(i_deinit), width(i_width), height(i_height) {}


	int width;
	int height;
	const char* title;
	Q_INIT_FUNC init;
	Q_DEINIT_FUNC deinit;
	Q_DISPLAY_FUNC display;
};