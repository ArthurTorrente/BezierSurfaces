#include "BSpline.h"
#include "BezierSurface.h"

#define GLEW_STATIC 1
#include "GL\glew.h"

#ifdef _WIN32
#	include <Windows.h>
#	define FREEGLUT_LIB_PRAGMAS 0
#	pragma comment(lib, "freeglut.lib")
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glew32s.lib")
#endif

#include "GL/freeglut.h"

#include "Materials/ShaderLoader.h"
#include "Render/Renderer.h"
#include "Cameras/PerspectiveCamera.h"

#include <time.h>

static const float g_cubeVertices[] = {
	-1.f, -1.f, 1.0f,		// 0
	1.f, -1.f, 1.0f,  		// 1	
	1.f, 1.f, 1.0f,			// 2
	-1.f, 1.f, 1.0f,		// 3	
	-1.f, -1.f, -1.0f,		// 4
	1.f, -1.f, -1.0f,		// 5
	1.f, 1.f, -1.0f,		// 6
	-1.f, 1.f, -1.0f		// 7
};

static const unsigned int g_cubeIndices[] = {
	0, 1, 2, // avant
	2, 3, 0,
	3, 2, 6, // haut
	6, 7, 3,
	7, 6, 5, // arriere
	5, 4, 7,
	1, 5, 6, // droite
	6, 2, 1,
	4, 0, 3, // gauche
	3, 7, 4,
	4, 5, 1, // bas
	1, 0, 4
};

Renderer r;
Scene s;
PerspectiveCamera c;

std::vector<Object3D*> objects;
std::vector<float> randRot;

void display()
{	
	for (uint i = 0; i < objects.size(); ++i)
		objects[i]->rotateY(0.001);

	r.render(s, c);

	if (r.autoUpdate())
		glutPostRedisplay();
}

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Test B-Spline");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		// Erreur
	}

	r.setClearColor(Vector3(0.0, 1.0, 1.0));
	r.setViewport(800, 600);
	r.setAutoUpdate(true);

	c.setPosition(Vector3(0, 0, -30));

	ShaderLoader::loadShader("Simple", "simple.vs", "simple.fs");
	MaterialSPtr mat(new BasicMaterial("Simple", Vector3(1.0, 0.0, 0.0)));
	Geometry geo = Geometry(
		std::vector<float>(g_cubeVertices, g_cubeVertices + sizeof(g_cubeVertices) / sizeof(float)), 
		std::vector<int>(g_cubeIndices, g_cubeIndices + sizeof(g_cubeIndices) / sizeof(int))		
	);
	MeshSPtr mesh(new Mesh(geo, mat));

	for (uint i = 0; i < 50; ++i)
	{
		Object3D* obj = new Object3D();
		obj->setPosition(Vector3(
			-10.0f + static_cast <float> (rand() % 20),
			-10.0f + static_cast <float> (rand() % 20),
			0
		));
		obj->setMesh(mesh);
		objects.push_back(obj);
		// randRot.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		s.add(obj);
	}

	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}