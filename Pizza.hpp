#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"

class Pizza : public Object {	//»ó ¼Ó
	int type;
public:
	Pizza(float rad, int type) : Object(PIZZA), type{ type } {
		setRotate({ 0.0f, rad, 0.0f });
	}
};

#endif // !PIZZA_HPP
