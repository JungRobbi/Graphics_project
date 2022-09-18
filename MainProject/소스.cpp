#define  _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION


#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <cctype>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include "stdafx.h"

#include "Scene.h"

#include "stb_image.h"
#include "shader.h"
#include "objRead.h"

#include "Mmsystem.h"
#include "Digitalv.h"

#include "VAO.h"
#include "GameScene.h"
#include "PlayerJump.h"

#include "Input.h"
#include "DestroyEffect.h"

MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;

using namespace std;

uniform_real_distribution<float> uid(-3.5, 3.5);

GLuint g_window_w = 800;
GLuint g_window_h = 800;

GLuint VAO[100];
GLuint VBO_position[100];
GLuint VBO_normal[100];
GLuint VBO_uv[100];

int polygon_mode = 2;

void keyboard(unsigned char, int, int);
void keyboard2(unsigned char key2, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Motion2(int x, int y);
void TimerFunction(int value);
void Display();
void Display_Sub1();
void Reshape(int w, int h);
void InitBuffer();
void InitTexture();
void TimerFunction(int value);

void InitBuffer_bind(const int);


// obj 읽기 변수

int loadObj(const char* filename);
int loadObj_normalize_center_3f(const char* filename);
int loadObj_normalize_center_4f(const char* filename);
float* sphere_object;
int num_shape_list[10];
int num_vertices = 3;


float sunSize;
int shape = 1;					// 불러올 모양 (1. 육면체, 2. 구)

ObjRoad obj;

// 텍스쳐 변수

int img = 7;
GLuint texture[30];
int n_model = 0;
int n_max_model = 4;
int widthImage, heightImage, numberOfChannel = 0;

// 게임 변수
int num_ob = 10;

int game = 0;					// 게임 state

int intmpx = 0;
int intmpy = 0;

float mousex = 0;				// 마우스 x
float mousey = 0;				// 마우스 y

float create_height = 5.0f;

float f_Light_ambients[3];

BoundingBox BoundBox[10];


glm::mat4 TR = glm::mat4(1.0f);

bool key[256];
float msx, msy = 0;
bool start = false;

std::vector<Scene*> sc;

void SceneChange(int num_scene);
int main(int argc, char** argv)
{
	// create window using freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Computer Graphics");

	//////////////////////////////////////////////////////////////////////////////////////
	//// initialize GLEW
	//////////////////////////////////////////////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "GLEW OK\n";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//// Create shader program an register the shader
	//////////////////////////////////////////////////////////////////////////////////////

	GLuint vShader[4];
	GLuint fShader[4];

	vShader[0] = MakeVertexShader("vertex.glvs", 0);
	fShader[0] = MakeFragmentShader("fragment.glfs", 0);

	// shader Program
	s_program[0] = glCreateProgram();
	glAttachShader(s_program[0], vShader[0]);
	glAttachShader(s_program[0], fShader[0]);
	glLinkProgram(s_program[0]);
	checkCompileErrors(s_program[0], "PROGRAM");

	{ // initialize
		for (int i{}; i < 3; ++i)
			f_Light_ambients[i] = 0.3f;

		InitBuffer();
		InitTexture();
		for (int i{}; i < num_ob; ++i) {
			InitBuffer_bind(i);
		}

		glEnable(GL_DEPTH_TEST);
		mciOpen.lpstrElementName = "Resource/bgm.mp3";
		mciOpen.lpstrDeviceType = "Resource/mpegvideo";

		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
			(DWORD)(LPVOID)&mciOpen);

		dwID = mciOpen.wDeviceID;

		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);
	}

	sc.emplace_back(new GameScene(1, num_shape_list, texture, VAO, s_program));


	// callback functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	glutMouseFunc(Mouse);
	//glutMotionFunc(Motion);

	glutPassiveMotionFunc(Motion2);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard2);
	glutTimerFunc(10, TimerFunction, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// freeglut 윈도우 이벤트 처리 시작. 윈도우가 닫힐때까지 후한루프 실행.
	glutMainLoop();

	return 0;
}


void InitBuffer()
{
	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(10, VAO);
	glGenBuffers(10, VBO_position);
	glGenBuffers(10, VBO_normal);
	glGenBuffers(10, VBO_uv);
}

void InitBuffer_bind(const int street) {
	BoundingBox* bb = nullptr;
	if (street == 0) {
		num_shape_list[Cube] = obj.loadObj_normalize_center_3f("Resource/cube.obj");
		bb = &BoundBox[Cube];
	}
	else if (street == 1) {
		num_shape_list[Star] = obj.loadObj_normalize_center_3f("Resource/Star.obj");
		bb = &BoundBox[Star];
	}
	else if (street == 2) {
		num_shape_list[Plane] = obj.loadObj_normalize_center_3f("Resource/plane.obj");
		bb = &BoundBox[Plane];
	}
	else if (street == 3) {
		num_shape_list[Pickaxe] = obj.loadObj_normalize_center_4f("Resource/Pickaxe.obj");
		bb = &BoundBox[Pickaxe];
	}
	else if (street == 4) {
		num_shape_list[Grass] = obj.loadObj_normalize_center_4f("Resource/grass.obj");
		bb = &BoundBox[Grass];
	}
	else if (street == 5) {
		num_shape_list[Shoes] = obj.loadObj_normalize_center_4f("Resource/Shoe.obj");
		bb = &BoundBox[Shoes];
	}
	else if (street == 6) {
		num_shape_list[Cannon] = obj.loadObj_normalize_center_4f("Resource/Cannon.obj");
		bb = &BoundBox[Cannon];
	}
	else if (street == 7) {
		num_shape_list[Ball] = obj.loadObj_normalize_center_3f("Resource/sphere.obj");
		bb = &BoundBox[Ball];
	}
	else if (street == 8) {
		num_shape_list[Book] = obj.loadObj_normalize_center_4f("Resource/Book.obj");
		bb = &BoundBox[Book];
	}
	else if (street == 9) {
		num_shape_list[Spike] = obj.loadObj_normalize_center_4f("Resource/spike.obj");
		bb = &BoundBox[Spike];
	}

	if (bb) {
		bb->maxX = obj.maxX;
		bb->minX = obj.minX;
		bb->maxY = obj.maxY;
		bb->minY = obj.minY;
		bb->maxZ = obj.maxZ;
		bb->minZ = obj.minZ;
	}

	glUseProgram(s_program[0]);
	glBindVertexArray(VAO[street]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[street]);
	glBufferData(GL_ARRAY_BUFFER, obj.outvertex.size() * sizeof(glm::vec3), &obj.outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[street]);
	glBufferData(GL_ARRAY_BUFFER, obj.outnormal.size() * sizeof(glm::vec3), &obj.outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_uv[street]);
	glBufferData(GL_ARRAY_BUFFER, obj.outuv.size() * sizeof(glm::vec2), &obj.outuv[0], GL_STATIC_DRAW);
	GLint tAttribute = glGetAttribLocation(s_program[0], "aTex");
	glVertexAttribPointer(tAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(tAttribute);

	obj.outvertex = std::vector< glm::vec3 >(0.0f);  // 다음 obj 불러오기 위한 초기화
	obj.outnormal = std::vector< glm::vec3 >(0.0f);
	obj.outuv = std::vector< glm::vec2 >(0.0f);

	obj.vertexIndices = std::vector< unsigned int >(0.0f);
	obj.uvIndices = std::vector< unsigned int >(0.0f);
	obj.normalIndices = std::vector< unsigned int >(0.0f);
	obj.temp_vertices = std::vector< glm::vec3 >(0.0f);
	obj.temp_uvs = std::vector< glm::vec2 >(0.0f);
	obj.temp_normals = std::vector< glm::vec3 >(0.0f);
}


void InitTexture()
{	
	BITMAPINFO* bmp;
	string map[30] = { "Resource/main.png","Resource/B.png","Resource/gold.png","Resource/123.png","Resource/321.png","Resource/grass.png",
		"Resource/face.png","Resource/gun_tex.png", "Resource/skybox2_top.png", "Resource/skybox2_left.png", "Resource/skybox2_front.png", "Resource/skybox2_right.png",
		"Resource/skybox2_back.png", "Resource/skybox2_bottom.png", "Resource/vinus.png", "Resource/mars.png", "Resource/jupiter.png"};
																							// ↑ 여기가 14
	glGenTextures(30, texture); //--- 텍스처 생성

	for (int i = 0; i < 30; ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]); //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(map[i].c_str(), &widthImage, &heightImage, &numberOfChannel, 0);//--- 텍스처로 사용할 비트맵 이미지 로드하기
		glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의
		stbi_image_free(data);
	}

	glUseProgram(s_program[0]);
	int tLocation = glGetUniformLocation(s_program[0], "outTex"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
}

void Display()
{
	//*************************************************************************
	// 출력 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (Scene::scene->p_player->GetComponent<Camera>()->state != FIRST_VIEW)
		ShowCursor(true);
	else {
		ShowCursor(false);
	}
	ShowCursor(false);

	//*************************************************************************
	// 카메라 설정
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "model");
	//*************************************************************************
	// 조명 설정


	int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 10.0, 0.0);
	int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	int lighAmbientLocation = glGetUniformLocation(s_program[0], "lightAmbient"); //--- lightAmbient 
	glUniform3f(lighAmbientLocation, f_Light_ambients[0], f_Light_ambients[1], f_Light_ambients[2]);

	//*************************************************************************
	// 그리기 부분

	glUseProgram(s_program[0]);

	glViewport(0, 0, WINDOWX, WINDOWY);

	if (!sc.empty()) {
		Scene::scene->update();
		Scene::scene->render();
	}

	if (Scene::scene->p_player->GetComponent<Camera>()->state == TOP_VIEW) {
		if (n_model == Cube) {
			glBindVertexArray(VAO[Cube]);
			TR = glm::mat4(1.0f);
			TR = glm::translate(TR, glm::vec3(msx * 8.0f, create_height, -msy * 8.0f));
			TR = glm::scale(TR, glm::vec3(0.4f,0.4f,0.4f));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			glDrawArrays(GL_TRIANGLES, 0, num_shape_list[Cube]);
		}
	}

	/*glBindVertexArray(VAO[3]);
	TR = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawArrays(GL_TRIANGLES, 0, num_shape_list[3]);*/


	if (f_Light_ambients[0] < 0.3f) { 
		Display_Sub1();
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND); // 블렌딩 해제

	glutSwapBuffers();
}

void Display_Sub1()
{
	// 카메라 설정
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "model");
	unsigned int viewLocation = glGetUniformLocation(s_program[0], "view");
	unsigned int projLocation = glGetUniformLocation(s_program[0], "projection");

	glm::mat4 Vw = glm::mat4(1.0f);
	glm::mat4 Pj = glm::mat4(1.0f);
	Vw = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.0005f, 100.0f);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);

	//*************************************************************************
	// 조명 설정
	int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 0.0, 0.0);
	int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	//*************************************************************************
	// 그리기 부분
	glViewport(WINDOWX / 4, WINDOWY / 4, WINDOWX / 2, WINDOWY / 2);

	glBindVertexArray(VAO[Plane]);
	TR = glm::mat4(1.0f);
	TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, -4.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glDrawArrays(GL_TRIANGLES, 0, num_shape_list[Plane]);
}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && Scene::scene->p_player->GetComponent<Camera>()->state == TOP_VIEW) {
		if (n_model == Cube) {
			auto box = Scene::scene->CreateBox(num_shape_list, texture, VAO);
			box->AddComponent<DestroyEffect>();

			box->GetComponent<Transform3D>()->position = glm::vec3(msx * 8.0f, create_height, -msy * 8.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
			box->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);
			box->GetComponent<Collide>()->BoundBox.pos = box->GetComponent<Transform3D>()->position;
		}
		else if (n_model == Star) {
			/*auto star = CreateStar(index_list, tex, vao);

			star->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 5.5f, 0.0f);
			star->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 0.3f, 1.0f);
			star->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);
			star->GetComponent<Transform3D>()->roll = 90.0f;*/
		} 
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && Scene::scene->p_player->GetComponent<Camera>()->state == TOP_VIEW) {

	}
}
void Motion(int x, int y)
{

}

void Motion2(int x, int y)
{
	if (Scene::scene->p_player->GetComponent<Camera>()->state == TOP_VIEW) {
		msx = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2);
		msy = -((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2);
	}
	if (Scene::scene->p_player->GetComponent<Camera>()->state == FIRST_VIEW) {
		if (x > WINDOWX  - 100 || x < 100 || y > WINDOWY - 100 || y < 100) {
			SetCursorPos(WINDOWX / 2, WINDOWY / 2);
		}

		float xoffset = x - intmpx;
		float yoffset = intmpy - y;
		intmpx = x;
		intmpy = y;

		if (abs(xoffset) < WINDOWX / 4 && abs(yoffset) < WINDOWX / 4) {

			xoffset *= 0.2;
			yoffset *= 0.2;
		
			Scene::scene->p_player->GetComponent<Camera>()->fpsy += xoffset;
			Scene::scene->p_player->GetComponent<Camera>()->fpsup += yoffset;


			if (Scene::scene->p_player->GetComponent<Camera>()->fpsup > 70.0f)
				Scene::scene->p_player->GetComponent<Camera>()->fpsup = 70.0f;
			if (Scene::scene->p_player->GetComponent<Camera>()->fpsup < -70.0f)
				Scene::scene->p_player->GetComponent<Camera>()->fpsup = -70.0f;

		}
	}
}

void keyboard(unsigned char key2, int x, int y) {
	key[key2] = true;
	
	if (key2 == 27) {
		exit(0);
	}

	switch (key2) {
	case '1':
		if (Scene::scene->p_player->GetComponent<Camera>()->state == FIRST_TO_TOP ||
			Scene::scene->p_player->GetComponent<Camera>()->state == FIRST_VIEW)
			Scene::scene->p_player->GetComponent<Camera>()->state = FIRST_TO_TOP;
		else
			Scene::scene->p_player->GetComponent<Camera>()->state = TOP_TO_FIRST;
		break;
	case '2':
		if (Scene::scene->p_player->GetComponent<Camera>()->state == FIRST_TO_TOP || 
			Scene::scene->p_player->GetComponent<Camera>()->state == FIRST_VIEW)
			Scene::scene->p_player->GetComponent<Camera>()->state = FIRST_TO_TOP;
		else 
			Scene::scene->p_player->GetComponent<Camera>()->state = TOP_TO_FIRST;
		break;
	case '3':
		SceneChange(sc.size() + 1);
		break;
	case 'q':
		if (Scene::scene->p_player->GetComponent<Camera>()->state == TOP_VIEW) {
			if (--n_model < 0)
				n_model = 0;
		}
		break;
	case 'e':
		if (Scene::scene->p_player->GetComponent<Camera>()->state == TOP_VIEW) {
			if (++n_model == n_max_model)
				n_model = n_max_model - 1;
		}
		break;
	case 'c': // 임시 clear 키
		/*if (f_Light_ambients[0] < 0.3f) {
			for (int i{}; i < 3; ++i)
				f_Light_ambients[i] = 0.3f;
		}
		else {
			for (int i{}; i < 3; ++i)
				f_Light_ambients[i] = 0.29f;
		}*/
		for (auto& gameobject : Scene::scene->gameObjects) {
			if (gameobject->VAO == VAO[Cube] && gameobject->GetComponent<DestroyEffect>()) {
				gameobject->GetComponent<DestroyEffect>()->destroy = true;
				break;
			}
		}

		break;
	case VK_SPACE:
		if (Scene::scene->p_player->GetComponent<PlayerJump>() && 
			Scene::scene->p_player->GetComponent<Transform3D>()->velocity.y <= 0.002f && Scene::scene->p_player->GetComponent<Transform3D>()->velocity.y >= -0.002f)
			Scene::scene->p_player->GetComponent<Transform3D>()->velocity += Scene::scene->p_player->GetComponent<PlayerJump>()->jump_acceleration;
		else {
			auto p = find(Scene::scene->p_player->Item_bag.begin(), Scene::scene->p_player->Item_bag.end(), Shoes);
			if (p != Scene::scene->p_player->Item_bag.end()) {
				Scene::scene->p_player->GetComponent<Transform3D>()->velocity += Scene::scene->p_player->GetComponent<PlayerJump>()->jump_acceleration;
				Scene::scene->p_player->Item_bag.erase(p);
			}
		}
		break;
	}

	glutPostRedisplay();
}
void keyboard2(unsigned char key2, int x, int y) {
	key[key2] = false;
	glutPostRedisplay();
}

void TimerFunction(int value) {

	bool collide = false;

	if (key['a'] == true) {						// 위로 이동
		Scene::scene->p_player->GetComponent<Transform3D>()->position.x += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
		Scene::scene->p_player->GetComponent<Transform3D>()->position.z -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
	}
	if (key['d'] == true) {						// 아래로 이동
		Scene::scene->p_player->GetComponent<Transform3D>()->position.x -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
		Scene::scene->p_player->GetComponent<Transform3D>()->position.z += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
	}
	if (key['s'] == true) {						// 왼쪽으로 이동
		Scene::scene->p_player->GetComponent<Transform3D>()->position.x -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
		Scene::scene->p_player->GetComponent<Transform3D>()->position.z -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
	}
	if (key['w'] == true) {						// 오른쪽으로 이동
		Scene::scene->p_player->GetComponent<Transform3D>()->position.x += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
		Scene::scene->p_player->GetComponent<Transform3D>()->position.z += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
	}

	glutPostRedisplay();

	glutTimerFunc(10, TimerFunction, 1);

}

void SceneChange(int num_scene)
{
	auto p = find_if(sc.begin(), sc.end(), [num_scene](const Scene* p_s) {
		return p_s->n_scene == num_scene;
		});

	if (p == sc.end()) {
		sc.emplace_back(new GameScene(num_scene, num_shape_list, texture, VAO, s_program));
	}
	else {
		(*p)->scene = (*p);
	}
}