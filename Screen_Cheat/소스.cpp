#define  _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define WINDOWX 800
#define WINDOWY 800
#define pie 3.14159265358979324846 // 난 파이

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

#include "stb_image.h"
#include "shader.h"
#include "objRead.cpp"

#include "Mmsystem.h"
#include "Digitalv.h"

MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;

using namespace std;

random_device rd;
default_random_engine dre(rd());
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
int num_Triangle;
int num_vertices = 3;
int num_triangles = 1;
int num_Sphere = 0;
int num_cube = 0;
int num_Gun;
int num_Tank;
int num_Tong;

float sunSize;
int shape = 1;					// 불러올 모양 (1. 육면체, 2. 구)

// 텍스쳐 변수

int img = 7;
GLuint texture[20];
int Imagenum = 0;
int widthImage, heightImage, numberOfChannel = 0;

// 게임 변수

int game = 1;					// 게임 state

int intmpx = 0;
int intmpy = 0;

float fpsy = 0;					// 1p 좌 우 시야
float fpsup = 0;				// 1p 위 아래 시야
float fpsy2 = 0;					// 1p 좌 우 시야
float fpsup2 = 0;				// 1p 위 아래 시야
float walkmove = 0;				// 1p 걷는 흔들림
bool walkmove2 = false;
float shootmove = 0;			// 1p 총 흔들림
bool shootmove2 = false;
bool shoot = false;
float jumpcount = 0;			// 캐릭터 점프 이동거리

float cx = 0;					// 카메라 x
float cz = 0;					// 카메라 z
float ry = 0;					// 회전

int clone[5];					// 좀비
float mx[6];					// 캐릭터들 x
float my[6];					// 캐릭터들 y
float mz[6];					// 캐릭터들 z
float leg[6];					// 캐릭터들 다리 움직임
float turn[6];					// 캐릭터들 돌기
bool walk[6];					// 캐릭터들 걷기 체크1
bool walk2[6];					// 캐릭터들 걷기 체크2
bool jump[6];					// 캐릭터들 점프 체크
float savey[6];					// 캐릭터들 점프 시 y저장
int dir[6];						// 캐릭터들 방향
int mummy;

float mousex = 0;				// 마우스 x
float mousey = 0;				// 마우스 y

float makelegX[5];				// 좀비 다리만들기 x
float makelegY[5];				// 좀비 다리만들기 y
float makehead[5];				// 좀비 머리만들기 x
float makearmX[5];				// 좀비 팔만들기 x
float makearmY[5];				// 좀비 팔만들기 y
float makenose[5];				// 좀비 코만들기

float boom[6];					// 캐릭터들 폭발
bool fireball = false;			// 공격

int dir2;						// 주인공의 마지막 움직임
float clonespeed[5];			// 좀비 이동속도

float turnY2 = 0;
float boxturn = 0;
int num = 0;
float zsize = 1;

int count2 = 0;

int now;
float color2[3];
float fb[4];

float dieing = 0;				// 죽는 애니메이션
bool die[6];					// 캐릭터 죽음 체크

float grav = 0;

glm::vec3 cameraPos = glm::vec4(mx[0], my[0], mz[0], 0.0f);
glm::vec3 cameraDirection = glm::vec4(0.0, fpsup + walkmove, -2.0, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 gunPos = glm::vec3(0.0f, 1.0f, 0.0f);

// 맵 만들기 변수

int itemnum = 0;
int itemkind = 0;
bool cl = false;

class Item {
private:

public:
	int state = 0;											// 0 = 마우스 위		1 = 설치됨
	int kind = 0;
	float left = 0;
	float right = 0;
	float top = 0;
	float bottom = 0;
	float front = 0;
	float back = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float plus = 0;
	float savey = 0;

	glm::vec3 s;
	glm::vec3 r;
	glm::vec3 t;



	void Update() {
		
		if (this->state == 0) {
			this->kind = itemkind;
		}

		if (this->kind == 0) {								// 나무 상자
			this->s = glm::vec3(0.3f + this -> plus, 0.3f + this->plus, 0.3f + this->plus);
		}
		else if (this->kind == 1) {							// 세로 철창
			this->s = glm::vec3(1.0f + this->plus, 1.0f + this->plus, 0.001f);
		}
		else if (this->kind == 2) {							// 가로 철창
			this->s = glm::vec3(0.001f, 1.0f + this->plus, 1.0f + this->plus);
		}
		else if (this->kind == 3) {							// 탱크
			this->s = glm::vec3(0.7f + this->plus, 0.4f + this->plus, 0.6f + this->plus);
		}

		this->t = glm::vec3(this -> x, this->y, this-> z);
		this->savey = this->y + this->s.y;
	}

	void Draw() {
		if (this->kind == 0) {								// 나무상자
			glBindVertexArray(VAO[0]);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
		}
		else if (this->kind == 1) {							// 세로 철창
			glBindVertexArray(VAO[0]);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
		}
		else if (this->kind == 2) {							// 가로 철창
			glBindVertexArray(VAO[0]);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
		}
		else if (this->kind == 3) {							// 탱크
			glBindVertexArray(VAO[1]);
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glDrawArrays(GL_TRIANGLES, 0, num_Tank);
		}
	}

	bool check(float mx, float my, float mz) {
		float x1, x2, y1, y2, z1, z2;
		x1 = this->x - this->s.x;
		x2 = this->x + this->s.x;
		y1 = this->y - this->s.y;
		y2 = this->y + this->s.y;
		z1 = this->z - this->s.z;
		z2 = this->z + this->s.z;
		if (x1 < mx + 0.03 && mx - 0.03 < x2 &&
			y1 < my + 0.5 && my - 0.03 < y2 &&
			z1 < mz + 0.03 && mz - 0.03 < z2
			) {							
			return true;
		}
		else {
			return false;
		}
	}


};

Item item[1000];
Item setblock;



// 응가 변수

glm::mat4 TR = glm::mat4(1.0f);
int Click = 0;
bool key[256];
float msx, msy = 0;
bool start = false;

bool checkBtoB(Item B1, Item B2);
bool checkZtoB(float x,float y, float z, Item B2);
bool checkBtoB(Item B1 , Item B2) {

	if (B2.x - B2.s.x < B1.x + B1.s.x && B1.x - B1.s.x < B2.x + B2.s.x &&
		B2.y - B2.s.y < B1.y + B1.s.y && B1.y - B1.s.y < B2.y + B2.s.y &&
		B2.z - B2.s.z < B1.z + B1.s.z && B1.z - B1.s.z < B2.z + B2.s.z
		) {
	
		return true;
	}
	else {
		return false;
	}
}
bool checkZtoB(float x, float y, float z, Item B2) {

	if (B2.x - B2.s.x < x + 0.03 && x - 0.03 < B2.x + B2.s.x &&
		B2.y - B2.s.y < y + 0.5 && y - 0.03 < B2.y + B2.s.y &&
		B2.z - B2.s.z < z + 0.03 && z - 0.03 < B2.z + B2.s.z
		) {
		
		return true;
	}
	else {
		return false;
	}
	
}
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

	InitBuffer();
	InitTexture();

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
	if (street == 0) {
		num_Triangle = loadObj_normalize_center_3f("cube.obj");
	}
	else if (street == 1) {
		num_Tank = loadObj_normalize_center_4f("tank.obj");
	}
	else if (street == 2) {
		num_Tong = loadObj_normalize_center_4f("sohwajeon.obj");
	}
	else if (street == 3) {
		num_Sphere = loadObj_normalize_center_3f("sphere.obj");
	}
	else if (street == 4) {
		
	}

	glUseProgram(s_program[0]);
	glBindVertexArray(VAO[street]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[street]);
	glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[street]);
	glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_uv[street]);
	glBufferData(GL_ARRAY_BUFFER, outuv.size() * sizeof(glm::vec3), &outuv[0], GL_STATIC_DRAW);
	GLint tAttribute = glGetAttribLocation(s_program[0], "aTex");
	glVertexAttribPointer(tAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(tAttribute);

	outvertex = std::vector< glm::vec3 >(0.0f);  // 다음 obj 불러오기 위한 초기화
	outnormal = std::vector< glm::vec3 >(0.0f);
	outuv = std::vector< glm::vec2 >(0.0f);

	vertexIndices = std::vector< unsigned int >(0.0f);
	uvIndices = std::vector< unsigned int >(0.0f);
	normalIndices = std::vector< unsigned int >(0.0f);
	temp_vertices = std::vector< glm::vec3 >(0.0f);
	temp_uvs = std::vector< glm::vec2 >(0.0f);
	temp_normals = std::vector< glm::vec3 >(0.0f);
}


void InitTexture()
{
	BITMAPINFO* bmp;
	string map[20] = { "main.png","B.png","C.png","D.png","E.png","body.png","face.png","gun_tex.png" };
	glGenTextures(20, texture); //--- 텍스처 생성

	for (int i = 0; i < 20; ++i) {
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
	// 시작 변수(한번 만 설정 해야함)
	if (!start) {
		// obj가 새로 생기면 추가로 bind 하고, 번호에 맞는 VAO[n]을 써야함
		InitBuffer_bind(0); // 0 : 정육면체, 1 : 탱크, 2 : 총
		InitBuffer_bind(1);
		InitBuffer_bind(2);
		InitBuffer_bind(3);

		glEnable(GL_DEPTH_TEST);



		for (int i = 0; i < 12; ++i) {										// (0,1) (2,3) (4,5) (6,7) (8,9) (10,11) = 캐릭터
			if (i < 6) {
				walk[i] = false;
				walk2[i] = false;
				jump[i] = false;
				savey[i] = -1.3;
				dir[i] = 1;
				mx[i] = uid(dre);
				my[i] = -0.65;
				mz[i] = uid(dre);
				leg[i] = 0;
				turn[i] = 0;
				if (i < 5) {
					clone[i] = 1;
					makelegX[i] = 0;
					makelegY[i] = 0;
					makehead[i] = 0;
					makearmX[i] = 0;
					makearmY[i] = 0;
					makenose[i] = 0;

				}
				boom[i] = 0;
				die[i] = false;
			}
		}

		my[0] = -0.5;
		for (int i = 0; i < 3; ++i) {
			color2[i] = 1.0;
		}

		clonespeed[0] = 0.007;
		clonespeed[1] = 0.009;
		clonespeed[2] = 0.008;
		clonespeed[3] = 0.010;
		clonespeed[4] = 0.012;
		 
		grav = 0.04;


		mciOpen.lpstrElementName = "bgm.mp3" ;
		mciOpen.lpstrDeviceType = "mpegvideo";

		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
			(DWORD)(LPVOID)&mciOpen);

		dwID = mciOpen.wDeviceID;

		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);
		start = true;
	}
	//*************************************************************************
	// 출력 설정

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (game == 0 || game == 2) {
		ShowCursor(false);
	}
	else {
		ShowCursor(true);
	}


	//*************************************************************************
	// 카메라 설정
	unsigned int color = glGetUniformLocation(s_program[0], "outColor");
	unsigned int modelLocation = glGetUniformLocation(s_program[0], "model");
	unsigned int viewLocation = glGetUniformLocation(s_program[0], "view");
	unsigned int projLocation = glGetUniformLocation(s_program[0], "projection");

	glm::mat4 Vw = glm::mat4(1.0f);
	glm::mat4 Cp = glm::mat4(1.0f);

	Cp = glm::rotate(Cp, (float)glm::radians(fpsy), glm::vec3(0.0f, 1.0f, 0.0f));

	cameraPos = glm::vec4(mx[0], my[0], mz[0], 0.0f);
	//cameraDirection = glm::vec4(0.0, fpsup + walkmove, -2.0, 0.0f) * Cp;
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



	Vw = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);

	glm::mat4 Pj = glm::mat4(1.0f);

	Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.0005f, 40.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);

	//*************************************************************************
	// 조명 설정


	int lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 5.0, 3.0);
	int lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	//*************************************************************************
	// 그리기 부분

	glUseProgram(s_program[0]);



	if (game == 0) {
		glViewport(0, 0, WINDOWX, WINDOWY);

		glm::mat4 Vw = glm::mat4(1.0f);
		glm::mat4 Cp = glm::mat4(1.0f);

		Cp = glm::rotate(Cp, (float)glm::radians(fpsy), glm::vec3(0.0f, 1.0f, 0.0f));

		cameraPos = glm::vec4(mx[0], my[0], mz[0], 0.0f);
		//cameraDirection = glm::vec4(0.0, 0.0, -4.0, 0.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



		Vw = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);

		glm::mat4 Pj = glm::mat4(1.0f);

		Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.0005f, 40.0f);
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);

		Imagenum = 4;

		glBindVertexArray(VAO[0]);
		TR = glm::mat4(1.0f);																		// 맵
		TR = glm::translate(TR, glm::vec3(0.0f, 1.7f, 0.0f));
		TR = glm::scale(TR, glm::vec3(7.0, 3.0, 7.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
		glDrawArrays(GL_TRIANGLES, 0, num_Triangle);


		glBindVertexArray(VAO[0]);
		// 클론 블럭
		for (int i = 1; i < 6; ++i) {
			Imagenum = 0;

			if (clone[i - 1] == 0) {
				TR = glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], -1.3 + 0.5, mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(boxturn), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.1, 0.1, 0.1));

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
			}
		}

		//glBindVertexArray(VAO[0]);
		//// 장애물
		//for (int i = 0; i < 4; ++i) {
		//	Imagenum = 2;
		//	TR = glm::mat4(1.0f);
		//	TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		//	TR = glm::translate(TR, glm::vec3(boxx[i], -0.4, boxy[i]));
		//	TR = glm::rotate(TR, (float)glm::radians(boxturn), glm::vec3(0.0f, 1.0f, 0.0f));
		//	TR = glm::rotate(TR, 90.0f, glm::vec3(1.0f, 0.0f, 1.0f));
		//	TR = glm::scale(TR, glm::vec3(0.1, 0.1, 0.1));

		//	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		//	glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
		//	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
		//}
		if (item[itemnum].state == 0) {
			itemnum--;
		}

		glBindVertexArray(VAO[0]);
		glm::mat4 TR2 = glm::mat4(1.0f);
		TR2 = glm::scale(TR, glm::vec3(2.0, 2.0, 2.0));
		// 클론 만들어지기
		for (int i = 1; i < 6; ++i) {
			if (clone[i - 1] == 1) {
				Imagenum = 5;
				glBindVertexArray(VAO[0]);
				// 왼 다리 그리기
				TR =  glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));

				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(-makelegX[i - 1], makelegY[i - 1], 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 오른 다리 그리기
				TR =  glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(makelegX[i - 1], makelegY[i - 1], 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 왼 팔 그리기
				TR =  glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(-makearmX[i - 1], makearmY[i - 1], 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 오른 팔 그리기
				TR =  glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(makearmX[i - 1], makearmY[i - 1], 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));

				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 몸 그리기
				TR =  glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], my[i] + 0.075, mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.05, 0.05, 0.03));
				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				
			}
		}
		// 클론 그림
		glBindVertexArray(VAO[0]);

		// 캐릭터
		for (int i = 1; i < 6; ++i) {									
			Imagenum = 5;
			if (i == 0 || (i != 0 && clone[i - 1] == 2)) {
				// 왼 다리 그리기
				TR =  glm::mat4(1.0f);
				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i] - boom[i], my[i] - boom[i], mz[i]));

				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(-0.01, 0.03, 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));

				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 오른 다리 그리기
				TR =  glm::mat4(1.0f);

				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i] + boom[i], my[i] - boom[i], mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.01, 0.03, 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 왼 팔 그리기
				TR =  glm::mat4(1.0f);

				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i] - boom[i], my[i] + boom[i], mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(-0.03, 0.08, 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 오른 팔 그리기
				TR =  glm::mat4(1.0f);

				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i] + boom[i], my[i] + boom[i], mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.03, 0.08, 0.0));

				TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

				// 몸 그리기
				TR =  glm::mat4(1.0f);

				TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::translate(TR, glm::vec3(mx[i], my[i] + 0.08, mz[i]));
				TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0f, 1.0f, 0.0f));
				TR = glm::scale(TR, glm::vec3(0.03, 0.04, 0.03));
				modelLocation = glGetUniformLocation(s_program[0], "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
				glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
				glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

			}
		}



		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 블렌딩

		Imagenum = 1;
		for (int i = 0; i < itemnum + 1; ++i) {
			item[i].Update();
			TR = glm::mat4(1.0f);																		// 오브젝트 그리기
			TR = glm::translate(TR, glm::vec3(item[i].t));
			TR = glm::scale(TR, glm::vec3(item[i].s));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			item[i].Draw();
		}

		glBindVertexArray(VAO[3]);

		if (fireball == true) {
			TR = glm::mat4(1.0f);
			TR = glm::translate(TR, glm::vec3(fb[0], fb[1], fb[2]));
			TR = glm::scale(TR, glm::vec3(0.01, 0.01, 0.01));

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			glBindTexture(GL_TEXTURE_2D, texture[6]);
			glDrawArrays(GL_TRIANGLES, 0, num_Sphere);
		}


		glEnable(GL_CULL_FACE);

		Imagenum = 7;

		glBindVertexArray(VAO[0]);

		gunPos.x = cos(glm::radians(fpsup)) * cos(glm::radians(fpsy)) * 0.2 + cameraPos.x;
		gunPos.y = sin(glm::radians(fpsup)) * 0.2 + cameraPos.y + walkmove * 0.4 - 0.04;
		gunPos.z = cos(glm::radians(fpsup)) * sin(glm::radians(fpsy)) * 0.2 + cameraPos.z;

		TR = glm::mat4(1.0f);

		TR = glm::translate(TR, glm::vec3(gunPos.x, gunPos.y, gunPos.z));

		TR = glm::rotate(TR, (float)glm::radians(-fpsy), glm::vec3(0.0, 1.0, 0.0));
		TR = glm::rotate(TR, (float)glm::radians(fpsup), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::rotate(TR, (float)glm::radians(shootmove), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.05));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
		glDrawArrays(GL_TRIANGLES, 0, num_Triangle);


		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND); // 블렌딩 해제






	}

	else if (game == 1) {																			// 메인 화면

		lightPosLocation = glGetUniformLocation(s_program[0], "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
		glUniform3f(lightPosLocation, 1.0, 0.0, 0.0);
		lightColorLocation = glGetUniformLocation(s_program[0], "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
		glUniform3f(lightColorLocation, 0.6, 0.6, 0.6);


		glm::mat4 Vw = glm::mat4(1.0f);
		glm::mat4 Cp = glm::mat4(1.0f);

		glm::vec3 cameraPos = glm::vec4(1.0, 0.0, 0.0, 0.0f);
		glm::vec3 cameraDirection = glm::vec4(-1.0, 0.0, 0.0, 0.0f) * Cp;
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Vw = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);

		glm::mat4 Pj = glm::mat4(1.0f);

		Pj = Pj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.00f);
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);

		// 그리기 코드
		glBindVertexArray(VAO[0]);
		TR = glm::mat4(1.0f);
		TR = glm::translate(TR, glm::vec3(0.0f, 0.0f, 0.0f));
		TR = glm::scale(TR, glm::vec3(0.5, 1.0, 1.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
		glDrawArrays(GL_TRIANGLES, 0, num_Triangle);

	}

	else if (game == 2) {																			// 맵 만들기

		glm::vec3 cameraPos = glm::vec4(0.0, 14.0, 0.0, 0.0f);
		glm::vec3 cameraDirection = glm::vec4(0.0, -1.0, 0.0, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);

		Vw = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);

		glm::mat4 Pj = glm::mat4(1.0f);

		Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.0005f, 40.0f);
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);


		// 그리기 코드



		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 블렌딩

		
		for (int i = 0; i < itemnum+1; ++i) {
			item[i].Update();
			TR = glm::mat4(1.0f);																		// 오브젝트 그리기
			TR = glm::translate(TR, glm::vec3(item[i].t));
			TR = glm::scale(TR, glm::vec3(item[i].s));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			item[i].Draw();
		}
		Imagenum = 4;
		glBindVertexArray(VAO[0]);
		TR = glm::mat4(1.0f);																		// 맵
		TR = glm::translate(TR, glm::vec3(0.0f, 4.5f, 0.0f));
		TR = glm::scale(TR, glm::vec3(7.0, 10.0, 7.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));

		glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);
		glDrawArrays(GL_TRIANGLES, 0, num_Triangle);


		glDisable(GL_BLEND); // 블렌딩 해제

	}
	glutSwapBuffers();
}


void Reshape(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	msx = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2);
	msy = -((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2);

	if (game == 1) {

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

			if (msx > -0.8 && msx<-0.2 && msy>-0.8 && msy < -0.2) {
				game = 2;
			}
			else if (msx < 0.8 && msx > 0.2 && msy > -0.8 && msy < -0.2) {
				game = 0;
			}


		}


	}
	else if (game == 0) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			mousex = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 90;
			mousey = -((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2) * 1.5;
			Click = 1;
			fireball = true;
			fb[0] = gunPos.x;
			fb[1] = gunPos.y;
			fb[2] = gunPos.z;
			fb[3] = 1;
			shoot = true;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			fpsy += fpsy2;
			fpsup += fpsup2;
			fpsy2 = 0;
			fpsup2 = 0;
			Click = 0;
		}
	}
	else if (game == 2) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			item[itemnum].x = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 5;
			item[itemnum].y = -1.3 + item[itemnum].s.y;
			item[itemnum].z = ((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2) * 5;
			
			item[itemnum].state = 1;
			cl = true;
			Click = 1;


			bool Ycollide = false;
			for (int i = 0; i < itemnum; ++i) {
				Ycollide = checkBtoB(item[itemnum], item[i]);
				if (Ycollide) {
					if (item[itemnum].y <= item[i].y + item[i].s.y + item[itemnum].s.y) {
						item[itemnum].y = item[i].y + item[i].s.y + item[itemnum].s.y;
					}

				}
			}
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			Click = 0;
			if (cl == true) {
				itemnum++;
				cl = false;
			}
			item[itemnum].x = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 5;
			item[itemnum].z = ((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2) * 5;


		}
	}
}
void Motion(int x, int y)
{

}

void Motion2(int x, int y)
{
	if (game == 0) {
		if (x > WINDOWX - 100 || x < 100 || y > WINDOWY - 100 || y < 100) {
			SetCursorPos(WINDOWX / 2, WINDOWY / 2);
		}

		float xoffset = x - intmpx;
		float yoffset = intmpy - y;
		intmpx = x;
		intmpy = y;

		if (abs(xoffset) < WINDOWX / 4 && abs(yoffset) < WINDOWX / 4) {

			xoffset *= 0.2;
			yoffset *= 0.2;

			fpsy += xoffset;
			fpsup += yoffset;


			if (fpsup > 70.0f)
				fpsup = 70.0f;
			if (fpsup < -25.0f)
				fpsup = -25.0f;

		}

	}
	else if (game == 2) {
		item[itemnum].x = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 5;
		item[itemnum].z = ((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2) * 5;
	}
}

void keyboard(unsigned char key2, int x, int y) {
	key[key2] = true;

	if (game == 2) {
		if (key2 == 27) {
			exit(0);
		}
		switch (key2) {
		case '1':
			itemkind = 0;
			break;

		case '2':
			itemkind = 1;
			break;
		case '3':
			itemkind = 2;
			break;
		case '4':
			itemkind = 3;
			break;
		case '5':
			itemkind = 4;
			break;
		case '=':
			item[itemnum].plus += 0.01;
			break;
		case '-':
			item[itemnum].plus -= 0.01;
			break;
		}

	}
	if (key2 == 27) {
		exit(0);
	}
	switch (key2) {
	case ',':
		game = 0;
		break;

	case '.':
		game = 1;
		Imagenum = 0;
		break;
	case '/':
		game = 2;
		break;
	}
	glutPostRedisplay();
}
void keyboard2(unsigned char key2, int x, int y) {
	key[key2] = false;
	glutPostRedisplay();
}

void TimerFunction(int value) {
	if (fireball == true) {

		fb[0] += cos(glm::radians(fpsup)) * cos(glm::radians(fpsy)) * 0.1;
		fb[1] += sin(glm::radians(fpsup)) * 0.1;
		fb[2] += cos(glm::radians(fpsup)) * sin(glm::radians(fpsy)) * 0.1;

		
	}

	for (int i = 0; i < 6; ++i) {											// 죽으면 터짐
		if (die[i] == true) {
			boom[i] += 0.05;
		}
		if ((i != 0) && boom[i] >= 1) {
			mx[i] = uid(dre);
			my[i] = -0.65;
			mz[i] = uid(dre);
			clone[i - 1] = 1;
			die[i] = false;
			boom[i] = 0;
			makelegX[i - 1] = 0;
			makelegY[i - 1] = 0;
			makehead[i - 1] = 0;
			makearmX[i - 1] = 0;
			makearmY[i - 1] = 0;
			makenose[i - 1] = 0;
		}
	}

	if (boom[0] >= 1) {								// 죽으면 초기화
		

		for (int i = 0; i < 12; ++i) {										// (0,1) (2,3) (4,5) (6,7) (8,9) (10,11) = 캐릭터
			if (i < 6) {
				walk[i] = false;
				walk2[i] = false;
				jump[i] = false;
				savey[i] = -1.3;
				dir[i] = 1;
				mx[i] = uid(dre);
				my[i] = -0.65;
				mz[i] = uid(dre);
				leg[i] = 0;
				turn[i] = 0;
				if (i < 5) {
					clone[i] = 1;
					makelegX[i] = 0;
					makelegY[i] = 0;
					makehead[i] = 0;
					makearmX[i] = 0;
					makearmY[i] = 0;
					makenose[i] = 0;

				}
				boom[i] = 0;
				die[i] = false;
			}
		}

		my[0] = -0.5;
		for (int i = 0; i < 3; ++i) {
			color2[i] = 1.0;
		}

		clonespeed[0] = 0.007;
		clonespeed[1] = 0.009;
		clonespeed[2] = 0.008;
		clonespeed[3] = 0.010;
		clonespeed[4] = 0.012;

		grav = 0.04;

	}

	for (int i = 0; i < 6; ++i) {
		if (i == 0 || clone[i - 1] == 2) {
			if (walk[i] == true) {
				if (walk2[i] == false) {
					leg[i] += 6;
					if (leg[i] > 60) {
						walk2[i] = true;
					}
				}
				else {
					leg[i] -= 6;
					if (leg[i] < -60) {
						walk2[i] = false;
					}
				}
			}

			if (dir[i] == 1) {
				if (turn[i] < 90) {
					turn[i] += 10;
					if (turn[i] > 90) {
						turn[i] = 90;
					}
				}
				else if (turn[i] > 90) {
					turn[i] -= 10;
					if (turn[i] < 90) {
						turn[i] = 90;
					}
				}
			}
			else if (dir[i] == 2) {
				if (turn[i] < 270) {
					turn[i] += 10;
					if (turn[i] > 270) {
						turn[i] = 270;
					}
				}
				else if (turn[i] > 270) {
					turn[i] -= 10;
					if (turn[i] < 270) {
						turn[i] = 270;
					}
				}
			}
			else if (dir[i] == 3) {
				if (turn[i] < 180) {
					turn[i] += 10;
					if (turn[i] > 180) {
						turn[i] = 180;
					}
				}
				else if (turn[i] > 180) {
					turn[i] -= 10;
					if (turn[i] < 180) {
						turn[i] = 180;
					}
				}
			}
			else {
				if (turn[i] < 360) {
					turn[i] += 10;
					if (turn[i] > 360) {
						turn[i] = 360;
					}
				}
				else if (turn[i] > 360) {
					turn[i] -= 10;
					if (turn[i] < 360) {
						turn[i] = 360;
					}
				}
			}


		}
	}

	for (int i = 1; i < 6; ++i) {					// 로봇 따라가기
		if (clone[i - 1] == 2) {

			walk[i] = true;

			if ((mx[i] <= mx[0])) {
				bool collide = false;
				for (int j = 0; j < itemnum; ++j) {
					collide = checkZtoB(mx[i] + clonespeed[i - 1], my[i], mz[i], item[j]);
					if (collide) {
						break;
					}
				}
				if (!collide) {
					mx[i] += clonespeed[i - 1];
				}
				dir[i] = 1;
			}

			if ((mz[i] <= mz[0])) {
				bool collide = false;
				for (int j = 0; j < itemnum; ++j) {
					collide = checkZtoB(mx[i], my[i], mz[i] + clonespeed[i - 1], item[j]);
					if (collide) {
						break;
					}
				}
				if (!collide) {
					mz[i] += clonespeed[i - 1];
				}
				dir[i] = 4;

			}

			if ((mx[i] >= mx[0])) {
				bool collide = false;
				for (int j = 0; j < itemnum; ++j) {
					collide = checkZtoB(mx[i] - clonespeed[i - 1], my[i], mz[i], item[j]);
					if (collide) {
						break;
					}
				}
				if (!collide) {
					mx[i] -= clonespeed[i - 1];
				}
				dir[i] = 2;

			}

			if ((mz[i] >= mz[0])) {
				bool collide = false;
				for (int j = 0; j < itemnum; ++j) {
					collide = checkZtoB(mx[i], my[i], mz[i] - clonespeed[i - 1], item[j]);
					if (collide) {
						break;
					}
				}
				if (!collide) {
					mz[i] -= clonespeed[i - 1];
				}
				dir[i] = 3;
			}

			if (abs(mz[i] - mz[0]) > abs(mx[i] - mx[0])) {
				if (mz[i] >= mz[0]) {
					dir[i] = 3;
				}
				else {
					dir[i] = 4;
				}
			}
			else {
				if (mx[i] >= mx[0]) {
					dir[i] = 2;
				}
				else {
					dir[i] = 1;
				}
			}

		}
	}

	for (int i = 0; i < 6; ++i) {
		if (i == 0 || clone[i - 1] == 2) {
			if (walk[i] == true) {
				if (walk2[i] == false) {
					leg[i] += 6;
					if (leg[i] > 60) {
						walk2[i] = true;
					}
				}
				else {
					leg[i] -= 6;
					if (leg[i] < -60) {
						walk2[i] = false;
					}
				}
			}

		}


		for (int j = 1; j < 6; ++j) {																			// 플레이어와 좀비
			if ((mx[0] - 0.025 < mx[j] + 0.025) && (mz[0] - 0.015 < mz[j] + 0.015) &&
				(mx[0] + 0.025 > mx[j] - 0.025) && (mz[0] + 0.015 > mz[j] - 0.015) &&
				(my[0] - 0.1 < my[j] + 0.015) && (my[0] + 0.1 > my[j] - 0.015)) {
				if (clone[j - 1] == 2) {
					die[0] = true;
				}

			}
		}

		if (i != 0 && clone[i - 1] == 0) {																		// 플레이어와 비활성 좀비
			if ((mx[0] - 0.025 < mx[i] + 0.05) && (mz[0] - 0.015 < mz[i] + 0.05) &&
				(mx[0] + 0.025 > mx[i] - 0.05) && (mz[0] + 0.015 > mz[i] - 0.05) &&
				(my[0] - 0.1 < my[i] + 0.015) && (my[0] + 0.1 > my[i] - 0.015)) {
				clone[i - 1] = 1;
			}

		}

		if (i != 0 && clone[i - 1] == 2) {																		// 좀비와 파이어볼
			if ((mx[i] - 0.025 < fb[0] + 0.05) && (mz[i] - 0.015 < fb[2] + 0.05) &&
				(mx[i] + 0.025 > fb[0] - 0.05) && (mz[i] + 0.015 > fb[2] - 0.05) &&
				(my[i] - 0.4 < fb[1] + 0.05) && (my[i] + 0.4 > fb[1] - 0.05) &&
				
				fireball == true) {
				die[i] = true;
				fireball = false;
			}

		}

		if (jump[i] == true) {
			my[i] += grav;
			grav -= 0.001;

			if (grav <= 0.0) {
				jump[i] = false;
			}
		}


		walk[i] = false;
	}

	for (int i = 0; i < 5; ++i) {					// 로봇 만들어지는 과정
		if (clone[i] == 1) {
			makelegX[i] += 0.0001;
			makelegY[i] += 0.0003;

			makearmX[i] += 0.0001;
			makearmY[i] += 0.0008;

			makehead[i] += 0.0012;

			if (makelegX[i] > 0.01) {
				clone[i] = 2;
			}
		}
	}


	if (shoot == true) {
		if (shootmove2 == false && shootmove <= 5) {
			shootmove += 1;
			if (shootmove > 5) {
				shootmove2 = true;
			}
		}
		else {
			shootmove -= 2;
			if (shootmove < 0) {
				shootmove2 = false;
				shoot = false;
			}
		}
	}


	if (key['w'] == true || key['s'] == true || key['a'] == true || key['d'] == true) {
		if (walkmove2 == false) {
			walkmove += 0.001;
			if (walkmove > 0.02) {
				walkmove2 = true;
			}
		}
		else {
			walkmove -= 0.002;
			if (walkmove < 0) {
				walkmove2 = false;
			}
		}
	}

	else {
		walkmove = 0;
	}


	savey[0] = -1.3;

	bool Ycollide = false;
	
	for (int i = 0; i < itemnum + 1; ++i) {
		Ycollide = item[i].check(mx[0], my[0] - 0.8, mz[0]);
		if (Ycollide) {
			if (my[0] - 0.8 < item[i].savey) {
				savey[0] = item[i].savey;
				grav = 0.04;
			}
			break;
		}
	}

	if (my[0] - 0.8 > savey[0] && jump[0] == false) {
		my[0] -= grav;
		grav += 0.001;
		
		if (my[0] <= savey[0]) {
			my[0] = savey[0];
		}

		if (grav >= 0.04) {
			grav = 0.04;
		}
	}

	bool collide = false;

	if (key['a'] == true) {						// 위로 이동
		walk[0] = true;
		
		for (int i = 0; i < itemnum+1; ++i) {
			collide = item[i].check(mx[0] + sin((float)glm::radians(fpsy + fpsy2)) * 0.015, my[0] - 0.4, mz[0] - cos((float)glm::radians(fpsy + fpsy2)) * 0.015);
			if (collide) {

				break;
			}
		}
		if (!collide) {
			mx[0] += sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
			mz[0] -= cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
		}
		dir[0] = 3;
	}
	if (key['d'] == true) {						// 아래로 이동
		walk[0] = true;
		for (int i = 0; i < itemnum + 1; ++i) {
			collide = item[i].check(mx[0] - sin((float)glm::radians(fpsy + fpsy2)) * 0.015, my[0] - 0.4, mz[0] + cos((float)glm::radians(fpsy + fpsy2)) * 0.015);
			if (collide) {
			
				break;
			}
		}
		if (!collide) {
			mx[0] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
			mz[0] += cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
		}
		dir[0] = 4;
	}
	if (key['s'] == true) {						// 왼쪽으로 이동
		walk[0] = true;
		for (int i = 0; i < itemnum + 1; ++i) {
			collide = item[i].check(mx[0] - cos((float)glm::radians(fpsy + fpsy2)) * 0.015, my[0] - 0.4, mz[0] - sin((float)glm::radians(fpsy + fpsy2)) * 0.015);
			if (collide) {

				break;
			}
		}
		if (!collide) {
			mx[0] -= cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
			mz[0] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
		}
		dir[0] = 2;
	}
	if (key['w'] == true) {						// 오른쪽으로 이동
		walk[0] = true;
		for (int i = 0; i < itemnum + 1; ++i) {
			collide = item[i].check(mx[0] + cos((float)glm::radians(fpsy + fpsy2)) * 0.015, my[0] - 0.4, mz[0] + sin((float)glm::radians(fpsy + fpsy2)) * 0.015);
			if (collide) {

				break;
			}
		}
		if (!collide) {
			mx[0] += cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
			mz[0] += sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
		}
		dir[0] = 1;
	}

	if (key[VK_SPACE] == true) {
		jump[0] = true;
	}

	cameraDirection.x = cos(glm::radians(fpsup)) * cos(glm::radians(fpsy)) + cameraPos.x;
	cameraDirection.y = sin(glm::radians(fpsup)) + cameraPos.y + walkmove;
	cameraDirection.z = cos(glm::radians(fpsup)) * sin(glm::radians(fpsy)) + cameraPos.z;



	glutPostRedisplay();

	glutTimerFunc(10, TimerFunction, 1);

}