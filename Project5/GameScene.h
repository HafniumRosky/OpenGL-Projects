#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <vector>
#include <sstream>
#include "GameObject.h"
#include "Effect.h"
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "LightHelper.h"
#include "imgui.h"
#include "stb_image_write.h"


extern Mouse mouse;
extern Keyboard keyboard;
//This class is for managing the objects in the scene
class GameScene
{

public:
	GameScene() {}
	virtual ~GameScene() {}
	void add() {}
	void virtual Start() {}
	void virtual InitiScene() {}
	void virtual DisplayScene() {}
	void virtual UpdateScene() {}
	void virtual OnResize(int width, int height) {}
	void virtual GUIDisplay() {}
	void virtual MouseMotion(int x, int y) {}
};

#endif