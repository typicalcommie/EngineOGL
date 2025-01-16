#pragma once
#include "glfw/glfw3.h"
#include <glad/glad.h>

typedef unsigned int uint;

struct double2
{
	double x{}, y{};

	void operator =(double2 value)
	{
		x = value.x;
		y = value.y;
	}

	double2 operator +(double2 value)
	{
		return double2{ x + value.x, y + value.y};
	}

	double2 operator -(double2 value)
	{
		return double2{ x - value.x, y - value.y };
	}

	void operator +=(double2 value)
	{
		x += value.x; 
		y += value.y;
	}

	void operator -=(double2 value)
	{
		x -= value.x; 
		y -= value.y;
	}

};

struct int2
{
	int x{}, y{};

	void operator =(int2 value)
	{
		x = value.x;
		y = value.y;
	}

	int2 operator +(int2 value)
	{
		return int2{ x + value.x, y + value.y };
	}

	int2 operator -(int2 value)
	{
		return int2{ x - value.x, y - value.y };
	}

	void operator +=(int2 value)
	{
		x += value.x;
		y += value.y;
	}

	void operator -=(int2 value)
	{
		x -= value.x;
		y -= value.y;
	}

	operator double2()
	{
		return double2{ (double)x, (double)y };
	}
};

namespace Base
{

	static GLFWwindow* window{};
	static unsigned int windowSizeX{800}, windowSizeY{600};
	int2 windowPosition{};
	static double scroll{};
	static bool initialInput = true;
	static bool isWindowInFocus = false;
	static double runTime = 0;
	static double runTimeOld = {};
	static double currentFrameTime{};
	static double2 mousePosition{};
	static double2 mousePositionOld{};
	static double2 mousePositionRelative{};
	static double2 mouseDelta{};

	inline char GetKey(int key)
	{
		return glfwGetKey(window, key);
	}
	
	void ScrollCallback(GLFWwindow* window, double xpos, double ypos)
	{
		scroll += ypos;
	}

	void ResizeCallback(GLFWwindow* window, int x, int y)
	{
		windowSizeX = x; windowSizeY = y;
		glViewport(0, 0, x, y);
	}

	void WindowMoveCallback(GLFWwindow* window, int x, int y)
	{
		windowPosition = { x, y };
	}

	void LockCursor()
	{
		glfwSetInputMode(Base::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void UnlockCursor()
	{
		glfwSetInputMode(Base::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Routine()
	{
		glfwPollEvents();

		//Delta time
		Base::runTimeOld = Base::runTime;
		Base::runTime = glfwGetTime();
		Base::currentFrameTime = Base::runTime - Base::runTimeOld;
		Base::runTimeOld = Base::runTime;

		//Processing mouse input

		if (Base::GetKey(GLFW_KEY_LEFT_ALT))
		{
			Base::UnlockCursor();
		}

		else if (Base::isWindowInFocus)
		{
			Base::LockCursor();
		}

		if (!Base::isWindowInFocus)
		{
			Base::initialInput = true;
			mousePosition = {};
			mousePositionOld = {};
		}
		else
		{
			mousePositionOld = mousePosition;
			glfwGetCursorPos(Base::window, &mousePosition.x, &mousePosition.y);
			if (Base::initialInput)
			{
				mousePositionOld = mousePosition;
				initialInput = false;
			}
		}
		mouseDelta = { mousePosition.x - mousePositionOld.x, mousePositionOld.y - mousePosition.y };
		
		mousePositionRelative = (double2)windowPosition - mousePosition;

		//Window focus processing
		if (glfwGetMouseButton(window, 0) && !Base::isWindowInFocus)
		{
			Base::isWindowInFocus = true;
		}

		else if (glfwGetKey(window, GLFW_KEY_ESCAPE) && Base::isWindowInFocus)
		{
			glfwSetInputMode(Base::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Base::isWindowInFocus = false;
		}
	}

	uint InitGLFW()
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	//Set OpenGL version. First number.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);	//Set OpenGL version. Second number.
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // There is also legacy and ES(embeded systems) profiles. Core profile used for modern desctops.

		window = glfwCreateWindow(600, 400, "Test", 0, 0);

		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			return -2;
		}

		glfwSetFramebufferSizeCallback(window, ResizeCallback);
		glfwSetScrollCallback(Base::window, Base::ScrollCallback);
		glfwSetWindowPosCallback(window, WindowMoveCallback);
		glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

		glfwGetWindowPos(window, &windowPosition.x, &windowPosition.y);
		glfwSetInputMode(Base::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Base::isWindowInFocus = true;
		
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
}