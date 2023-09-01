// Práctica 2 semestre 2024-1: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
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
const float toRadians = 3.14159265f / 180.0; // grados a radianes
Window mainWindow;

std::vector<Mesh *> meshList;
std::vector<MeshColor *> meshColorList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char *vShader = "shaders/shader.vert";
static const char *fShader = "shaders/shader.frag";
static const char *vShaderColor = "shaders/shadercolor.vert";
static const char *fShaderColor = "shaders/shadercolor.frag";
static const char *vShaderRed = "shaders/shaderRed.vert";
static const char *vShaderGreen = "shaders/shaderGreen.vert";
static const char *vShaderBlue = "shaders/shaderBlue.vert";
static const char *vShaderDarkGreen = "shaders/shaderDarkGreen.vert";
static const char *vShaderBrown = "shaders/shaderBrown.vert";

float angulo = 0.0f;
// color café: 0.478, 0.255, 0.067

// Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0, 1, 2,
		1, 3, 2,
		3, 0, 2,
		1, 0, 3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // 0
		0.5f, -0.5f, 0.0f,	// 1
		0.0f, 0.5f, -0.25f, // 2
		0.0f, -0.5f, -0.5f, // 3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
// Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f};
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetraE()
{
	unsigned int indices[] = {
		0, 2, 3,
		0, 1, 2,
		2, 4, 6,
		4, 5, 6,
		7, 9, 10,
		7, 8, 9,
		1, 12, 13,
		1, 11, 12};

	GLfloat vertices[] = {
		// X			Y			Z
		-1.0f, 0.2f, 0.0f, // 0
		-0.8f, 0.2f, 0.0f, // 1
		-0.8f, 1.0f, 0.0f, // 2
		-1.0f, 1.0f, 0.0f, // 3
		-0.8f, 0.8f, 0.0f, // 4
		-0.2f, 0.8f, 0.0f, // 5
		-0.2f, 1.0f, 0.0f, // 6
		-0.8f, 0.5f, 0.0f, // 7
		-0.6f, 0.5f, 0.0f, // 8
		-0.6f, 0.7f, 0.0f, // 9
		-0.8f, 0.7f, 0.0f, // 10
		-0.2f, 0.2f, 0.0f, // 11
		-0.2f, 0.4f, 0.0f, // 12
		-0.8f, 0.4f, 0.0f, // 13

	};

	Mesh *letraE = new Mesh();
	letraE->CreateMesh(vertices, indices, 42, 24);
	meshList.push_back(letraE);
}

void CrearLetraD()
{
	unsigned int indices[] = {
		0, 2, 3,
		0, 1, 2,
		2, 4, 6,
		4, 5, 6,
		1, 8, 9,
		1, 7, 8,
		5, 11, 8,
		8, 11, 10,
		5, 11, 6,
		7, 10, 8};

	GLfloat vertices[] = {
		// X			Y			Z
		-1.0f, -1.0f, 0.0f, // 0
		-0.8f, -1.0f, 0.0f, // 1
		-0.8f, -0.2f, 0.0f, // 2
		-1.0f, -0.2f, 0.0f, // 3
		-0.8f, -0.4f, 0.0f, // 4
		-0.4f, -0.4f, 0.0f, // 5
		-0.4f, -0.2f, 0.0f, // 6
		-0.4f, -1.0f, 0.0f, // 7
		-0.4f, -0.8f, 0.0f, // 8
		-0.8f, -0.8f, 0.0f, // 9
		-0.2f, -0.8f, 0.0f, // 10
		-0.2f, -0.4f, 0.0f, // 11

	};

	Mesh *letraD = new Mesh();
	letraD->CreateMesh(vertices, indices, 36, 30);
	meshList.push_back(letraD);
}

void CrearLetraM()
{
	unsigned int indices[] = {
		0, 2, 3,
		0, 1, 2,
		4, 6, 7,
		4, 5, 6,
		2, 8, 9,
		8, 9, 11,
		9, 7, 10,
		9, 10, 11};

	GLfloat vertices[] = {
		// X			Y			Z
		0.2f, -1.0f, 0.0f, // 0
		0.4f, -1.0f, 0.0f, // 1
		0.4f, -0.2f, 0.0f, // 2
		0.2f, -0.2f, 0.0f, // 3
		0.8f, -1.0f, 0.0f, // 4
		1.0f, -1.0f, 0.0f, // 5
		1.0f, -0.2f, 0.0f, // 6
		0.8f, -0.2f, 0.0f, // 7
		0.4f, -0.4f, 0.0f, // 8
		0.6f, -0.4f, 0.0f, // 9
		0.8f, -0.4f, 0.0f, // 10
		0.6f, -0.6f, 0.0f, // 11

	};

	Mesh *letraM = new Mesh();
	letraM->CreateMesh(vertices, indices, 36, 24);
	meshList.push_back(letraM);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		// X	Y		Z		R		G		B
		-1.0f,	-1.0f,	0.5f,	0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,	0.5f,	0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,	0.5f,	0.0f,	0.0f,	1.0f,
		/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/

	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 18);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		// X		Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		1.0f,	0.0f,	0.0f,
		1.0f,		-1.0f,		0.5f,		1.0f,	0.0f,	0.0f,
		0.0f,		1.0f,		0.5f,		1.0f,	0.0f,	0.0f,
	};

	MeshColor *triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_trianguloazul[] = {
		// X		Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		0.0f,	0.0f,	1.0f,
		1.0f,		-1.0f,		0.5f,		0.0f,	0.0f,	1.0f,
		0.0f,		1.0f,		0.5f,		0.0f,	0.0f,	1.0f,
	};

	MeshColor *trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_trianguloverde[] = {
		// X		Y			Z			R		G		B
		-1.0f,		-1.0f,		0.5f,		0.0f,	1.0f,	0.0f,
		1.0f,		-1.0f,		0.5f,		0.0f,	1.0f,	0.0f,
		0.0f,		1.0f,		0.5f,		0.0f,	1.0f,	0.0f,
	};

	MeshColor *trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradoverde[] = {
		// X		Y			Z			R		G		B
		-0.5f,		-0.5f,		0.5f,		0.0f,	0.5f,	0.0f,
		0.5f,		-0.5f,		0.5f,		0.0f,	0.5f,	0.0f,
		0.5f,		0.5f,		0.5f,		0.0f,	0.5f,	0.0f,
		-0.5f,		-0.5f,		0.5f,		0.0f,	0.5f,	0.0f,
		0.5f,		0.5f,		0.5f,		0.0f,	0.5f,	0.0f,
		-0.5f,		0.5f,		0.5f,		0.0f,	0.5f,	0.0f,
	};

	MeshColor *cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradorojo[] = {
		// X		Y			Z			R		G		B
		-0.5f,		-0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		0.5f,		-0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		0.5f,		0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		-0.5f,		-0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		0.5f,		0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
		-0.5f,		0.5f,		0.5f,		1.0f,	0.0f,	0.0f,
	};

	MeshColor *cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	GLfloat vertices_cuadradocafe[] = {
		// X		Y			Z			R			G			B
		-0.5f,		-0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		0.5f,		-0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		0.5f,		0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		-0.5f,		-0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		0.5f,		0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
		-0.5f,		0.5f,		0.5f,		0.478f,		0.255f,		0.067f,
	};

	MeshColor *cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);
}

void CreateShaders()
{

	Shader *shader1 = new Shader(); // shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader(); // shader para usar color como parte del VAO: letras
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader *shader3 = new Shader();
	shader3->CreateFromFiles(vShaderRed, fShader);
	shaderList.push_back(*shader3);

	Shader *shader4 = new Shader();
	shader4->CreateFromFiles(vShaderGreen, fShader);
	shaderList.push_back(*shader4);

	Shader *shader5 = new Shader();
	shader5->CreateFromFiles(vShaderBlue, fShader);
	shaderList.push_back(*shader5);

	Shader *shader6 = new Shader();
	shader6->CreateFromFiles(vShaderDarkGreen, fShader);
	shaderList.push_back(*shader6);

	Shader *shader7 = new Shader();
	shader7->CreateFromFiles(vShaderBrown, fShader);
	shaderList.push_back(*shader7);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); // índice 0 en MeshList
	CrearCubo();	// índice 1 en MeshList
	// CrearLetraE();
	// CrearLetraD();
	// CrearLetraM();
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	// glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
	glm::mat4 model(1.0);

	// Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		// Recibir eventos del usuario
		glfwPollEvents();
		// Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Se agrega limpiar el buffer de profundidad

		// 1. Dibujar letras
		
		// shaderList[2].useShader();
		// uniformModel = shaderList[2].getModelLocation();
		// uniformProjection = shaderList[2].getProjectLocation();

		// model = glm::mat4(1.0);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		// glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// meshList[2]->RenderMesh();

		// shaderList[3].useShader();
		// uniformModel = shaderList[3].getModelLocation();
		// uniformProjection = shaderList[3].getProjectLocation();

		// model = glm::mat4(1.0);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		// glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		// glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// meshList[3]->RenderMesh();

		// shaderList[4].useShader();
		// uniformModel = shaderList[4].getModelLocation();
		// uniformProjection = shaderList[4].getProjectLocation();

		// model = glm::mat4(1.0);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		// glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		// glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// meshList[4]->RenderMesh();

		// 2. Dibujar casa

		//Cubos cafes
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, -0.45f, -4.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.78f, -0.45f, -4.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramides verdes
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, -0.13f, -4.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.78f, -0.13f, -4.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Cubo rojo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramide azul
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Cubos verdes
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.24f, 0.19f, -3.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.24f, 0.19f, -3.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.01f, -0.35f, -3.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
// Traslación
// model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
// model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
// model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/

// Matar proceso
// taskkill /F /IM Project2.exe