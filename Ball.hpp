#include "Object.hpp"

#ifndef BALL_HPP
#define BALL_HPP

class Ball : public Object {
	float radius{ 3.0f }; // 원기둥 중심으로 부터의 거리

	float velocity{ 5.0f };		// 좌우 이동 속도(rad)
	int move_dir{ 0 };			// 좌우 이동 방향(0: 이동안함)

	float reset_velocity{ 0.5f };	// 땅과 부딫힐 경우 초기화될 값.
	float fall_velocity{ 0.0f };	// 내려가는 속도 (가속도로 계속 줄어들 값 )
	float fall_acceleration{ 0.05f };	// 중력역할 ( 줄어드는 값을 적으면 됨 )
public:
	Ball() : Object(SPHERE) { backup(); }
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

	// 좌우 회전 이동
	void horizontal_move() {
		translation_rotate({ 0.0f,  move_dir * velocity, 0.0f });
		addRotate({ 0.0f,  move_dir * velocity, 0.0f });
	}

	// vertical_move( 상하 수직 운동 )
	void falling() {
		fall_velocity -= fall_acceleration; // 시간개념은 안넣었음.
		fall_velocity = glm::clamp(fall_velocity, -4.0f, 4.0f);
		addTranslation({ 0.0f, fall_velocity, 0.0f });
	}

	void update() override {
		//std::cout << "Ball.update() 호출" << '\n';
		horizontal_move();	// 좌우 이동 갱신
		falling();	// 상하 이동 갱신
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
			//TODO 볼의 속도를 초기화 = 다시 위로 튀기기 하는 코드
			fall_velocity = reset_velocity;
		}
		if (group == "Ball:Cube") {
			//TODO 아이템을 획득
		}
	}

};

#endif // !BALL_HPP