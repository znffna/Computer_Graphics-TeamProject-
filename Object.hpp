#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Default.hpp"
#include "Mesh.hpp"

class Object : public std::enable_shared_from_this<Object>{
private:
	void World_after_Scale(glm::mat4&) const;
	void World_after_Translation(glm::mat4&) const;
	void World_after_rotate(glm::mat4&) const;

	bool invaild{ false }; // true�Ͻ� game_world�� ��� �κп��� ����.

	glm::vec3 after_translation{ 0.0f };			//�̵� ����
	glm::vec3 after_rotate{ 0.0f };			//ȸ�� ����(�� �࿡���� degree��)
	glm::vec3 after_scale{ 1.0f };			//���/Ȯ�� ����

	glm::vec3 translation{ 0.0f };	//�̵� ����
	glm::vec3 rotate{ 0.0f };			//ȸ�� ����
	glm::vec3 scale{ 1.0f };			//���/Ȯ�� ����

	glm::vec3 color{ 1.0f };		//������Ʈ ��� ����

	// ����� 3D ������Ʈ
	std::shared_ptr<Mesh> mesh{ nullptr };

public: // ---- ��� �Լ� ----	
	//--- special Function
	Object(const std::shared_ptr<Mesh>&);	//������
	virtual ~Object();

	Object(const Object& other);
	Object& operator=(const Object& other);

	Object(Object&& other) noexcept;
	Object& operator=(Object&& other) noexcept;

	//--- operator overloading


	// Object ���� �ʱ�ȭ
	void changemesh(const std::shared_ptr<Mesh>&);
	void transform_reset();

	// ���� ���
	void show_state() const;

	// interface function
	void setInvaild(bool rhs) { invaild = rhs; }
	bool getInvaild() { return invaild; }

	void setTranslation(const glm::vec3& vector);
	void setRotate(const glm::vec3& vector);
	void setScale(const glm::vec3& vector);	

	void addTranslation(const glm::vec3& vector);
	void addRotate(const glm::vec3& vector);
	void addScale(const glm::vec3& vector);


	glm::vec3 getTranslation() const ;
	glm::vec3 getRotate() const;
	glm::vec3 getScale() const;
	// ���� ���� �Լ� getter / setter
	void setColor(const glm::vec3&);
	glm::vec3 getColor() const;

	// Position ���� �Լ�.
	void translation_rotate(const glm::vec3&);

	

	//�� �������� ���� �ӽ� method
	bool Collision(const Object& other) const ;

	//World Transform ���� �Լ�
	void World_Transform(glm::mat4&) const;
	void Normal_Transform(glm::mat4&) const;

	// ���� object�� ���
	void draw() const;

	// �������� ���� �⺻ �ɼ�
	void render() const;
	virtual void update();
	virtual void handle_collision(const std::string& group, const std::shared_ptr<Object>& other);
};

extern std::shared_ptr<Object> NULLPTR;

class Ball : public Object {
	float velocity{ 5.0f };		// �¿� �̵� �ӵ�(rad)
	int move_dir{ 0 };			// �¿� �̵� ����(0: �̵�����)

	float fall_velocity{ 0.0f };// �������� �ӵ� (����)
	float fall_accelation{ 0.2f };// �߷¿���
public:
	Ball(): Object(SPHERE) {}
	// interface function
	float getVelocity() { return velocity; }
	void setVelocity(float rhs) { velocity = rhs; }

	float getFallVelocity() { return fall_velocity; }
	void setFallVelocity(float rhs) { fall_velocity = rhs; }

	float getFallAccelation() { return fall_accelation; }
	void setFallAccelation(float rhs) { fall_accelation = rhs; }

	// method

	// �¿� ȸ�� �̵�
	void horizontal_move() {	
		Object::translation_rotate({ 0.0f,  move_dir * velocity, 0.0f });
	}

	// vertical_move( ���� ���� � )
	void falling() {
		fall_velocity -= fall_accelation; // �ð������� �ȳ־���.
		addTranslation({ 0.0f, fall_velocity, 0.0f });
	}

	void update() override {
		//std::cout << "Ball.update() ȣ��" << '\n';
		horizontal_move();	// �¿� �̵� ����
		falling();	// ���� �̵� ����
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			//TODO ���� �ӵ��� �ʱ�ȭ
			fall_velocity = 2.0f;
		}
		if (group == "Ball:Cube") {
			//TODO �������� ȹ��
		}
	}

};

class Cube : public Object{
public:
	Cube(glm::vec3 position, glm::vec3 rotate, glm::vec3 scale) : Object(CUBE) {
		Object::setTranslation(position);
		Object::setRotate(rotate);
		Object::setScale(scale);
	}

	void update() override {
		//TODO �������� ��� ȸ����Ŵ.
		Object::addRotate({ 0.0f, 5.0f, 0.0f });
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "ball:cube") {
			//std::shared_ptr<Object> tmp = shared_from_this();
			setInvaild(true);
			//world.remove_object(tmp);
		}
	}
};

class Pillar : public Object {
public:
	Pillar(glm::vec3 scale) : Object(PILLAR) {
		Object::setScale(scale);
	}

	void update() override {
		// ��� ����� �ƹ��͵� ���� ����.
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		// ��� ����� �ƹ��͵� ���� ����.
	}
};




#endif