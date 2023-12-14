#include "Pizza.hpp"
#include "Mode.hpp"

void next_level(std::shared_ptr<Game_Frame_Work>& game_framework) {
	game_framework.get()->change_mode(std::make_shared<Play_mode>(game_framework.get()->get_stage() + 1));
}

Pizza::Pizza(float rad, int type) : Object(PIZZA), type{ type }
{
	float degree = rad;
	degree = degree_range_normalization(degree);
	setRotate({ 0.0f, degree, 0.0f });

	backup();
}

void Pizza::update()
{
	//std::cout << "Pizza 의 update 호출" << '\n';
}

void Pizza::handle_events(unsigned char key, const std::string&) {
	switch (key) {
	case 'r': case 'R':
		rollback();
		break;
	}
}

int Pizza::handle_collision(const std::string& group, const std::shared_ptr<Object>& other) {
	if (group == "Ball:Pizza") {
		switch (type) {
		case 0:
			//TODO 다음 스페이지로 연결하는 코드.
			std::cout << "도착함." << '\n';
			next_level(game_framework);
			return 1;
			break;		// finish		//흰
		case 5:
			//TODO 다음 스페이지로 연결하는 코드.
			std::cout << "도착함." << '\n';
			next_level(game_framework);
			return 1;
			break;		// finish		//검정
		case 2:
			break;		// die			// 빨강
		case 3:
			break;		// break블럭	// 회색
		case 4:
			break;		// 안전블럭		// 초록
		}
	}
	return 0;
}
