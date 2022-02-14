#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "InputLayout.h"
#include "GameObject.h"
#include "GameScene.h"
#include "BasicScene.h"
#include "GIScene.h"
#include "AssignmentScene1.h"
#include "AssignmentScene2.h"
#include "AssignmentScene3.h"
#include "AnimationScene1.h"
#include "AnimationScene2.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "LightHelper.h"
#include "Mesh.h"

//This is for loading optimization
std::vector<Texture> loadedTexture;

//Basic scene
GameScene* basicScene;

//GI scene
GameScene* giScene;

//Assignment scene 1
GameScene* assignmentScene1;

//Assignment scene 2
GameScene* assignmentScene2;

//Assignment scene 3
GameScene* assignmentScene3;

//Animation assignment scene 1
GameScene* animationScene1;

//Animation assignment scene 2
GameScene* animationScene2;

//Mouse
Mouse mouse;
//Keyboard
Keyboard keyboard;

//ImGui variables
//Assignment3
bool useNormalMaps = false;

//ImGui functions
void ImGuiDisplay();

void MouseMotion(int x, int y);
void MouseFunction(int button, int state, int x, int y);
void KeyDown(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void UpdataScene();
void Display();

int main(int argc, char** argv) 
{

	// Set up the window
	glutInit(&argc, argv);
	//MSAA enabled
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("Wavy Navy");

	mouse.initMouse();
	// Tell glut where the display function is
	glutDisplayFunc(Display);
	glutPassiveMotionFunc(MouseMotion);
	glutMotionFunc(MouseMotion);
	glutMouseFunc(MouseFunction);
	glutIdleFunc(UpdataScene);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) 
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	//ImGui initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	glutReshapeFunc(ImGui_ImplGLUT_ReshapeFunc);
	//ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL3_Init();


	//basicScene = new BasicScene();
	//basicScene->Start();
	//basicScene->InitiScene();
	
	//giScene = new GIScene();
	//giScene->Start();
	//giScene->InitiScene();

	//assignmentScene1 = new AssignmentScene1();
	//assignmentScene1->Start();
	//assignmentScene1->InitiScene();

	//assignmentScene2 = new AssignmentScene2();
	//assignmentScene2->Start();
	//assignmentScene2->InitiScene();

	assignmentScene3 = new AssignmentScene3();
	assignmentScene3->Start();
	assignmentScene3->InitiScene();

	//animationScene1 = new AnimationScene1();
	//animationScene1->Start();
	//animationScene1->InitiScene();

	//animationScene2 = new AnimationScene2();
	//animationScene2->Start();
	//animationScene2->InitiScene();

	// Begin infinite event loop
	glutMainLoop();

	//Destroy ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

void MouseFunction(int button, int state, int x, int y)
{
	mouse.MouseFunction(button, state, x, y);
}

void MouseMotion(int x, int y)
{
	mouse.MouseMotion(x, y);
	glutPostRedisplay();
}

void KeyDown(unsigned char key, int x, int y)
{
	keyboard.KeyDown(key, x, y);
	if (keyboard.keyState.ESC == true)
		exit(1);
}

void KeyUp(unsigned char key, int x, int y)
{
	keyboard.KeyUp(key, x, y);
}

void UpdataScene()
{
	//basicScene->UpdateScene();
	//giScene->UpdateScene();
	//assignmentScene1->UpdateScene();
	//assignmentScene2->UpdateScene();
	assignmentScene3->UpdateScene();
	//animationScene1->UpdateScene();
	//animationScene2->UpdateScene();
}


void Display()
{
	//ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	ImGuiDisplay();

	ImGui::Render();


	//basicScene->DisplayScene();
	//giScene->DisplayScene();
	//assignmentScene1->DisplayScene();
	//assignmentScene2->DisplayScene();
	assignmentScene3->DisplayScene();
	//animationScene1->DisplayScene();
	//animationScene2->DisplayScene();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glutSwapBuffers();
	glutPostRedisplay();

}

void ImGuiDisplay()
{
	ImGui::Begin("Map Manager");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Checkbox("Normal Map", &useNormalMaps);      // Edit bools storing our window open/close state
	ImGui::End();
}