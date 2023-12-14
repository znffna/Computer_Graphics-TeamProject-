#include "Object.hpp"

#ifndef BALL_HPP
#define BALL_HPP

extern FMOD::Sound* pingpong;

const std::string poing_sound{ "dsadasdas.mp3" };

class Ball : public Object {
	float radius{ 5.0f }; // ����� �߽����� ������ �Ÿ�
	float size{ 0.6f };		// ���� ũ��

	float velocity{ 5.0f };		// �¿� �̵� �ӵ�(rad)
	int move_dir{ 0 };			// �¿� �̵� ����(0: �̵�����)

	bool collision_flag{ false };	// ���� scale�� �����϶� true, translate�� �����϶� false;
	float reset_velocity{ 0.4f };	// ���� �΋H�� ��� �ʱ�ȭ�� ��.
	float fall_velocity{ 0.0f };	// �������� �ӵ� (���ӵ��� ��� �پ�� �� )
	float fall_acceleration{ 0.03f };	// �߷¿��� ( �پ��� ���� ������ �� )
public:
	Ball() : Object(SPHERE) {
		setTranslation({ radius, 0.0f, 0.0f });
		setScale(glm::vec3(size));
		backup();
		if(pingpong)
			ssystem->createSound(poing_sound.c_str(), FMOD_DEFAULT, 0, &pingpong);	// FMOD_LOOP_NORMAL(�ݺ� ���) , FMOD_DEFAULT (1�� ���)
	}
	// interface function
	float getVelocity() { return velocity; }
	void setVelocity(float rhs) { velocity = rhs; }

	float getFallVelocity() { return fall_velocity; }
	void setFallVelocity(float rhs) { fall_velocity = rhs; }

	float getFallAccelation() { return fall_acceleration; }
	void setFallAccelation(float rhs) { fall_acceleration = rhs; }

	float getRadius() { return radius; }
	void setRadius(float rhs) { radius = rhs; }

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
		if (collision_flag) {
			glm::vec3 pos = getTranslation() - getScale();
			addScale({ 0.0f, fall_velocity * 0.3f, 0.0f });
			pos += getScale();
			setTranslation(pos);
			if (getScale().y < size / 4.0f) {
				glm::vec3 tmp{ getScale() };
				setScale({ tmp.x, size / 4.0f, tmp.z });
				fall_velocity = -fall_velocity;
			}
			else if (getScale().y > size) {
				glm::vec3 tmp{ getScale() };
				setScale({ tmp.x, size, tmp.z });
				collision_flag = false;
				fall_velocity = reset_velocity;
			}
		}
		else{
			fall_velocity -= fall_acceleration; // �ð������� �ȳ־���.
			fall_velocity = glm::clamp(fall_velocity, -4.0f, 4.0f);
			addTranslation({ 0.0f, fall_velocity, 0.0f });
		}
	}

	void update() override {
		//std::cout << "Ball.update() ȣ��" << '\n';
		horizontal_move();	// �¿� �̵� ����
		falling();	// ���� �̵� ����
	}

	void render() const override {
		draw();
	}

	void handle_events(unsigned char key, const std::string& state) override {
		switch (key) {
		case 'd': case 'D':
			if (state == "DOWN")
				move_dir += 1;
			else {	// state == "UP"
				move_dir -= 1;
			}
			break;
		case 'a': case 'A':
			if (state == "DOWN")
				move_dir -= 1;
			else {
				move_dir += 1;
			}
			break;
		case 'r': case 'R':
			rollback();
			break;
		}
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			//TODO ���� �ӵ��� �ʱ�ȭ = �ٽ� ���� Ƣ��� �ϴ� �ڵ�
			collision_flag = true;
			ssystem->playSound(pingpong, 0, false, nullptr);	// �� ä�ο� sound1�� ��½�Ŵ.
		}
		if (group == "Ball:Cube") {
			//TODO �������� ȹ��
			//gettime. Ư���ð� ���� �浹���� ���� 
		}
	}

};

#endif // !BALL_HPP