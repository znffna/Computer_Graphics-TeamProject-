#include "Default.hpp"


// image loader header include
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool perspective{ true };	//���� ���(true : ���� ����)

float window_row = 800;			//������ ũ��(����)
float window_col = 600;			//������ ũ��(����)
// ���� ���� ������ �ڵ�
FMOD::System* ssystem;	//Sound System ������.
FMOD::Sound* sound1, * sound2;	// ����� ���尡 �����Ҵ���ٵ� �װ� ����Ű�� ������
FMOD::Channel* channel = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

void show_vec3(const glm::vec3& vector) {
	std::cout << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";
}

float random_number(const float& min, const float& max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> urd(min, max);

	return urd(gen);
}

int random_number(const int& min, const int& max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> urd(min, max);

	return urd(gen);
}

float degree_range_normalization(const float& degree) {
	float tmp = degree;
	while (tmp < 0.0f) tmp += 360.0f;
	while (360.0f <= tmp) tmp -= 360.0f;
	return tmp;
}

glm::vec3 CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	glm::vec3 p = uu * p0; // (1 - t)^2 * P0
	p += 2 * u * t * p1; // 2 * (1 - t) * t * P1
	p += tt * p2; // t^2 * P2
	return p;
}

void print_vec3(const glm::vec3& vector) {
	std::cout << vector.x << ", " << vector.y << ", " << vector.z << '\n';
}

bool opengl_error() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		// ���� ó�� �Ǵ� ���� �޽��� ���
		std::cerr << "OpenGL ���� �߻�: " << error << std::endl;
		return true;
	}
	return false;
}

GLuint loadTexture(const char* imagePath)
{
	// �̹��� ������ �ε�
	int width, height, channels;
	unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);
	if (!imageData) {
		std::cerr << "Failed to load image: " << imagePath << std::endl;
		return 0;
	}

	// OpenGL �ؽ�ó ����
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// �ؽ�ó �Ķ���� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �ؽ�ó ������ ����
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// �̹��� ������ ����
	stbi_image_free(imageData);

	return textureID;
}