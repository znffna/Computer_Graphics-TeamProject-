#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

class Pizza : public Object {	//»ó ¼Ó
	int type;
public:
	Pizza(float rad, int type);

	// interface
	void setType(int rhs) { type = rhs; }
	int getType() { return type; }

	void update();

	void render() const override {
		draw();
	}

	void handle_events(unsigned char key) override {
		switch (key) {
		case 'r': case 'R':
			rollback();
			break;
		}
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {

	}
};

#endif // !PIZZA_HPP
