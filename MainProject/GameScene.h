#pragma once
#include "stdafx.h"
#include "Scene.h"

#include "Transform3D.h"
#include "Camera.h"
#include "Gravity.h"
#include "PlayerJump.h"
#include "ItemRotate.h"

extern BoundingBox BoundBox[10];

enum {ITEM_PICKAXE = 3, ITEM_SHOE};

class GameScene : public Scene
{

public:
	unsigned int modelLocation;
	unsigned int viewLocation;
	unsigned int projLocation;

public:
	GameScene();
	GameScene(int);
	GameScene(int num_scene, int* index_list, GLuint* tex, GLuint* vao, GLuint* program);
	virtual ~GameScene()
	{
		for (auto object : gameObjects)
			delete object;
		gameObjects.clear();
	}
	
	virtual void update();
	virtual void render();

	GameObject* CreateBox(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateAirBox(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateAirHardBox(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateStar(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateItem_Pickaxe(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateItem_Shoes(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateCannon(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateGrass(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateBook(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateSpike(int* index_list, GLuint* tex, GLuint* vao);
	GameObject* CreateBall(int* index_list, GLuint* tex, GLuint* vao);

	void CreateSkyBox(int* index_list, GLuint* tex, GLuint* vao);



};