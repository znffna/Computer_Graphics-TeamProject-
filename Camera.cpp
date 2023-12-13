#include "Camera.hpp"


Camera camera;

Camera::Camera()
{
	lat = 0.0f;

	camera_rotate = { 0.0f, 0.0f, 0.0f };

	//뷰 변환용 카메라 파라미터
	cameraPos = glm::vec3(0.0f, 0.0f, 20.0f); //--- 카메라 위치
	cameraDirection = {0.0f, 0.0f, -1.0f}; //--- 카메라 바라보는 방향
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

	//투영 변환용 카메라 파라미터
	fovy = 45.0f;
	aspect = static_cast<float> (window_row) / static_cast<float>(window_col);
	n = 0.05f;
	f = 200.0f;
	//p_t = glm::vec3(0.0f, 0.0f, 0.0f);
	sensitivity = 10.0f;
}

void Camera::reset()
{
	lat = 0.0f;

	camera_rotate = { 0.0f, 0.0f, 0.0f };

	//뷰 변환용 카메라 파라미터
	cameraPos = glm::vec3(0.0f, 10.0f, 20.0f); //--- 카메라 위치
	cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f); //--- 카메라 바라보는 방향
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향


	//투영 변환용 카메라 파라미터
	fovy = 45.0f;
	aspect = static_cast<float> (window_row) / static_cast<float>(window_col);
	n = 0.05f;
	f = 200.0f;
	//p_t = glm::vec3(0.0f, 0.0f, 0.0f);
	sensitivity = 10.0f;
}


void Camera::show_state() const
{
	std::cout << "--------------카메라 외부 파라미터--------------------------" << '\n';
	std::cout << "cameraPos : {" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << "} " << '\n';
	std::cout << "cameraDir : {" << cameraDirection.x << ", " << cameraDirection.y << ", " << cameraDirection.z << "} " << '\n';
	std::cout << "cameraUp : {" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << "} " << '\n';
	//std::cout << "P_t : {" << p_t.x << ", " << p_t.y << ", " << p_t.z << "} " << '\n';
	std::cout << "-----------------------------------------------------" << '\n';
}

void Camera::World_transform(glm::mat4& transformMatrix) const
{
	transformMatrix = glm::translate(transformMatrix, cameraPos); //--- 이동(Translation)	
}


void Camera::setPos(const glm::vec3& pos)
{
	cameraPos = pos;
}
void Camera::setPos(const int idx, const float value)
{
	cameraPos[idx] = value;
}
void Camera::setDir(const glm::vec3& dir)
{
	cameraDirection = dir;
}
void Camera::setUp(const glm::vec3& up)
{
	cameraUp = up;
}


glm::vec3 Camera::getPos() const
{
	//std::cout << "CameraPos = {" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << "} " << '\n';
	return cameraPos;
}
glm::vec3 Camera::getDir() const
{
	return cameraDirection;
}
glm::vec3 Camera::getUp() const
{
	return cameraUp;
}

GLfloat Camera::getSensitivity() const
{
	return sensitivity;
}


void Camera::trans_Pos_x(const float& value)
{
	cameraPos.x += value;
}
void Camera::trans_Pos_y(const float& value)
{
	cameraPos.y += value;
}
void Camera::trans_Pos_z(const float& value)
{
	cameraPos.z += value;
}

void Camera::trans_Dir_x(const float& value)
{
	cameraDirection.x += value;
}
void Camera::trans_Dir_y(const float& value)
{
	cameraDirection.y += value;
}
void Camera::trans_Dir_z(const float& value)
{
	cameraDirection.z += value;
}


void Camera::rotate_Pos_x(const float& degree)
{
	glm::mat4 transform{ 1.0f };
	transform = glm::rotate(transform, glm::radians(degree), x_axis);
	cameraPos = transform * glm::vec4{ cameraPos, 1.0f };
}

void Camera::rotate_Pos_y(const float& degree)
{
	glm::mat4 transform{ 1.0f };
	transform = glm::translate(transform, cameraDirection);
	transform = glm::rotate(transform, glm::radians(degree), y_axis);
	transform = glm::translate(transform, glm::vec3{ -1.0f } * cameraDirection);
	cameraPos = transform * glm::vec4{ cameraPos, 1.0f };
}

void Camera::rotate_Pos_z(const float& degree)
{
	glm::mat4 transform{ 1.0f };
	transform = glm::rotate(transform, glm::radians(degree), z_axis);
	cameraPos = transform * glm::vec4{ cameraPos, 1.0f };
}

void Camera::rotate_Dir_y(const float& degree)
{
	//camera_rotate.y += degree;
	//degree_range_normalization(camera_rotate.y);
	glm::mat4 transform{ 1.0f };
	glm::vec3 initialDirection = cameraDirection - cameraPos;
	//glm::vec3 initialDirection = glm::normalize(cameraDirection - cameraPos);

	// 원하는 회전 각도 (라디안 단위)
	float rotationAngleRadians = glm::radians(degree);

	// Y축 기준 회전을 나타내는 쿼터니언 생성
	glm::quat rotationQuat = glm::angleAxis(rotationAngleRadians, glm::vec3(0.0f, 1.0f, 0.0f));

	// 초기 방향 벡터를 쿼터니언으로 회전
	glm::vec3 rotatedDirection = rotationQuat * initialDirection;

	// 새로운 바라보는 방향 벡터를 얻기 위해 EYE에 더함
	glm::vec3 newAT = cameraPos + rotatedDirection;
	cameraDirection = newAT;
}

void Camera::rotate_Up(const float& degree)
{
	//camera_rotate.y += degree;
	//degree_range_normalization(camera_rotate.y);
	glm::mat4 transform{ 1.0f };
	glm::vec3 initialDirection = glm::normalize(cameraDirection - cameraPos);
	//glm::vec3 initialDirection = glm::normalize(cameraDirection - cameraPos);

	// 원하는 회전 각도 (라디안 단위)
	float rotationAngleRadians = glm::radians(degree);

	// Y축 기준 회전을 나타내는 쿼터니언 생성
	glm::quat rotationQuat = glm::angleAxis(rotationAngleRadians, initialDirection);

	// 초기 방향 벡터를 쿼터니언으로 회전
	glm::vec3 rotatedDirection = rotationQuat * cameraUp;

	// 새로운 바라보는 방향 벡터를 얻기 위해 EYE에 더함
	cameraUp = rotatedDirection;
}

void Camera::Pos_translation(const glm::vec3& vector)
{
	trans_Pos_x(vector.x);
	trans_Pos_y(vector.y);
	trans_Pos_z(vector.z);
}

void Camera::Dir_rotate(const glm::vec3& degrees)
{
	camera_rotate += degrees;
	camera_rotate.x = glm::clamp(camera_rotate.x, -90.0f + 1.0f, 90.0f - 1.0f);

	glm::quat rotation = glm::angleAxis(glm::radians(camera_rotate.y), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(camera_rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));

	cameraDirection = glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
	return;
}

void Camera::Pos_scale(const glm::vec3& degrees)
{
	glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, degrees);
	cameraPos = glm::mat3{ transform } * cameraPos;
	//cameraPos.z = cameraPos.x > 0? 1 - cameraPos.x : cameraPos.x ;
}

void Camera::zoom_in(const float len) {
	glm::vec3 dir = glm::normalize(cameraPos - cameraDirection);
	if (glm::length(cameraPos) > 1.0f + len) {
		cameraPos -= glm::vec3{ len } *dir;
	}
}

void Camera::zoom_out(const float len) {
	glm::vec3 dir = glm::normalize(cameraPos - cameraDirection);
	cameraPos += glm::vec3{ len } * dir;
	
}

void Camera::focus(const glm::vec3& focus)
{
	cameraDirection = focus;
}

void Camera::adj_sensitivity(const float& value)
{
	sensitivity = value;
}


glm::mat4 Camera::View_transform() const {
	return  glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

glm::mat4 Camera::perspective_transform() const {
	glm::mat4 perspective{ 1.0f };
	//perspective = glm::scale(perspective, glm::vec3{ 1.0f, 1.0f, -1.0f });
	perspective = glm::perspective(glm::radians(fovy), aspect, n, f);
	//perspective = glm::translate(perspective, p_t);
	return perspective;
}

glm::mat4 Camera::ortho_transform() const {
	float max = 1.0f;
	glm::mat4 ortho{ 1.0f };
	ortho = glm::ortho(-max, max, -max, max, -10.0f, 10.0f); //--- 투영 공간 설정: [-100.0, 100.0]
	return ortho;
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
}

