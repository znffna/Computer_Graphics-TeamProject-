#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

class Game_Frame_Work;
class Play_Mode;
extern std::shared_ptr<Game_Frame_Work> game_framework;

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
				//TODO 다음 스페이지로 연결하는 코드.
				break;		// finish		//흰
			case 5:
				//TODO 다음 스페이지로 연결하는 코드.
				break;		// finish		//검정
			case 2:
				break;		// die			// 빨강
			case 3:
				break;		// break블럭	// 회색
			case 4:
				break;		// 안전블럭		// 초록
			}
			
		}
	}
};

#endif // !PIZZA_HPP
