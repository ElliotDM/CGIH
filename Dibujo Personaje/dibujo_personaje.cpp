#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
// glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
// clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

// Dimensiones de la ventana
Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<MeshColor *> meshColorList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char *vShader = "shaders/shaderpersonaje.vert";
static const char *fShader = "shaders/shaderpersonaje.frag";
static const char *vShaderColor = "shaders/shadercolor.vert";
static const char *fShaderColor = "shaders/shadercolor.frag";

void DibujarPersonaje()
{
	GLfloat vertices[] = {
		//X     Y       Z       R       G       B
		0.48f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //C
		0.50f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //D
		0.46f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //E

		0.50f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //D
		0.52f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //F
		0.46f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //E

		0.52f,  0.39f,  0.0f,   0.8f,   0.0f,   0.0f,    //F
		0.53f,  0.40f,  0.0f,   0.8f,   0.0f,   0.0f,    //G
		0.46f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //E

		0.46f,	0.39f,	0.0f,   0.8f,   0.0f,   0.0f,    //E
		0.53f,  0.40f,  0.0f,   0.8f,   0.0f,   0.0f,    //G
		0.45f,  0.40f,  0.0f,   0.8f,   0.0f,   0.0f,    //H

		0.45f,  0.40f,  0.0f,   0.8f,   0.0f,   0.0f,    //H
		0.53f,  0.40f,  0.0f,   0.8f,   0.0f,   0.0f,    //G
		0.45f,  0.41f,  0.0f,   0.8f,   0.0f,   0.0f,    //I

		0.45f,  0.41f,  0.0f,   0.8f,   0.0f,   0.0f,    //I
		0.53f,  0.40f,  0.0f,   0.8f,   0.0f,   0.0f,    //G
		0.53f,  0.41f,  0.0f,   0.8f,   0.0f,   0.0f,    //J

		0.45f,  0.41f,  0.0f,   0.8f,   0.0f,   0.0f,    //I
		0.53f,  0.41f,  0.0f,   0.8f,   0.0f,   0.0f,    //J
		0.53f,  0.42f,  0.0f,   0.8f,   0.0f,   0.0f,    //K

		0.45f,  0.41f,  0.0f,   0.8f,   0.0f,   0.0f,    //I
		0.53f,  0.42f,  0.0f,   0.8f,   0.0f,   0.0f,    //K
		0.45f,  0.42f,  0.0f,   0.8f,   0.0f,   0.0f,    //L

		0.45f,  0.42f,  0.0f,   0.8f,   0.0f,   0.0f,    //L
		0.53f,  0.42f,  0.0f,   0.8f,   0.0f,   0.0f,    //K
		0.46f,  0.43f,  0.0f,   0.8f,   0.0f,   0.0f,    //M

		0.46f,  0.43f,  0.0f,   0.8f,   0.0f,   0.0f,    //M
		0.53f,  0.42f,  0.0f,   0.8f,   0.0f,   0.0f,    //K
		0.52f,  0.43f,  0.0f,   0.8f,   0.0f,   0.0f,    //N

		0.46f,  0.43f,  0.0f,   0.8f,   0.0f,   0.0f,    //M
		0.52f,  0.43f,  0.0f,   0.8f,   0.0f,   0.0f,    //N
		0.51f,  0.44f,  0.0f,   0.8f,   0.0f,   0.0f,    //O

		0.46f,  0.43f,  0.0f,   0.8f,   0.0f,   0.0f,    //M
		0.51f,  0.44f,  0.0f,   0.8f,   0.0f,   0.0f,    //O
		0.47f,  0.44f,  0.0f,   0.8f,   0.0f,   0.0f,    //P

		0.47f,  0.44f,  0.0f,   0.8f,   0.0f,   0.0f,    //P
		0.51f,  0.44f,  0.0f,   0.8f,   0.0f,   0.0f,    //O
		0.49f,  0.44f,  0.0f,   0.8f,   0.0f,   0.0f,    //Q

		0.36f,  0.41f,  0.0f,   0.0f,   0.0f,   0.0f,    //R
		0.38f,  0.41f,  0.0f,   0.0f,   0.0f,   0.0f,    //S
		0.41f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //T

		0.36f,  0.41f,  0.0f,   0.0f,   0.0f,   0.0f,    //R
		0.41f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //T
		0.34f,  0.41f,  0.0f,   0.0f,   0.0f,   0.0f,    //U

		0.34f,  0.41f,  0.0f,   0.0f,   0.0f,   0.0f,    //U
		0.41f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //T
		0.33f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //V

		0.33f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //V
		0.41f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //T
		0.42f,  0.43f,  0.0f,   0.0f,   0.0f,   0.0f,    //W

		0.33f,  0.42f,  0.0f,   0.0f,   0.0f,   0.0f,    //V
		0.42f,  0.43f,  0.0f,   0.0f,   0.0f,   0.0f,    //W
		0.32f,  0.43f,  0.0f,   0.0f,   0.0f,   0.0f,    //Z

		0.32f,  0.43f,  0.0f,   0.0f,   0.0f,   0.0f,    //Z
		0.42f,  0.43f,  0.0f,   0.0f,   0.0f,   0.0f,    //W
		0.43f,  0.45f,  0.0f,   0.0f,   0.0f,   0.0f,    //A1

		0.32f,  0.43f,  0.0f,   0.0f,   0.0f,   0.0f,    //Z
		0.43f,  0.45f,  0.0f,   0.0f,   0.0f,   0.0f,    //A1
		0.31f,  0.45f,  0.0f,   0.0f,   0.0f,   0.0f,    //B1

		0.31f,  0.45f,  0.0f,   0.0f,   0.0f,   0.0f,    //B1
		0.43f,  0.45f,  0.0f,   0.0f,   0.0f,   0.0f,    //A1
		0.43f,  0.47f,  0.0f,   0.0f,   0.0f,   0.0f,    //C1

		0.31f,  0.45f,  0.0f,   0.0f,   0.0f,   0.0f,    //B1
		0.43f,  0.47f,  0.0f,   0.0f,   0.0f,   0.0f,    //C1
		0.31f,  0.47f,  0.0f,   0.0f,   0.0f,   0.0f,    //D1

		0.31f,  0.47f,  0.0f,   0.0f,   0.0f,   0.0f,    //D1
		0.43f,  0.47f,  0.0f,   0.0f,   0.0f,   0.0f,    //C1 
		0.32f,  0.48f,  0.0f,   0.0f,   0.0f,   0.0f,    //E1

		0.32f,  0.48f,  0.0f,   0.0f,   0.0f,   0.0f,    //E1
		0.43f,  0.47f,  0.0f,   0.0f,   0.0f,   0.0f,    //C1    
		0.42f,  0.49f,  0.0f,   0.0f,   0.0f,   0.0f,    //F1    

		0.32f,  0.48f,  0.0f,   0.0f,   0.0f,   0.0f,    //E1
		0.42f,  0.49f,  0.0f,   0.0f,   0.0f,   0.0f,    //F1  
		0.33f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //G1  

		0.33f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //G1  
		0.42f,  0.49f,  0.0f,   0.0f,   0.0f,   0.0f,    //F1  
		0.41f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //H1  

		0.33f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //G1 
		0.41f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //H1  
		0.35f,  0.51f,  0.0f,   0.0f,   0.0f,   0.0f,    //I1 

		0.35f,  0.51f,  0.0f,   0.0f,   0.0f,   0.0f,    //I1 
		0.41f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //H1  
		0.36f,  0.51f,  0.0f,   0.0f,   0.0f,   0.0f,    //J1  

		0.36f,  0.51f,  0.0f,   0.0f,   0.0f,   0.0f,    //J1  
		0.41f,  0.50f,  0.0f,   0.0f,   0.0f,   0.0f,    //H1  
		0.39f,  0.51f,  0.0f,   0.0f,   0.0f,   0.0f,    //K1  

	};

	MeshColor *dibujo = new MeshColor();
	dibujo->CreateMeshColor(vertices, 522);
	meshColorList.push_back(dibujo);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(512, 512);
	mainWindow.Initialise();
	DibujarPersonaje();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 model(1.0);

	while (!mainWindow.getShouldClose())
	{
		// Recibir eventos del usuario
		glfwPollEvents();

		// Limpiar la ventana
		glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Se agrega limpiar el buffer de profundidad

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();

		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.1f, 0.1f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
