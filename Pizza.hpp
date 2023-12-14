#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

class Game_Frame_Work;
class Play_Mode;
extern std::shared_ptr<Game_Frame_Work> game_framework;

class Pizza : public Object {	//�� ��
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
				//TODO ���� ���������� �����ϴ� �ڵ�.
				break;		// finish		//��
			case 5:
				//TODO ���� ���������� �����ϴ� �ڵ�.
				break;		// finish		//����
			case 2:
				break;		// die			// ����
			case 3:
				break;		// break��	// ȸ��
			case 4:
				break;		// ������		// �ʷ�
			}
			
		}
	}
};

#endif // !PIZZA_HPP
