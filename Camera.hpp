#include "Default.hpp"
#include "Object.hpp"

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera {
	float lat; // ī�޶� y��� ����������� ���밪�� Ŀ��(90.0f = y��� ����)
	float radius;
	//ī�޶� �ܺ� �Ķ����
	glm::vec3 cameraPos; //--- ī�޶� ��ġ
	glm::vec3 cameraDirection; //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp; //--- ī�޶� ���� ����
	//ī�޶� ���� �Ķ����	
	GLfloat fovy;	//--- �����þ߰�
	GLfloat aspect;	//--- ��Ⱦ��	
	GLfloat n;	//--- near	
	GLfloat f;	//--- far
	//�׿� �Ķ����
	//glm::vec3 p_t;	//���� ������ ���� �̵�

	//ī�޶� ���� �ӵ� ����
	GLfloat sensitivity;
public:
	Camera();

	void reset();

	glm::vec3 camera_rotate;
	void show_state() const;
	//ī�޶� �Ķ���� ����
	//�ܺ� �Ķ����
	glm::vec3 getPos() const;
	glm::vec3 getDir() const;
	glm::vec3 getUp() const;

	void setPos(const glm::vec3&);
	void setPos(const int , const float );
	void setDir(const glm::vec3&);
	void setUp(const glm::vec3&);

	//���� ����
	GLfloat getSensitivity() const;

	//ī�޶� ��ġ ����
	void trans_Pos_x(const float&);
	void trans_Pos_y(const float&);
	void trans_Pos_z(const float&);

	void trans_Dir_x(const float&);
	void trans_Dir_y(const float&);
	void trans_Dir_z(const float&);

	void rotate_Pos_x(const float&);
	void rotate_Pos_y(const float&);
	void rotate_Pos_z(const float&);

	void rotate_Dir_y(const float&);
	void rotate_Up(const float& );

	void Pos_translation(const glm::vec3&);
	void Dir_rotate(const glm::vec3&);
	void Pos_scale(const glm::vec3&);

	void movePos(unsigned char key) {
		switch (key) {
		case 'w': case 'W':
			cameraPos += cameraDirection;
			break;
		case 's': case 'S':
			cameraPos -= cameraDirection;
			break;
		case 'd': case 'D':
			cameraPos += glm::cross(cameraDirection, cameraUp);
			break;
		case 'a': case 'A':
			cameraPos -= glm::cross(cameraDirection, cameraUp);
			break;
		}
	}

	void zoom_in(const float len);
	void zoom_out(const float len);

	//�ٶ󺸴� ���� ����
	void focus(const glm::vec3&);

	void adj_sensitivity(const float&);

	void addP_t_z(const float&);

	//ī�޶� ��ġ ��¿�
	void World_transform(glm::mat4&) const;

	glm::mat4 View_transform() const;
	glm::mat4 perspective_transform() const;
	glm::mat4 ortho_transform() const;
};

#endif