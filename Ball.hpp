#include "Object.hpp"

class Ball : public Object {
	float velocity{ 5.0f };		// 좌우 이동 속도(rad)
	int move_dir{ 0 };			// 좌우 이동 방향(0: 이동안함)

	float fall_velocity{ 0.0f };// 내려가는 속도 (가변)
	float fall_accelation{ 0.2f };// 중력역할
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

	// 좌우 회전 이동
	void horizontal_move() {
		Object::translation_rotate({ 0.0f,  move_dir * velocity, 0.0f });
	}

	// vertical_move( 상하 수직 운동 )
	void falling() {
		fall_velocity -= fall_accelation; // 시간개념은 안넣었음.
		addTranslation({ 0.0f, fall_velocity, 0.0f });
	}

	void update() override {
		//std::cout << "Ball.update() 호출" << '\n';
		horizontal_move();	// 좌우 이동 갱신
		falling();	// 상하 이동 갱신
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			//TODO 볼의 속도를 초기화
			fall_velocity = 1.0f;
		}
		if (group == "Ball:Cube") {
			//TODO 아이템을 획득
		}
	}

};
