#pragma once

#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "Object.hpp"
#include <memory>

class Game_Frame_Work;
class Play_Mode;


extern FMOD::Sound* victory;
extern FMOD::Sound* defeat;

const std::string victory_sound{ "resource\\sound\\victory.mp3" };
const std::string defeat_sound{ "resource\\sound\\defeat.mp3" };

void next_level(std::shared_ptr<Game_Frame_Work>& );

class Pizza : public Object {	//»ó ¼Ó
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

	void handle_events(unsigned char key, const std::string&) override;

	int handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override;
};

#endif // !PIZZA_HPP
