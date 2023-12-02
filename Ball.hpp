#include "Object.hpp"

#ifndef BALL_HPP
#define BALL_HPP

class Ball : public Object {
	float velocity{ 5.0f };		// �¿� �̵� �ӵ�(rad)
	int move_dir{ 0 };			// �¿� �̵� ����(0: �̵�����)

	float reset_velocity{ 0.5f };	// ���� �΋H�� ��� �ʱ�ȭ�� ��.
	float fall_velocity{ 0.0f };	// �������� �ӵ� (���ӵ��� ��� �پ�� �� )
	float fall_acceleration{ 0.05f };	// �߷¿��� ( �پ��� ���� ������ �� )
public:
	Ball() : Object(SPHERE) { backup(); }
	// interface function
	float getVelocity() { return velocity; }
	void setVelocity(float rhs) { velocity = rhs; }

	float getFallVelocity() { return fall_velocity; }
	void setFallVelocity(float rhs) { fall_velocity = rhs; }

	float getFallAccelation() { return fall_acceleration; }
	void setFallAccelation(float rhs) { fall_acceleration = rhs; }

	int getDir() { return move_dir; }
	void setDir(int rhs) { move_dir = rhs; }

	// method

	// �¿� ȸ�� �̵�
	void horizontal_move() {
		translation_rotate({ 0.0f,  move_dir * velocity, 0.0f });
		addRotate({ 0.0f,  move_dir * velocity, 0.0f });
	}

	// vertical_move( ���� ���� � )
	void falling() {
		fall_velocity -= fall_acceleration; // �ð������� �ȳ־���.
		addTranslation({ 0.0f, fall_velocity, 0.0f });
	}

	void update() override {
		//std::cout << "Ball.update() ȣ��" << '\n';
		horizontal_move();	// �¿� �̵� ����
		falling();	// ���� �̵� ����
	}

	void render() const override {
		draw();
	}

	void handle_events(unsigned char key) override {
		switch (key) {
		case 'd': case 'D':
			move_dir += 1;
			break;
		case 'a': case 'A':
			move_dir -= 1;
			break;
		case 'r': case 'R':
			rollback();
			break;
		}
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			//TODO ���� �ӵ��� �ʱ�ȭ
			fall_velocity = reset_velocity;
		}
		if (group == "Ball:Cube") {
			//TODO �������� ȹ��
		}
	}

};

#endif // !BALL_HPP