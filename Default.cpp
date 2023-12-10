#include "Default.hpp"

float window_row = 800;			//윈도우 크기(가로)
float window_col = 600;			//윈도우 크기(세로)
// 사운드 관련 연습용 코드
FMOD::System* ssystem;	//Sound System 약자임.
FMOD::Sound* sound1, * sound2;	// 사용할 사운드가 동적할당될텐데 그걸 가르키는 포인터
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
		// 오류 처리 또는 오류 메시지 출력
		std::cerr << "OpenGL 오류 발생: " << error << std::endl;
		return true;
	}
	return false;
}