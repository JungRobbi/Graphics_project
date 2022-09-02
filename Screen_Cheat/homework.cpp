#define STB_IMAGE_IMPLEMENTATION
#define WINDOWX 800
#define WINDOWY 800
#define pie 3.14159265358979324846

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include "stb_image.h"

using namespace std;

random_device rd;
default_random_engine dre(rd());
uniform_real_distribution<float> uid(-2.0, 2.0);


void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid InitBuffer();
void InitShader();
GLchar* filetobuf(const char* file);
void InitShader();

GLuint shaderID;
GLint width, height;

GLuint vertexShader;
GLuint fragmentShader;

GLuint VAO, VBO[4];

int img = 4;
GLuint texture[4];
int Imagenum = 0;

class Plane {
public:
	GLfloat p[9];
	GLfloat n[9];
	GLfloat color[9];
	GLfloat tex[6];

	void Bind() {

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(1);


		glm::vec3 normal = glm::cross(glm::vec3(p[3] - p[0], p[4] - p[1], p[5] - p[2]), glm::vec3(p[6] - p[0], p[7] - p[1], p[8] - p[2]));
		for (int i = 0; i < 3; ++i) {
			n[(i * 3) + 0] = normal.x;
			n[(i * 3) + 1] = normal.y;
			n[(i * 3) + 2] = normal.z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(n), n, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(3);

	}


	void Draw() {
		glUseProgram(shaderID);
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[Imagenum]);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

BOOL Time, start = TRUE, TimeLoop;

float BackGround[] = { 0.0, 0.0, 0.0 };
Plane* Fvertex[20];

glm::vec4* vertex;
glm::vec4* face;
glm::vec3* outColor;


glm::mat4 TR = glm::mat4(1.0f);


FILE* FL;
int faceNum = 0;
int Click = 0;

float cx = 0;
float cz = 0;
float ry = 0;
float boxx[4];
float boxy[4];

int clone[5];
float mx[6];
float my[6];
float mz[6];
float leg[6];
float turn[6];
bool walk[6];
bool walk2[6];
bool jump[6];
float savey[6];
int dir[6];
bool key[256];

bool fps = false;
float fpsy = 0;
float fpsy2 = 0;
float fpsup = 0;
float fpsup2 = 0;
float mousex = 0;
float mousey = 0;

float makelegX[5];
float makelegY[5];
float makehead[5];
float makearmX[5];
float makearmY[5];
float makenose[5];

float boom[6];
bool fireball = false;

int dir2;
float movecount[5][4];

int order[6];
int clonecount = 1;
float clonespeed[5];

float turnY2 = 0;
float boxturn = 0;
int num = 0;
float zsize = 1;

int now;
float color2[3];
float fb[4];

float dieing = 0;
bool die[6];



int widthImage, heightImage, numberOfChannel;

void ReadObj(FILE* objFile);
void keyboard(unsigned char, int, int);
void keyboard2(unsigned char key2, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void vectoplane(Plane* p);
void planecolorset(Plane* p, int a);
GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);
void InitTexture();

BOOL CrossCheckfor4p(float x1, float  y1, float x2, float  y2, float x3, float  y3, float x4, float  y4);

void InitTexture()
{
	BITMAPINFO* bmp;
	string map[4] = { "band.png","floor.png","warn.png","player.png" };
	glGenTextures(4, texture); //--- 텍스처 생성

	for (int i = 0; i < 4; ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]); //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(map[i].c_str(), &widthImage, &heightImage, &numberOfChannel, 0);//--- 텍스처로 사용할 비트맵 이미지 로드하기
		glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의
		stbi_image_free(data);
	}

	glUseProgram(shaderID);
	int tLocation = glGetUniformLocation(shaderID, "outTex"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
}

void make_vertexShaders()
{

	GLchar* vertexShaderSource;

	vertexShaderSource = filetobuf("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);



	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		exit(-1);
	}
}
void make_fragmentShaders()
{
	GLchar* fragmentShaderSource = filetobuf("fragment.glsl");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);



	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		exit(-1);
	}

}
GLuint make_shaderProgram()
{
	GLint result;
	GLchar errorLog[512];
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기

	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		exit(-1);
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.

	return ShaderProgramID;
}
void InitShader()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
}
GLvoid InitBuffer() {
	//--- VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &VAO);
	//--- vertex data 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(4, VBO);
}


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	srand((unsigned int)time(NULL));
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOWX, WINDOWY); // 윈도우의 크기 지정
	glutCreateWindow("Example6");// 윈도우 생성	(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard2);
	glutTimerFunc(10, TimerFunction, 1);

	printf("- 조작 방법 -\n\n마우스 = 화면 조작\nw / a / s / d = 플레이어 이동\nSpace Bar = 플레이어 점프\ni / j / k / l = 파이어 볼\n\nr / R = 카메라 공전\nt / f / g / h = 카메라 이동\n1 = 1인칭 모드\n3 = 3인칭 모드\n- = 조명 어둡게 하기\n+ = 조명 밝게 하기\n[ 또는 ] = 조명 공전\n\nc = 초기화\nq = 프로그램 종료");
	InitTexture();
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	if (start) {
		start = FALSE;
		Time = TRUE; TimeLoop = TRUE;

		FL = fopen("cube.obj", "rt");										// 13 = 파이어 볼
		ReadObj(FL);
		fclose(FL);
		Fvertex[13] = (Plane*)malloc(sizeof(Plane) * faceNum);
		vectoplane(Fvertex[13]);
		planecolorset(Fvertex[13], 2);

		FL = fopen("cube.obj", "rt");
		ReadObj(FL);
		fclose(FL);
		Fvertex[12] = (Plane*)malloc(sizeof(Plane) * faceNum);
		vectoplane(Fvertex[12]);
		planecolorset(Fvertex[12], 1);										// 12 = 하얀색 ( 바닥 )

		for (int i = 0; i < 4; ++i) {
			boxx[i] = uid(dre);
			boxy[i] = uid(dre);

		}

		for (int i = 0; i < 12; ++i) {										// (0,1) (2,3) (4,5) (6,7) (8,9) (10,11) = 캐릭터
			FL = fopen("cube.obj", "rt");
			ReadObj(FL);
			fclose(FL);
			Fvertex[i] = (Plane*)malloc(sizeof(Plane) * faceNum);
			vectoplane(Fvertex[i]);
			planecolorset(Fvertex[i], 0);
			if (i < 6) {
				walk[i] = false;
				walk2[i] = false;
				jump[i] = false;
				savey[i] = -0.5;
				dir[i] = 1;
				mx[i] = uid(dre);
				my[i] = 0;
				mz[i] = uid(dre);
				leg[i] = 0;
				turn[i] = 0;
				if (i < 5) {
					clone[i] = 0;
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


		for (int i = 0; i < 3; ++i) {
			color2[i] = 1.0;
		}

		clonespeed[0] = 0.007;
		clonespeed[1] = 0.009;
		clonespeed[2] = 0.011;
		clonespeed[3] = 0.013;
		clonespeed[4] = 0.015;

		glEnable(GL_DEPTH_TEST);

	} // 초기화할 데이터



	glViewport(0, 0, 800, 800);
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	//배경

	glUseProgram(shaderID);
	glBindVertexArray(VAO);// 쉐이더 , 버퍼 배열 사용

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	unsigned int color = glGetUniformLocation(shaderID, "outColor");
	unsigned int modelLocation = glGetUniformLocation(shaderID, "model");
	unsigned int viewLocation = glGetUniformLocation(shaderID, "view");
	unsigned int projLocation = glGetUniformLocation(shaderID, "projection");

	int lightPosLocation = glGetUniformLocation(shaderID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, -cos((float)glm::radians(turnY2)) * (zsize), 0.0, sin((float)glm::radians(turnY2)) * (zsize));
	int lightColorLocation = glGetUniformLocation(shaderID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, color2[0], color2[0], color2[0]);

	glm::mat4 Vw = glm::mat4(1.0f);

	if (fps == false) {
		glm::vec3 cameraPos = glm::vec3(cx, 0.6, 6.0 + cz);
		glm::vec3 cameraDirection = glm::vec3(0.0, -0.6, -6.0);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Vw = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);
	}
	else {
		glm::mat4 Vw = glm::mat4(1.0f);
		glm::mat4 Cp = glm::mat4(1.0f);

		Cp = glm::rotate(Cp, (float)glm::radians(fpsy + fpsy2), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 cameraPos = glm::vec4(mx[0], my[0] + 0.15 + boom[0], mz[0], 0.0f);
		glm::vec3 cameraDirection = glm::vec4(0.0, -0.0, -2.0, 0.0f) * Cp;
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Vw = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &Vw[0][0]);
	}

	glm::mat4 Pj = glm::mat4(1.0f);
	Pj = glm::perspective(glm::radians(45.0f), (float)WINDOWX / (float)WINDOWY, 0.1f, 100.0f);
	Pj = glm::translate(Pj, glm::vec3(0.0, 0.0, 0.0));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &Pj[0][0]);

	// 그리기 코드


	TR = glm::mat4(1.0f);																								// 바닥
	TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(0.0, -0.5, 0.0));
	TR = glm::scale(TR, glm::vec3(5.0, 0.05, 5.0));

	modelLocation = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i = 0; i < 12; ++i) {
		Imagenum = 1;
		Fvertex[12][i].Bind();
		Fvertex[12][i].Draw();
	}

	for (int i = 0; i < 6; ++i) {
		Imagenum = 0;
		if (i == 0) {
			Imagenum = 3;
		}
		if (i == 0 || (i != 0 && clone[i - 1] == 2)) {
			// 왼 다리 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i] - boom[i], my[i] - boom[i], mz[i]));

			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(-0.01, 0.03, 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));

			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 오른 다리 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i] + boom[i], my[i] - boom[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.01, 0.03, 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 왼 팔 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i] - boom[i], my[i] + boom[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(-0.03, 0.08, 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 오른 팔 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i] + boom[i], my[i] + boom[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.03, 0.08, 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 몸 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i] + 0.075, mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));

			TR = glm::scale(TR, glm::vec3(0.05, 0.05, 0.03));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2 + 1][j].Bind();
				Fvertex[i * 2 + 1][j].Draw();
			}

			// 머리 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i] + 0.12 + boom[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));

			TR = glm::scale(TR, glm::vec3(0.04, 0.04, 0.04));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2 + 1][j].Bind();
				Fvertex[i * 2 + 1][j].Draw();
			}

			// 코 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i] + boom[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, 0.12, 0.025));

			TR = glm::scale(TR, glm::vec3(0.01, 0.01, 0.01));

			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}
		}
	}

	// 장애물
	for (int i = 0; i < 4; ++i) {
		Imagenum = 2;
		TR = glm::mat4(1.0f);
		TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::translate(TR, glm::vec3(boxx[i], -0.4, boxy[i]));
		TR = glm::rotate(TR, (float)glm::radians(boxturn), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::rotate(TR, 90.0f, glm::vec3(1.0f, 0.0f, 1.0f));
		TR = glm::scale(TR, glm::vec3(0.1, 0.1, 0.1));

		modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int j = 0; j < 12; ++j) {
			Fvertex[12][j].Bind();
			Fvertex[12][j].Draw();
		}
	}

	// 클론 블럭
	for (int i = 1; i < 6; ++i) {
		Imagenum = 0;
		if (clone[i - 1] == 0) {
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], -0.4, mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(boxturn), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::scale(TR, glm::vec3(0.1, 0.1, 0.1));

			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i][j].Bind();
				Fvertex[i][j].Draw();
			}
		}
	}

	// 클론 만들어지기
	for (int i = 1; i < 6; ++i) {
		if (clone[i - 1] == 1) {
			// 왼 다리 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));

			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(-makelegX[i - 1], makelegY[i - 1], 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));

			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 오른 다리 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(makelegX[i - 1], makelegY[i - 1], 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 왼 팔 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(-makearmX[i - 1], makearmY[i - 1], 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(-leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 오른 팔 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(makearmX[i - 1], makearmY[i - 1], 0.0));

			TR = glm::translate(TR, glm::vec3(0.0, 0.025, 0.0));
			TR = glm::rotate(TR, (float)glm::radians(leg[i]), glm::vec3(1.0f, 0.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, -0.025, 0.0));

			TR = glm::scale(TR, glm::vec3(0.01, 0.05, 0.01));


			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}

			// 몸 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i] + 0.075, mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::scale(TR, glm::vec3(0.05, 0.05, 0.03));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2 + 1][j].Bind();
				Fvertex[i * 2 + 1][j].Draw();
			}

			// 머리 그리기
			TR = glm::mat4(1.0f);
			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i] + makehead[i - 1], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::scale(TR, glm::vec3(0.04, 0.04, 0.04));
			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2 + 1][j].Bind();
				Fvertex[i * 2 + 1][j].Draw();
			}

			// 코 그리기
			TR = glm::mat4(1.0f);

			TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(mx[i], my[i], mz[i]));
			TR = glm::rotate(TR, (float)glm::radians(turn[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			TR = glm::translate(TR, glm::vec3(0.0, makehead[i - 1], 0.025));
			TR = glm::scale(TR, glm::vec3(0.01, 0.01, 0.01));

			modelLocation = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
			for (int j = 0; j < 12; ++j) {
				Fvertex[i * 2][j].Bind();
				Fvertex[i * 2][j].Draw();
			}
		}
	}

	if (fireball == true) {
		TR = glm::mat4(1.0f);
		TR = glm::rotate(TR, (float)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::translate(TR, glm::vec3(fb[0], fb[1], fb[2]));
		TR = glm::rotate(TR, (float)glm::radians(boxturn), glm::vec3(0.0f, 1.0f, 0.0f));
		TR = glm::scale(TR, glm::vec3(0.03, 0.03, 0.03));

		modelLocation = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		for (int j = 0; j < 12; ++j) {
			Fvertex[13][j].Bind();
			Fvertex[13][j].Draw();
		}
	}


	glutSwapBuffers();
	glutPostRedisplay();
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{

}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mousex = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 90;
		mousey = -((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2) * 1.5;
		Click = 1;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		fpsy += fpsy2;
		fpsup += fpsup2;
		fpsy2 = 0;
		fpsup2 = 0;
		Click = 0;
	}

}

void Motion(int x, int y)
{

	if (Click == 1) {
		if (fps == false) {
			ry = ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 180;
		}
		else {
			fpsy2 = -mousex + ((float)x - ((float)WINDOWX / (float)2)) / ((float)WINDOWX / (float)2) * 90;
			fpsup2 = -mousey - ((float)y - ((float)WINDOWY / (float)2)) / ((float)WINDOWY / (float)2) * 1.5;
		}
	}

}

void keyboard(unsigned char key2, int x, int y) {
	key[key2] = true;
	switch (key2) {
	case 'c':
		boom[0] = 0;
		for (int i = 0; i < 4; ++i) {
			boxx[i] = uid(dre);
			boxy[i] = uid(dre);
		}

		for (int i = 0; i < 12; ++i) {										// (0,1) (2,3) (4,5) (6,7) (8,9) (10,11) = 캐릭터
			if (i < 6) {
				walk[i] = false;
				die[i] = false;
				walk2[i] = false;
				jump[i] = false;
				savey[i] = -0.5;
				dir[i] = 1;
				mx[i] = uid(dre);
				my[i] = 0;
				mz[i] = uid(dre);;
				leg[i] = 0;
				turn[i] = 0;
				if (i < 5) {
					clone[i] = 0;
					makelegX[i] = 0;
					makelegY[i] = 0;
					makehead[i] = 0;
					makearmX[i] = 0;
					makearmY[i] = 0;
					makenose[i] = 0;
					for (int j = 0; j < 4; ++j) {
						movecount[i][j] = 0;
					}

				}
			}
		}
		cx = 0;
		cz = 0;
		ry = 0;

		clonecount = 1;
		turnY2 = 0;
		boxturn = 0;
		num = 0;
		zsize = 1;
		dieing = 0;

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
		if (fb[3] == 1) {
			if (fps == true) {
				fb[0] += cos((float)glm::radians(fpsy + fpsy2)) * 0.05;
				fb[2] += sin((float)glm::radians(fpsy + fpsy2)) * 0.05;
			}
			else {
				fb[0] += cos((float)glm::radians(ry)) * 0.05;
				fb[2] += sin((float)glm::radians(ry)) * 0.05;
			}
		}
		else if (fb[3] == 2) {
			if (fps == true) {
				fb[0] -= cos((float)glm::radians(fpsy + fpsy2)) * 0.05;
				fb[2] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.05;
			}
			else {
				fb[0] -= cos((float)glm::radians(ry)) * 0.05;
				fb[2] -= sin((float)glm::radians(ry)) * 0.05;
			}
		}
		else if (fb[3] == 3) {
			if (fps == true) {
				fb[0] += sin((float)glm::radians(fpsy + fpsy2)) * 0.05;
				fb[2] -= cos((float)glm::radians(fpsy + fpsy2)) * 0.05;
			}
			else {
				fb[0] += sin((float)glm::radians(ry)) * 0.05;
				fb[2] -= cos((float)glm::radians(ry)) * 0.05;
			}


		}
		else if (fb[3] == 4) {
			if (fps == true) {
				fb[0] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.05;
				fb[2] += cos((float)glm::radians(fpsy + fpsy2)) * 0.05;
			}
			else {
				fb[0] -= sin((float)glm::radians(ry)) * 0.05;
				fb[2] += cos((float)glm::radians(ry)) * 0.05;
			}

		}
		else {
			if (fps == true) {
				fb[0] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.05;
				fb[2] += cos((float)glm::radians(fpsy + fpsy2)) * 0.05;
			}
			else {
				fb[0] -= sin((float)glm::radians(ry)) * 0.05;
				fb[2] += cos((float)glm::radians(ry)) * 0.05;
			}
		}
		if ((fb[0] < -2.5) || (fb[0] > 2.5) || (fb[2] < -2.5) || (fb[2] > 2.5)) {
			fireball = false;
		}
	}

	for (int i = 0; i < 6; ++i) {											// 죽으면 터짐
		if (die[i] == true) {
			boom[i] += 0.05;
		}
		if ((i != 0) && boom[i] >= 1) {
			mx[i] = uid(dre);
			my[i] = 0;
			mz[i] = uid(dre);
			clone[i - 1] = 0;
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

	if (boom[0] >= 1) {
		boom[0] = 0;
		for (int i = 0; i < 4; ++i) {
			boxx[i] = uid(dre);
			boxy[i] = uid(dre);
		}

		for (int i = 0; i < 12; ++i) {										// (0,1) (2,3) (4,5) (6,7) (8,9) (10,11) = 캐릭터
			if (i < 6) {
				walk[i] = false;
				die[i] = false;
				walk2[i] = false;
				jump[i] = false;
				savey[i] = -0.5;
				dir[i] = 1;
				mx[i] = uid(dre);
				my[i] = 0;
				mz[i] = uid(dre);;
				leg[i] = 0;
				turn[i] = 0;
				if (i < 5) {
					clone[i] = 0;
					makelegX[i] = 0;
					makelegY[i] = 0;
					makehead[i] = 0;
					makearmX[i] = 0;
					makearmY[i] = 0;
					makenose[i] = 0;
					for (int j = 0; j < 4; ++j) {
						movecount[i][j] = 0;
					}

				}
			}
		}
		cx = 0;
		cz = 0;
		ry = 0;

		clonecount = 1;
		turnY2 = 0;
		boxturn = 0;
		num = 0;
		zsize = 1;
		dieing = 0;

	}

	for (int i = 1; i < 6; ++i) {					// 로봇 따라가기
		if (clone[i - 1] == 2) {

			walk[i] = true;

			if ((mx[i] <= mx[0])) {
				mx[i] += clonespeed[i - 1];
				dir[i] = 1;
			}

			if ((mz[i] <= mz[0])) {
				mz[i] += clonespeed[i - 1];
				dir[i] = 4;

			}

			if ((mx[i] >= mx[0])) {
				mx[i] -= clonespeed[i - 1];
				dir[i] = 2;

			}

			if ((mz[i] >= mz[0])) {
				mz[i] -= clonespeed[i - 1];
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

		for (int j = 0; j < 4; ++j) {																			// 플레이어와 장애물
			if ((mx[0] - 0.025 < boxx[j] + 0.05) && (mz[0] - 0.015 < boxy[j] + 0.05) &&
				(mx[0] + 0.025 > boxx[j] - 0.05) && (mz[0] + 0.015 > boxy[j] - 0.05)) {
				die[0] = true;
			}
		}

		for (int j = 1; j < 6; ++j) {																			// 플레이어와 좀비
			if ((mx[0] - 0.025 < mx[j] + 0.025) && (mz[0] - 0.015 < mz[j] + 0.015) &&
				(mx[0] + 0.025 > mx[j] - 0.025) && (mz[0] + 0.015 > mz[j] - 0.015)) {
				if (clone[j - 1] == 2) {
					die[0] = true;
				}

			}
		}

		if (i != 0 && clone[i - 1] == 0) {																		// 플레이어와 비활성 좀비
			if ((mx[0] - 0.025 < mx[i] + 0.05) && (mz[0] - 0.015 < mz[i] + 0.05) &&
				(mx[0] + 0.025 > mx[i] - 0.05) && (mz[0] + 0.015 > mz[i] - 0.05)) {
				clone[i - 1] = 1;
			}

		}

		if (i != 0 && clone[i - 1] == 2) {																		// 좀비와 파이어볼
			if ((mx[i] - 0.025 < fb[0] + 0.05) && (mz[i] - 0.015 < fb[2] + 0.05) &&
				(mx[i] + 0.025 > fb[0] - 0.05) && (mz[i] + 0.015 > fb[2] - 0.05) && fireball == true) {
				die[i] = true;
				fireball = false;
			}

		}

		if (jump[i] == true) {
			my[i] += 0.01;

			if (my[i] > -0.2) {
				jump[i] = false;
			}
		}

		else {
			my[i] -= 0.015;
			if (my[i] < savey[i]) {
				my[i] = savey[i];
			}
		}

		walk[i] = false;
	}

	boxturn += 2;
	if (Time) {
		if (key['w'] == true && mz[0] > -2.5) {						// 위로 이동
			walk[0] = true;

			if (fps == true) {
				mx[0] += sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
				mz[0] -= cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
			}
			else {
				mx[0] += sin((float)glm::radians(ry)) * 0.015;
				mz[0] -= cos((float)glm::radians(ry)) * 0.015;
			}

			dir[0] = 3;
		}
		if (key['s'] == true && mz[0] < 2.5) {						// 아래로 이동
			walk[0] = true;
			if (fps == true) {
				mx[0] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
				mz[0] += cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
			}
			else {
				mx[0] -= sin((float)glm::radians(ry)) * 0.015;
				mz[0] += cos((float)glm::radians(ry)) * 0.015;
			}

			dir[0] = 4;
		}
		if (key['a'] == true && mx[0] > -2.5) {						// 왼쪽으로 이동
			walk[0] = true;
			if (fps == true) {
				mx[0] -= cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
				mz[0] -= sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
			}
			else {
				mx[0] -= cos((float)glm::radians(ry)) * 0.015;
				mz[0] -= sin((float)glm::radians(ry)) * 0.015;
			}
			dir[0] = 2;
		}
		if (key['d'] == true && mx[0] < 2.5) {						// 오른쪽으로 이동
			walk[0] = true;
			if (fps == true) {
				mx[0] += cos((float)glm::radians(fpsy + fpsy2)) * 0.015;
				mz[0] += sin((float)glm::radians(fpsy + fpsy2)) * 0.015;
			}
			else {
				mx[0] += cos((float)glm::radians(ry)) * 0.015;
				mz[0] += sin((float)glm::radians(ry)) * 0.015;
			}
			dir[0] = 1;
		}
		if (key['a'] == false && key['w'] == false && key['s'] == false && key['d'] == false) {
			dir[0] = 0;
		}
		if (key[' '] == true) {						// 점프
			jump[0] = true;
		}
		if (key['['] == true) {						// 조명 공전 ( 양 )
			turnY2++;
		}
		if (key[']'] == true) {						// 조명 공전 ( 음 )
			turnY2--;
		}
		if (key['='] == true) {						// 조명 밝게 하기
			for (int i = 0; i < 3; ++i) {
				color2[i] += 0.1;
				if (color2[i] > 1.0) {
					color2[i] = 1.0;
				}
			}
		}
		if (key['-'] == true) {						// 조명 어둡게 하기
			for (int i = 0; i < 3; ++i) {
				color2[i] -= 0.1;
				if (color2[i] < 0.0) {
					color2[i] = 0.0;
				}
			}
		}
		if (key['y'] == true) {						// 카메라 공전 (양)
			ry++;
		}
		if (key['Y'] == true) {						// 카메라 공전 (음)
			ry--;
		}
		if (key['g'] == true) {						// 카메라 뒤로 이동
			cz += 0.02;
		}
		if (key['t'] == true) {						// 카메라 앞으로 이동
			cz -= 0.02;
		}
		if (key['h'] == true) {						// 카메라 오른쪽 이동
			cx += 0.02;
		}
		if (key['f'] == true) {						// 카메라 왼쪽 이동
			cx -= 0.02;
		}

		if (key['1'] == true) {						// 1인칭
			fps = true;
		}
		if (key['3'] == true) {						// 3인칭
			fps = false;
		}
		if (key['i'] == true) {						// 위 공격
			if (fireball == false) {
				fireball = true;
				fb[0] = mx[0];
				fb[1] = my[0] + 0.1;
				fb[2] = mz[0];
				fb[3] = 3;
			}
		}
		if (key['k'] == true) {						// 아래 공격
			if (fireball == false) {
				fireball = true;
				fb[0] = mx[0];
				fb[1] = my[0] + 0.1;
				fb[2] = mz[0];
				fb[3] = 4;
			}
		}
		if (key['j'] == true) {						// 좌 공격
			if (fireball == false) {
				fireball = true;
				fb[0] = mx[0];
				fb[1] = my[0] + 0.1;
				fb[2] = mz[0];
				fb[3] = 2;
			}
		}
		if (key['l'] == true) {						// 우 공격
			if (fireball == false) {
				fireball = true;
				fb[0] = mx[0];
				fb[1] = my[0] + 0.1;
				fb[2] = mz[0];
				fb[3] = 1;
			}
		}
		if (key['q'] == true) {						// 프로그램 종료
			glutLeaveMainLoop();
		}

		glutPostRedisplay();
	}

	glutTimerFunc(10, TimerFunction, 1);

}




BOOL CrossCheckfor4p(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22) {
	BOOL b1, b2;

	if (((x12 - x11) * (y21 - y11) - (y12 - y11) * (x21 - x11)) * ((x12 - x11) * (y22 - y11) - (y12 - y11) * (x22 - x11)) < 0) {
		b1 = TRUE;
	}
	else {
		b1 = FALSE;
	}

	if (((x22 - x21) * (y11 - y21) - (y22 - y21) * (x11 - x21)) * ((x22 - x21) * (y12 - y21) - (y22 - y21) * (x12 - x21)) < 0) {
		b2 = TRUE;
	}
	else {
		b2 = FALSE;
	}

	if (b1 && b2) {
		return TRUE;
	}
	return FALSE;
}

GLchar* filetobuf(const char* file) {
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

void ReadObj(FILE* objFile)
{
	faceNum = 0;
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	char bind[100];
	int vertexNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count));
	}
	int vertIndex = 0;
	int faceIndex = 0;
	vertex = (glm::vec4*)malloc(sizeof(glm::vec4) * vertexNum);
	face = (glm::vec4*)malloc(sizeof(glm::vec4) * faceNum);

	fseek(objFile, 0, 0);
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&face[faceIndex].x, &face[faceIndex].y, &face[faceIndex].z);
			int x = face[faceIndex].x - 1, y = face[faceIndex].y - 1, z = face[faceIndex].z - 1;
			faceIndex++;
		}
	}
}

void vectoplane(Plane* p) {
	for (int i = 0; i < faceNum; ++i) {
		int x = face[i].x - 1, y = face[i].y - 1, z = face[i].z - 1;
		p[i].p[0] = vertex[x].x;
		p[i].p[1] = vertex[x].y;
		p[i].p[2] = vertex[x].z;

		p[i].p[3] = vertex[y].x;
		p[i].p[4] = vertex[y].y;
		p[i].p[5] = vertex[y].z;

		p[i].p[6] = vertex[z].x;
		p[i].p[7] = vertex[z].y;
		p[i].p[8] = vertex[z].z;
	}
}

void planecolorset(Plane* p, int a) {
	if (a == 0) {										// 랜덤 색
		for (int i = 0; i < faceNum; i += 2) {
			for (int j = 0; j < 3; ++j) {
				p[i].color[j * 3] = 1.0;
				p[i].color[j * 3 + 1] = 1.0;
				p[i].color[j * 3 + 2] = 1.0;

				p[i + 1].color[j * 3] = 1.0;
				p[i + 1].color[j * 3 + 1] = 1.0;
				p[i + 1].color[j * 3 + 2] = 1.0;
			}
		}
		for (int i = 0; i < faceNum; ++i) {
			if (i % 2 == 0) {
				p[i].tex[0] = 0.0;
				p[i].tex[1] = 1.0;
				p[i].tex[2] = 0.0;
				p[i].tex[3] = 0.0;
				p[i].tex[4] = 1.0;
				p[i].tex[5] = 1.0;
			}
			else {
				p[i].tex[0] = 1.0;
				p[i].tex[1] = 0.0;
				p[i].tex[2] = 1.0;
				p[i].tex[3] = 1.0;
				p[i].tex[4] = 0.0;
				p[i].tex[5] = 0.0;
			}
		}
	}
	if (a == 1) {										// 하얀 색
		for (int i = 0; i < faceNum; i += 2) {
			for (int j = 0; j < 3; ++j) {
				p[i].color[j * 3] = 1.0;
				p[i].color[j * 3 + 1] = 1.0;
				p[i].color[j * 3 + 2] = 1.0;

				p[i + 1].color[j * 3] = 1.0;
				p[i + 1].color[j * 3 + 1] = 1.0;
				p[i + 1].color[j * 3 + 2] = 1.0;
			}
		}
		for (int i = 0; i < faceNum; ++i) {
			if (i % 2 == 0) {
				p[i].tex[0] = 0.0;
				p[i].tex[1] = 1.0;
				p[i].tex[2] = 0.0;
				p[i].tex[3] = 0.0;
				p[i].tex[4] = 1.0;
				p[i].tex[5] = 1.0;
			}
			else {
				p[i].tex[0] = 1.0;
				p[i].tex[1] = 0.0;
				p[i].tex[2] = 1.0;
				p[i].tex[3] = 1.0;
				p[i].tex[4] = 0.0;
				p[i].tex[5] = 0.0;
			}
		}
	}
	if (a == 2) {										// 하얀 색
		for (int i = 0; i < faceNum; i += 2) {
			for (int j = 0; j < 3; ++j) {
				p[i].color[j * 3] = 1.0;
				p[i].color[j * 3 + 1] = 0.5;
				p[i].color[j * 3 + 2] = 0.0;

				p[i + 1].color[j * 3] = 1.0;
				p[i + 1].color[j * 3 + 1] = 0.5;
				p[i + 1].color[j * 3 + 2] = 0.0;
			}
		}
	}
}






