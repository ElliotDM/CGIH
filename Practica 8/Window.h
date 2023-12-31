#pragma once
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmueveCarro() { return mueveCarro; }
	GLfloat getmueveHeli() { return mueveHeli; }
	GLfloat getanguloCofre() { return angulo_cofre; }
	GLboolean getlampara() { return lampara; }
	GLboolean getcalabaza() { return calabaza; }
	GLboolean getLuzDelantera() { return luzDelantera; }
	GLboolean getLuzTrasera() { return luzTrasera; }

	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}
	bool *getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat mueveCarro;
	GLfloat mueveHeli;
	GLfloat angulo_cofre;
	GLboolean lampara;
	GLboolean calabaza;
	GLboolean luzDelantera;
	GLboolean luzTrasera;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow *window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow *window, double xPos, double yPos);
};
