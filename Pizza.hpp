#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

class Pizza : public Object {	//»ó ¼Ó
	int type;
public:
	Pizza(float rad, int type);

	void update();

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {

	}
};

#endif // !PIZZA_HPP
