#include "Object.hpp"
#include <chrono>

#ifndef BALL_HPP
#define BALL_HPP

extern FMOD::Sound* pingpong;
extern FMOD::Sound* item;

const std::string poing_sound{ "resource\\sound\\ballsound.mp3" };
const std::string item_sound{ "resource\\sound\\item.mp3" };

class Ball : public Object {
	float radius{ 5.0f }; // ����� �߽����� ������ �Ÿ�
	float size{ 0.6f };		// ���� ũ��

	float velocity{ 5.0f };		// �¿� �̵� �ӵ�(rad)
	int move_dir{ 0 };			// �¿� �̵� ����(0: �̵�����)

	bool collision_flag{ false };	// ���� scale�� �����϶� true, translate�� �����϶� false;
	float reset_velocity{ 0.4f };	// ���� �΋H�� ��� �ʱ�ȭ�� ��.
	float fall_velocity{ 0.0f };	// �������� �ӵ� (���ӵ��� ��� �پ�� �� )
	float fall_acceleration{ 0.03f };	// �߷¿��� ( �پ��� ���� ������ �� )
	time_t item_start{};
	bool invincibility{ false }; //����

public:
	Ball() : Object(SPHERE) {
		setTranslation({ radius, 0.0f, 0.0f });
		setScale(glm::vec3(size));
		backup();
		FMOD_RESULT result1;
		FMOD_RESULT result2;
		result1 = ssystem->createSound(poing_sound.c_str(), FMOD_DEFAULT, nullptr, &pingpong);
		result2 = ssystem->createSound(item_sound.c_str(), FMOD_DEFAULT, nullptr, &item);
		item_start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	}
	// interface function
	float getVelocity() { return velocity; }
	void setVelocity(float rhs) { velocity = rhs; }

	float getFallVelocity() { return fall_velocity; }
	void setFallVelocity(float rhs) { fall_velocity = rhs; }

	float getFallAccelation() { return fall_acceleration; }
	void setFallAccelation(float rhs) { fall_acceleration = rhs; }

	bool getinvincibility() { return  invincibility; }

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
			fall_velocity = glm::clamp(fall_velocity, -2.0f, 2.0f);
			addTranslation({ 0.0f, fall_velocity, 0.0f });
		}
	}

	void update() override {
		//std::cout << "Ball.update() ȣ��" << '\n';
		horizontal_move();	// �¿� �̵� ����
		if (invincibility) {
			std::cout << "���� ���� �ð�: " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - item_start << '\n';
			if (std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - item_start > 1) {
				invincibility = false;
				collision_flag = false;
				fall_velocity = 0.0f;
				setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			}
			addTranslation({ 0.0f, -0.5f, 0.0f });
		}
		else {
			falling();	// ���� �̵� ����
		}

	}

	void render() const override {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw();
		glDisable(GL_BLEND);
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

	int handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			//TODO ���� �ӵ��� �ʱ�ȭ = �ٽ� ���� Ƣ��� �ϴ� �ڵ�
			if(!invincibility) {
				collision_flag = true;
				ssystem->playSound(pingpong, 0, false, nullptr);	// �� ä�ο� sound1�� ��½�Ŵ.
			}

		}
		if (group == "Ball:Cube") {
			//TODO �������� ȹ��
			//gettime. Ư���ð� ���� �浹���� ���� 
			item_start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			invincibility = true;
			setColor({ 134.0f / 256.0f, 71.0f / 256.0f, 71.0f / 256.0f, 0.5f });

			ssystem->playSound(item, 0, false, nullptr);	// �� ä�ο� sound1�� ��½�Ŵ.
		}
		return 0;
	}

	void rollback() override {
		Object::rollback();
		fall_velocity = 0.0f;
		collision_flag = false;
	}

};

#endif // !BALL_HPP