#include "Pizza.hpp"

Pizza::Pizza(float rad, int type) : Object(PIZZA), type{ type }
{
	setRotate({ 0.0f, rad, 0.0f });
	backup();
}

void Pizza::update()
{

}
