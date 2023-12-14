#include "Object.hpp"
#include <chrono>

#ifndef BALL_HPP
#define BALL_HPP

extern FMOD::Sound* pingpong;
extern FMOD::Sound* item;

const std::string poing_sound{ "resource\\sound\\ballsound.mp3" };
const std::string item_sound{ "resource\\sound\\item.mp3" };

class Ball : public Object {
	float radius{ 5.0f }; // 원기둥 중심으로 부터의 거리
	float size{ 0.6f };		// 공의 크기

	float velocity{ 5.0f };		// 좌우 이동 속도(rad)
	int move_dir{ 0 };			// 좌우 이동 방향(0: 이동안함)

	bool collision_flag{ false };	// 공의 scale을 움직일땐 true, translate가 움직일땐 false;
	float reset_velocity{ 0.4f };	// 땅과 부딫힐 경우 초기화될 값.
	float fall_velocity{ 0.0f };	// 내려가는 속도 (가속도로 계속 줄어들 값 )
	float fall_acceleration{ 0.03f };	// 중력역할 ( 줄어드는 값을 적으면 됨 )
	time_t item_start{};
	bool invincibility{ false }; //무적

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

	// 좌우 회전 이동
	void horizontal_move() {
		translation_rotate({ 0.0f,  move_dir * velocity, 0.0f });
		addRotate({ 0.0f,  move_dir * velocity, 0.0f });
	}

	// vertical_move( 상하 수직 운동 )
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
			fall_velocity -= fall_acceleration; // 시간개념은 안넣었음.
			fall_velocity = glm::clamp(fall_velocity, -2.0f, 2.0f);
			addTranslation({ 0.0f, fall_velocity, 0.0f });
		}
	}

	void update() override {
		//std::cout << "Ball.update() 호출" << '\n';
		horizontal_move();	// 좌우 이동 갱신
		if (invincibility) {
			std::cout << "무적 진행 시간: " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - item_start << '\n';
			if (std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - item_start > 1) {
				invincibility = false;
				collision_flag = false;
				fall_velocity = 0.0f;
				setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			}
			addTranslation({ 0.0f, -0.5f, 0.0f });
		}
		else {
			falling();	// 상하 이동 갱신
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
			//TODO 볼의 속도를 초기화 = 다시 위로 튀기기 하는 코드
			if(!invincibility) {
				collision_flag = true;
				ssystem->playSound(pingpong, 0, false, nullptr);	// 뒤 채널에 sound1을 출력시킴.
			}

		}
		if (group == "Ball:Cube") {
			//TODO 아이템을 획득
			//gettime. 특정시간 무적 충돌판정 무시 
			item_start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			invincibility = true;
			setColor({ 134.0f / 256.0f, 71.0f / 256.0f, 71.0f / 256.0f, 0.5f });

			ssystem->playSound(item, 0, false, nullptr);	// 뒤 채널에 sound1을 출력시킴.
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