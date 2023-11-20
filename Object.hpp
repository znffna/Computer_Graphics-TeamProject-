#include "Default.hpp"
#include "Mesh.hpp"


#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
private:
	void World_after_Scale(glm::mat4&) const;
	void World_after_Translation(glm::mat4&) const;
	void World_after_rotate(glm::mat4&) const;
	void World_after_rotate_origin(glm::mat4&) const;
public:
	//���� ��ȯ
	GLfloat radius;	//�̵� ���Ϳ� ���̸� ����	(1.0 = ���� ��ǥ�� �̵�)
	glm::vec3 after_translation{ 0.0f };			//�̵� ����
	glm::vec3 after_rotate{ 0.0f };			//ȸ�� ����(�� �࿡���� degree��)
	glm::vec3 after_scale{ 1.0f };			//���/Ȯ�� ����
	
	glm::vec3 after_rotate_origin{ 0.0f };			//���� �߽� ȸ�� ����

	glm::vec3 translation{ 0.0f };	//�̵� ����
	glm::vec3 rotate{ 0.0f };			//ȸ�� ����
	glm::vec3 scale{ 1.0f };			//���/Ȯ�� ����

	glm::vec3 own_point{ 0.0f };	//���ο� ������(Ŀ���ҿ�)

	glm::vec3 color{ 1.0f };		//������Ʈ ��� ����

	//����� 3D ������Ʈ
	Mesh mesh;		//�ش� ������Ʈ�� ������ 3D ��ü �ѹ�

	//�ִϸ��̼� ī��Ʈ
	int t;	//�̵��� ��ġ
	int depth;
	//---��� �Լ�
	
	//--- Ư���� ��Ȳ 
	Object();	//������
	~Object();

	Object(const Object& other);
	Object& operator=(const Object& other);

	Object(Object&& other) noexcept;
	Object& operator=(Object&& other) noexcept;
	//---


	//Object ���� �ʱ�ȭ
	void changemesh(const int& number);
	void reset();
	void reset(const int& polygon);
	void transform_reset();
	//���� ���
	void show_state() const;

	//World Transform ���� �Լ�
	void setTranslation(const glm::vec3& vector);
	void setRotate(const glm::vec3& vector);
	void setScale(const glm::vec3& vector);

	void addTranslation_x(const float& );
	void addTranslation_y(const float& );
	void addTranslation_z(const float& );

	void addAfter_Translation_x(const float&);
	void addAfter_Translation_y(const float&);
	void addAfter_Translation_z(const float&);

	void addRotate_x(const float& );
	void addRotate_y(const float& );
	void addRotate_z(const float& );

	void addAfter_Rotate_x(const float&);
	void addAfter_Rotate_y(const float&);
	void addAfter_Rotate_z(const float&);

	void addAfter_Rotate_origin_x(const float&);
	void addAfter_Rotate_origin_y(const float&);
	void addAfter_Rotate_origin_z(const float&);

	void addScale_x(const float& );
	void addScale_y(const float& );
	void addScale_z(const float& );

	
	void translation_rotate(const glm::vec3&);

	//�� �������� ���� �ӽ� method
	void setPoint(const glm::vec3&);
	bool Collision(const Object& other) const ;

	//World Transform ���� �Լ�
	void World_Transform(glm::mat4&) const;
	void Normal_Transform(glm::mat4&) const;
	const GLuint& getVao() const;

	// ���� object�� ���
	void draw() const;

};

class Axis : public Object {
public:
	Axis();

	void draw() const;
};

#endif