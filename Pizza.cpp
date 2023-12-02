#include "Pizza.hpp"

Pizza::Pizza(float rad, int type) : Object(PIZZA), type{ type }
{
	float degree = rad;
	degree_range_normalization(degree);
	setRotate({ 0.0f, degree, 0.0f });

	backup();
}

void Pizza::update()
{
	//std::cout << "Pizza ÀÇ update È£Ãâ" << '\n';
}
