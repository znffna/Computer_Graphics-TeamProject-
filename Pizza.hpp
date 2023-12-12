#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

class Pizza : public Object {	//상 속
	int type;
	bool isCollapse{ false };
public:
	Pizza(float rad, int type);

	// interface
	void setType(int rhs) { type = rhs; }
	int getType() { return type; }

	void update();

	void render() const override {
		draw();
	}

	void handle_events(unsigned char key, const std::string&) override {
		switch (key) {
		case 'r': case 'R':
			rollback();
			break;
		}
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			switch (type) {
			case 0:
				// 0은 평지기 때문에 아무것도 안한다.
				break;
			case 1:
				//TODO 게임 오버
				// 1은 닿으면 죽는다.
				break;
			case 2:
				// 얘는 한번 밣으면 부서지는 블럭
				// 부서지는 함수 호출
				break;
			}
		}
	}
};

#endif // !PIZZA_HPP
