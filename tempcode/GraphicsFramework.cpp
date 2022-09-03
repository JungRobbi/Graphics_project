#include "GraphicsFramework.h"
#include "stb_image.h"

void GraphicsFramework::run() // 한 프레임 출력
{
	if (!scenes.empty())
		scenes.front()->scene->update();
}
void GraphicsFramework::InitBuffer()
{
	glGenVertexArrays(10, VAO::vao);
	glGenBuffers(10, VBO::vbo_position);
	glGenBuffers(10, VBO::vbo_normal);
	glGenBuffers(10, VBO::vbo_uv);
}
void GraphicsFramework::InitTexture()
{
	//BITMAPINFO* bmp;
	std::string map[20] = { "Resource/main.png","Resource/B.png","Resource/C.png",
		"Resource/D.png","Resource/E.png","Resource/body.png","Resource/face.png","Resource/gun_tex.png" };
	glGenTextures(20, Texture::texture); //--- 텍스처 생성

	for (int i = 0; i < 20; ++i) {
		glBindTexture(GL_TEXTURE_2D, Texture::texture[i]); //--- 텍스처 바인딩
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
void GraphicsFramework::InitBuffer_bind(const int street)
{
	if (street == 0) {
		num_Triangle = loadObj_normalize_center_3f("Resource/cube.obj");
	}
	else if (street == 1) {
		num_Tank = loadObj_normalize_center_4f("Resource/tank.obj");
	}
	else if (street == 2) {
		num_Tong = loadObj_normalize_center_4f("Resource/sohwajeon.obj");
	}
	else if (street == 3) {
		num_Sphere = loadObj_normalize_center_3f("Resource/sphere.obj");
	}
	else if (street == 4) {

	}

	glUseProgram(s_program[0]);
	glBindVertexArray(VAO::vao[street]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO::vbo_position[street]);
	glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program[0], "aPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO::vbo_normal[street]);
	glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program[0], "aNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO::vbo_uv[street]);
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
void GraphicsFramework::Initialize()
{
	InitBuffer();
	InitTexture();
	InitBuffer_bind(0);
	InitBuffer_bind(1);
	InitBuffer_bind(2);
	InitBuffer_bind(3);
	glEnable(GL_DEPTH_TEST);
}