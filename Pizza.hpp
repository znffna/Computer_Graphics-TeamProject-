#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

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
				// 0�� ������ ������ �ƹ��͵� ���Ѵ�.
				break;
			case 1:
				//TODO ���� ����
				// 1�� ������ �״´�.
				break;
			case 2:
				// ��� �ѹ� �P���� �μ����� ��
				// �μ����� �Լ� ȣ��
				break;
			}
		}
	}
};

#endif // !PIZZA_HPP
