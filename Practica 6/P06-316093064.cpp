/*
Semestre 2024-1
Practica 6: Texturizado
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
// para probar el importer
// #include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture llantasTexture;
Texture d8Texture;

Model Coche;
Model Llanta;
Model Cofre;

Skybox skybox;

// Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";

// c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f };

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3 };

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f };

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7 };

	GLfloat vegetacionVertices[] = {
		-0.5f,	-0.5f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,	0.0f,	1.0f,	1.0f,	0.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	0.0f,	0.0f,	0.0f,

		0.0f,	-0.5f,	-0.5f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-0.5f,	0.5f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	0.5f,	0.5f,	1.0f,	1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	0.5f,	-0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0,	1,	2,
		2,	3,	0,

		// back
		8,	9,	10,
		10,	11,	8,

		// left
		12,	13,	14,
		14,	15,	12,
		// bottom
		16,	17,	18,
		18,	19,	16,
		// top
		20,	21,	22,
		22,	23,	20,

		// right
		4,	5,	6,
		6,	7,	4,

	};
	// Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
	GLfloat cubo_vertices[] = {
		// front
		// x	y		z		S		T		NX		NY		NZ
		-0.5f,	-0.5f,	0.5f,	0.26f,	0.34f,	0.0f,	0.0f,	-1.0f, // 0
		0.5f,	-0.5f,	0.5f,	0.49f,	0.34f,	0.0f,	0.0f,	-1.0f, // 1
		0.5f,	0.5f,	0.5f,	0.49f,	0.66f,	0.0f,	0.0f,	-1.0f, // 2
		-0.5f,	0.5f,	0.5f,	0.26f,	0.66f,	0.0f,	0.0f,	-1.0f, // 3
		// right
		// x		y		z		S		T
		0.5f,	-0.5f,	0.5f,	0.74f,	0.34f,	-1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,	0.49f,	0.34f,	-1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,	0.49f,	0.66f,	-1.0f,	0.0f,	0.0f,	
		0.5f,	0.5f,	0.5f,	0.74f,	0.66f,	-1.0f,	0.0f,	0.0f,
		// back
		-0.5f,	-0.5f,	-0.5f,	1.0f,	0.34f,	0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,	0.74f,	0.34f,	0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,	0.74f,	0.66f,	0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,	-0.5f,	1.0f,	0.66f,	0.0f,	0.0f,	1.0f,

		// left
		// x	y		z		S		T
		-0.5f,	-0.5f,	-0.5f,	0.0f,	0.34f,	1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,	0.5f,	0.26f,	0.34f,	1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,	0.26f,	0.66f,	1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,	0.0f,	0.66f,	1.0f,	0.0f,	0.0f,

		// bottom
		// x	y		z		S		T
		-0.5f,	-0.5f,	0.5f,	0.49f,	0.0f,	0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,	0.74f,	0.0f,	0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,	0.74f,	0.34f,	0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,	0.49f,	0.34f,	0.0f,	1.0f,	0.0f,

		// UP
		// x	y		z		S		T
		-0.5f,	0.5f,	0.5f,	0.49f,	0.66f,	0.0f,	-1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,	0.74f,	0.66f,	0.0f,	-1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,	0.74f,	1.0f,	0.0f,	-1.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,	0.49f,	1.0f,	0.0f,	-1.0f,	0.0f,
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);
}

void CrearD8()
{
	unsigned int indices_d8[] = {
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23

	};
	GLfloat vertices_d8[] = {
		//x		y		z		S		T			NX		NY		NZ
		0.5f, -0.5f,  0.0f,		0.25,   1.0f,		0.0f,	0.0f,	-1.0f,
		0.5f,  0.5f,  0.0f,		0.0f,	0.75f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.5f,	0.75f,		0.0f,	0.0f,	-1.0f,

		0.5f,  0.5f,  0.0f,		0.5f,   0.75f,		0.0f,	0.0f,	-1.0f,
	   -0.5f,  0.5f,  0.0f,		0.0f,	0.75f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.25f,	0.5f,		0.0f,	0.0f,	-1.0f,

	   -0.5f,  0.5f,  0.0f,		0.25,   0.5f,		0.0f,	0.0f,	-1.0f,
	   -0.5f, -0.5f,  0.0f,		0.75f,	0.5f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.5f,	0.75f,		0.0f,	0.0f,	-1.0f,

	   -0.5f, -0.5f,  0.0f,		0.5f,   0.75f,		0.0f,	0.0f,	-1.0f,
		0.5f, -0.5f,  0.0f,		0.75f,	0.5f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		1.0f,	0.75f,		0.0f,	0.0f,	-1.0f,

		 0.5f, -0.5f,  0.0f,	0.25f,   0.0f,		0.0f,	0.0f,	-1.0f,
		 0.5f,  0.5f,  0.0f,	0.5f,	0.25f,		0.0f,	0.0f,	-1.0f,
		 0.0f,  0.0f,  -0.6f,	0.0f,	0.25f,		0.0f,	0.0f,	-1.0f,

		 0.5f,  0.5f,  0.0f,	0.0f,   0.25f,		0.0f,	0.0f,	-1.0f,
		 -0.5f,  0.5f,  0.0f,	0.5f,	0.25f,		0.0f,	0.0f,	-1.0f,
		 0.0f,  0.0f,  -0.6f,	0.25f,	0.5f,		0.0f,	0.0f,	-1.0f,

		 -0.5f,  0.5f,  0.0f,	0.25,   0.5f,		0.0f,	0.0f,	-1.0f,
		 -0.5f, -0.5f,  0.0f,	0.5f,	0.25f,		0.0f,	0.0f,	-1.0f,
		 0.0f,  0.0f,  -0.6f,	0.75f,	0.5f,		0.0f,	0.0f,	-1.0f,

		 -0.5f, -0.5f,  0.0f,	0.75f,  0.5f,		0.0f,	0.0f,	-1.0f,
		 0.5f, -0.5f,  0.0f,	0.5f,	0.25f,		0.0f,	0.0f,	-1.0f,
		 0.0f,  0.0f,  -0.6f,	1.0f,	0.25f,		0.0f,	0.0f,	-1.0f

	};

	Mesh* d8 = new Mesh();
	d8->CreateMesh(vertices_d8, indices_d8, 192, 24);
	meshList.push_back(d8);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearD8();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales.tga");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	llantasTexture = Texture("Textures/llantas_con_rin.tga");
	llantasTexture.LoadTextureA();
	d8Texture = Texture("Textures/d8.tga");
	d8Texture.LoadTextureA();

	Coche = Model();
	Coche.LoadModel("Models/coche.obj");
	Llanta = Model();
	Llanta.LoadModel("Models/llanta_con_rin.obj");
	Cofre = Model();
	Cofre.LoadModel("Models/cofre.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();

		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); // color blanco, multiplica a la informacion de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		/*Reporte de practica :
		Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de codigo
		Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y
		texturizar el logo de la Facultad de ingenieria en el cofre de su propio modelo de coche*/

		// Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		d8Texture.UseTexture();
		meshList[4]->RenderMesh();


		// Coche
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f + mainWindow.get_avanzar()));
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coche.RenderModel();

		// Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.95f));
		model = glm::rotate(model, glm::radians(-mainWindow.get_angulo_cofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();

		// Ruedas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.45f, -0.2f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.get_angulo_rueda()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.45f, -0.2f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.get_angulo_rueda()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.45f, -0.2f, -0.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.get_angulo_rueda()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.45f, -0.2f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.get_angulo_rueda()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/