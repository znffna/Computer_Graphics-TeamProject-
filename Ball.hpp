#include "Object.hpp"

class Ball : public Object {
	float velocity{ 5.0f };		// �¿� �̵� �ӵ�(rad)
	int move_dir{ 0 };			// �¿� �̵� ����(0: �̵�����)

	float fall_velocity{ 0.0f };// �������� �ӵ� (����)
	float fall_accelation{ 0.2f };// �߷¿���
public:
	Ball() : Object(SPHERE) { backup(); }
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
			fall_velocity = 1.0f;
		}
		if (group == "Ball:Cube") {
			//TODO �������� ȹ��
		}
	}

};
