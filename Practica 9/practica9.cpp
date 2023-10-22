/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
// para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

// para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

// variables para animación
int cara;
float dado_movx;
float dado_movy;
float dado_movz;
float dado_rotx;
float dado_roty;
float dado_rotz;

bool avanza;
bool vuelta;
bool bajar;
float heli_movx;
float heli_movy;
float heli_movz;
float heli_rotar;
float helice_rotar;
float helice_offset;
float helicoptero_offset;

float rot_offset;
float mov_offset;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

Camera camera;

Model helicoptero_M;
Model helicePrin_M;
Model heliceTras_M;
Model helipuerto_M;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture d8Texture;
Texture helipuertoTexture;

Skybox skybox;

// materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
// para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char *vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char *fShader = "shaders/shader_light.frag";

// función de calculo de normales por promedio de vértices
void calcAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount,
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

		in0 += normalOffset;in1 += normalOffset;in2 += normalOffset;
		vertices[in0] += normal.x;vertices[in0 + 1] += normal.y;vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;vertices[in1 + 1] += normal.y;vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;vertices[in2 + 1] += normal.y;vertices[in2 + 2] += normal.z;
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
	Shader *shader1 = new Shader();
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
	d8Texture = Texture("Textures/d8.tga");
	d8Texture.LoadTextureA();
	helipuertoTexture = Texture("Textures/helipuerto.png");
	helipuertoTexture.LoadTextureA();

	helicoptero_M = Model();
	helicoptero_M.LoadModel("Models/helicoptero.obj");
	helicePrin_M = Model();
	helicePrin_M.LoadModel("Models/helice_principal.obj");
	heliceTras_M = Model();
	heliceTras_M.LoadModel("Models/helice_trasera.obj");
	helipuerto_M = Model();
	helipuerto_M.LoadModel("Models/helipuerto.obj");

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

	// luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
								 0.3f, 0.3f,
								 0.0f, 0.0f, -1.0f);

	// contador de luces puntuales
	unsigned int pointLightCount = 0;

	unsigned int spotLightCount = 0;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		   uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	dado_movx  = -5.0f;
	dado_movy  =  5.0f;
	dado_movz  = -2.0f;
	dado_rotx  =  0.0f;
	dado_roty  =  0.0f;
	dado_rotz  =  0.0f;
	
	avanza = true;
	vuelta = false;
	bajar = false;
	heli_movx = 0.0f;
	heli_movy = -0.7f;
	heli_movz = 6.0f;
	heli_rotar = 0.0f;
	helice_rotar = 0.0f;
	helice_offset = 2.0f;
	helicoptero_offset = 1.0f;

	mov_offset = 0.1f;
	rot_offset = 2.0f;
	

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		// información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		std::srand(std::time(NULL));
		cara = std::rand() % 8 + 1;	// Numero aleatorio en el rango de 1 a 8

		// Animacion del dado
		if (mainWindow.getTirada())
		{
			switch (cara)
			{
			case 1:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;
					dado_rotx += rot_offset * deltaTime / 2;
				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else { }
				}

				break;

			case 2:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;

					if (dado_movy > 3.0f)
					{
						dado_roty -= rot_offset * deltaTime * 4;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotx += rot_offset * deltaTime * 3;
					}
					else if (dado_movy > 1.5f)
					{
						dado_rotz += rot_offset * deltaTime * 4;
					}
					else if (dado_movy > 0.5f)
					{
						dado_rotz += rot_offset * deltaTime * 5;
					}

				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else {}
				}

				break;

			case 3:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;

					if (dado_movy > 3.0f)
					{
						dado_roty -= rot_offset * deltaTime * 2;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotz += rot_offset * deltaTime * 4;
					}
				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else {}
				}

				break;

			case 4:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;

					if (dado_movy > 3.0f)
					{
						dado_roty += rot_offset * deltaTime * 2;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotx += rot_offset * deltaTime * 2;
					}
				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else {}
				}

				break;

			case 5:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;

					if (dado_movy > 3.0f)
					{
						dado_roty -= rot_offset * deltaTime * 4;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotx += rot_offset * deltaTime * 3;
					}
				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else {}
				}

				break;

			case 6:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;

					if (dado_movy > 3.0f)
					{
						dado_rotx += rot_offset * deltaTime * 2;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotz += rot_offset * deltaTime * 4;
					}
					else if (dado_movy > 1.0f)
					{
						dado_rotz += rot_offset * deltaTime * 4;
					}
				}
				else
				{
					Sleep(500);
					mainWindow.setTirada(false);
				}

				break;

			case 7:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;


					if (dado_movy > 3.0f)
					{
						dado_roty += rot_offset * deltaTime * 2;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotx += rot_offset * deltaTime * 4;
					}
					else if (dado_movy > 1.0f)
					{
						dado_rotx += rot_offset * deltaTime * 2;
					}
				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else {}
				}

				break;

			case 8:
				if (dado_movy > -1.0f)
				{
					dado_movy -= mov_offset * deltaTime;

					if (dado_movy > 3.0f)
					{
						dado_roty -= rot_offset * deltaTime * 2;

					}
					else if (dado_movy > 2.0f)
					{
						dado_rotz += rot_offset * deltaTime * 2;
					}
				}
				else
				{
					if ((int)now % 5 == 0) { mainWindow.setTirada(false); }
					else {}
				}

				break;

			default:
				dado_movx = -5.0f;
				dado_movy = 5.0f;
				dado_movz = -2.0f;
				dado_rotx = 0.0f;
				dado_roty = 0.0f;
				dado_rotz = 0.0f;

				mainWindow.setTirada(false);
				break;
			}
		}
		else
		{
			dado_movx = -5.0f;
			dado_movy =  5.0f;
			dado_movz = -2.0f;
			dado_rotx =  0.0f;
			dado_roty =  0.0f;
			dado_rotz =  0.0f;
		}

		// Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(dado_movx, dado_movy, dado_movz));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, dado_rotx * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, dado_roty * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, dado_rotz * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		d8Texture.UseTexture();
		meshList[4]->RenderMesh();

		// Helipuerto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, -1.3f, 6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helipuerto_M.RenderModel();

		// Animacion del helicoptero
		if (avanza)
		{
			helice_rotar += helice_offset * deltaTime;

			if (helice_offset < 15.0f)
			{
				helice_offset += 0.1f;
			}
			else {
				if (not vuelta)
				{
					if (heli_movy <= 7.0f)
					{
						heli_movy += mov_offset * deltaTime / 2;
					}
					else if (heli_movx <= 20.0f) {
						heli_movx += mov_offset * deltaTime / 2;
					}
					else {
						vuelta = true;
					}
				}
				else 
				{
					if (heli_rotar <= 180)
					{
						heli_rotar += helicoptero_offset * deltaTime / 2;
					}
					else if (heli_rotar >= 180)
					{
						if (heli_movx >= 0.0f)
						{
							heli_movx -= mov_offset * deltaTime / 2;
						}
						else 
						{
							bajar = true;
						}
					}
				}
				
				if (bajar)
				{
					if (heli_rotar <= 360)
					{
						heli_rotar += helicoptero_offset * deltaTime / 2;
					}
					else if (heli_rotar >= 360)
					{
						if (heli_movy >= -0.7f)
						{
							heli_movy -= mov_offset * deltaTime / 2;
						}
						else {
							avanza = false;
						}
					}			
				}
			}
		}
		else
		{
			heli_movx = 0.0f;
			heli_movy = -0.7f;
			heli_movz = 6.0f;
			heli_rotar = 0.0f;
			helice_rotar = 0.0f;
			avanza = true;
			vuelta = false;
			bajar = false;
			helice_offset = 2.0f;
			helicoptero_offset = 1.0f;
		}

		// Helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(heli_movx, heli_movy, heli_movz));
		model = glm::rotate(model, heli_rotar * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helicoptero_M.RenderModel();

		// Helice principal
		model = modelaux;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(heli_movx, 0.7f + heli_movy, heli_movz));
		model = glm::rotate(model, helice_rotar * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helicePrin_M.RenderModel();

		// Helice trasera
		model = modelaux;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.1f + heli_movx, 0.7f + heli_movy, 0.1f + heli_movz));
		model = glm::rotate(model, helice_rotar * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		heliceTras_M.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}