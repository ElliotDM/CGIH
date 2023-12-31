/*
Semestre 2024-1
Pr�ctica 7: Iluminaci�n 1
*/
//para cargar imagen
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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture logofiTexture;
Texture llantasTexture;
Texture d8Texture;
Texture pumpkinTexture;

Model Camino_M;
Model Blackhawk_M;
Model Coche;
Model Llanta;
Model Cofre;
Model Lamp_M;
Model Pumpkin_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights1[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights1[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
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

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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
		// Parte superior
		0.5f, -0.5f,  0.0f,		0.25,   1.0f,		-1.0f,	0.0f,	-1.0f,
		0.5f,  0.5f,  0.0f,		0.0f,	0.75f,		-1.0f,	0.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.5f,	0.75f,		-1.0f,	0.0f,	-1.0f,

		0.5f,  0.5f,  0.0f,		0.0f,   0.75f,		0.0f,	-1.0f,	-1.0f,
	    -0.5f,  0.5f,  0.0f,	0.25f,	0.5f,		0.0f,	-1.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.5f,	0.75f,		0.0f,	-1.0f,	-1.0f,

	    -0.5f,  0.5f,  0.0f,	0.25,   0.5f,		1.0f,	0.0f,	-1.0f,
	    -0.5f, -0.5f,  0.0f,	0.75f,	0.5f,		1.0f,	0.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.5f,	0.75f,		1.0f,	0.0f,	-1.0f,

	    -0.5f, -0.5f,  0.0f,	0.75f,  0.5f,		0.0f,	1.0f,	-1.0f,
		0.5f, -0.5f,  0.0f,		1.0f,	0.75f,		0.0f,	1.0f,	-1.0f,
		0.0f,  0.0f,  0.6f,		0.5f,	0.75f,		0.0f,	1.0f,	-1.0f,

		// Parte inferior
		0.5f, -0.5f,  0.0f,		0.25f,   0.0f,		-1.0f,	0.0f,	1.0f,
		0.5f,  0.5f,  0.0f,		0.0f,	0.25f,		-1.0f,	0.0f,	1.0f,
		0.0f,  0.0f,  -0.6f,	0.5f,	0.25f,		-1.0f,	0.0f,	1.0f,

		0.5f,  0.5f,  0.0f,		0.0f,   0.25f,		0.0f,	-1.0f,	1.0f,
		-0.5f,  0.5f,  0.0f,	0.25f,	0.5f,		0.0f,	-1.0f,	1.0f,
		0.0f,  0.0f,  -0.6f,	0.5f,	0.25f,		0.0f,	-1.0f,	1.0f,

		-0.5f,  0.5f,  0.0f,	0.25,   0.5f,		1.0f,	0.0f,	1.0f,
		-0.5f, -0.5f,  0.0f,	0.75f,	0.5f,		1.0f,	0.0f,	1.0f,
		0.0f,  0.0f,  -0.6f,	0.5f,	0.25f,		1.0f,	0.0f,	1.0f,

		-0.5f, -0.5f,  0.0f,	0.75f,  0.5f,		0.0f,	1.0f,	1.0f,
		0.5f, -0.5f,  0.0f,		1.0f,	0.25f,		0.0f,	1.0f,	1.0f,
		0.0f,  0.0f,  -0.6f,	0.5f,	0.25f,		0.0f,	1.0f,	1.0f

	};

	Mesh* d8 = new Mesh();
	d8->CreateMesh(vertices_d8, indices_d8, 192, 24);
	meshList.push_back(d8);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
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
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	llantasTexture = Texture("Textures/llantas_con_rin.tga");
	llantasTexture.LoadTextureA();
	d8Texture = Texture("Textures/d8.tga");
	d8Texture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Lamp_M = Model();
	Lamp_M.LoadModel("Models/lamp.obj");
	Coche = Model();
	Coche.LoadModel("Models/coche.obj");
	Llanta = Model();
	Llanta.LoadModel("Models/llanta_con_rin.obj");
	Cofre = Model();
	Cofre.LoadModel("Models/cofre.obj");
	Pumpkin_M = Model();
	Pumpkin_M.LoadModel("Models/Halloween_Pumpkin.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	// PointLight de la lampara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		5.0f, 1.5f, -4.0,
		1.0f, 0.2f, 0.1f);
	pointLightCount++;

	// PointLight de la calabaza
	pointLights[1] = PointLight(1.0f, 0.502f, 0.0f,
		0.0f, 1.0f,
		8.0f, 0.0f, -4.0f,
		1.0f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights1[0] = pointLights[1];
	pointLights1[1] = pointLights[0];

	//contador de luces spotlight
	unsigned int spotLightCount = 0;

	// Linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// SpotLight del helicoptero
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		2.0f, 5.0f, 6.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	// SpotLight delantera del coche
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		2.0f, 0.4f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// SpotLight trasera del coche
	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		-6.0f, 0.4f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights1[0] = spotLights[0]; // Linterna
	spotLights1[1] = spotLights[2]; // Luz delantera
	spotLights1[2] = spotLights[1]; // Helicoptero
	spotLights1[3] = spotLights[3]; // Luz trasera

	spotLights2[0] = spotLights[0]; // Linterna
	spotLights2[1] = spotLights[3]; // Luz trasera
	spotLights2[2] = spotLights[1]; // Helicoptero
	spotLights2[3] = spotLights[2]; // Luz delantera

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Instancia del coche 
		// Carroceria
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f + mainWindow.getmueveCarro()));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coche.RenderModel();

		// Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.95f));
		model = glm::rotate(model, glm::radians(-mainWindow.getanguloCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();

		// Ruedas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.45f, -0.2f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmueveCarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.45f, -0.2f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmueveCarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.45f, -0.2f, -0.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getmueveCarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.45f, -0.2f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getmueveCarro()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		// Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmueveHeli(), 5.0f, 6.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// Luz delantera del coche
		if (mainWindow.getLuzDelantera() == true)
		{
			// Mostrar linterna
			spotLights1[0].SetFlash(lowerLight, camera.getCameraDirection());

			// Mostrar luz delantera
			glm::vec3 carPosition = glm::vec3(6.0f, 0.4f, 0.0f);
			carPosition.x -= mainWindow.getmueveCarro();
			spotLights1[1].SetPos(carPosition);

			// Mostrar luz helicoptero
			glm::vec3 helicopterPosition = glm::vec3(0.0f + mainWindow.getmueveHeli(), 5.0f, 6.0f);
			spotLights1[2].SetPos(helicopterPosition);

			if ((int)now % 2 == 0)
			{
				shaderList[0].SetSpotLights(spotLights1, spotLightCount - 1);
			}
			else
			{
				shaderList[0].SetSpotLights(spotLights1, spotLightCount - 2);
			}
		} 
		// Luz trasera del coche
		else if (mainWindow.getLuzTrasera() == true)
		{
			// Mostrar linterna
			spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());

			// Mostrar luz trasera
			glm::vec3 carPosition = glm::vec3(7.0f, 0.4f, 0.0f);
			carPosition.x -= mainWindow.getmueveCarro();
			spotLights2[1].SetPos(carPosition);

			// Mostrar luz helicoptero
			glm::vec3 helicopterPosition = glm::vec3(0.0f + mainWindow.getmueveHeli(), 5.0f, 6.0f);
			spotLights2[2].SetPos(helicopterPosition);
			
			if ((int)now % 2 == 0)
			{
				shaderList[0].SetSpotLights(spotLights2, spotLightCount - 1);
			}
			else
			{
				shaderList[0].SetSpotLights(spotLights2, spotLightCount - 2);
			}
		}
		else
		{
			if ((int)now % 2 == 0)
			{
				glm::vec3 helicopterPosition = glm::vec3(0.0f + mainWindow.getmueveHeli(), 5.0f, 6.0f);
				spotLights[1].SetPos(helicopterPosition);

				spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
				shaderList[0].SetSpotLights(spotLights, spotLightCount - 2);
				
			}
			else
			{
				glm::vec3 helicopterPosition = glm::vec3(0.0f + mainWindow.getmueveHeli(), 5.0f, 6.0f);
				spotLights[1].SetPos(helicopterPosition);

				spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
				shaderList[0].SetSpotLights(spotLights, spotLightCount - 3);
			}
		}

		// Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp_M.RenderModel();

		// Calabaza
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.0f, -1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pumpkin_M.RenderModel();

		// Lampara 0	Calabaza 0
		if (mainWindow.getlampara() == false and mainWindow.getcalabaza() == false)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}
		// Lampara 0	Calabaza 1
		else if (mainWindow.getlampara() == false and mainWindow.getcalabaza() == true)
		{
			shaderList[0].SetPointLights(pointLights1, pointLightCount - 1);
		}
		// Lampara 1	Calabaza 0
		else if (mainWindow.getlampara() == true and mainWindow.getcalabaza() == false)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}
		// Lampara 1	Calabaza 1
		else if (mainWindow.getlampara() == true and mainWindow.getcalabaza() == true)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}

		// Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 6.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		d8Texture.UseTexture();
		meshList[4]->RenderMesh();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
