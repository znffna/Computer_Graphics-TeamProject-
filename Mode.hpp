//-------------------------------------------------------
//  Mode.hpp
//  2023. 12. 02
// 
//  게임 모드를 관리하는 class
//-------------------------------------------------------

#ifndef MODE_HPP
#define MODE_HPP

#include "Default.hpp"
#include "World.hpp"
#include "Map.hpp"

extern Camera camera;
extern Shader shader;
extern std::unique_ptr<Light> light;

class Mode {
public:
	Mode() {} // setup 역할
	~Mode() { world.clear(); } // world를 비운다.

	virtual void render() { // displayScene 역할
		
	}

	virtual void update() { // timer 역할

	}
};

class Play_mode : public Mode {

	std::shared_ptr<Ball> ball;
	std::shared_ptr<Map> map;
	std::vector<std::shared_ptr<Cube>> items;
public:
	Play_mode(int stage) {
		// 오브젝트 초기화
		{	// 맵구조 로딩
			map = std::make_shared<Map>();
			if (stage < 0) {
				//TODO 랜덤으로 맵 생성하는 함수 호출 할 것.
				// map.get()->exampleMap();
			}
			else {
				map.get()->loadMap(Map_filename[stage]);
			}
			map.get()->makeMap();
		}

		{	// 조작할 공 생성
			ball = std::make_shared<Ball>();
			ball.get()->setTranslation({ 3.0f, map.get()->getHeight() + ball.get()->getScale().y, 0.0f });
			auto send = std::dynamic_pointer_cast<Object>(ball);
			world.add_object(send);
			world.add_collision_pair("Ball:Pizza", send, NULLPTR);
		}

		{	//조명 초기화
			if(!light)
				light = std::make_unique<Light>(CUBE);
			// Light = new Object(CUBE);
			light->setRotate({ 0.0f, 0.0f, 0.0f });
			light->setTranslation({ 0.0f, map.get()->getHeight() + 5.0f, 10.0f });	//light_pos
			light->setColor({ 1.0f, 1.0f, 1.0f });			//light_color
		}

		{
			camera.setPos({ 0.0f, map.get()->getHeight() + 5.0f, 25.0f * sqrt(2) });
		}
	}
	~Play_mode() {
		Mode::~Mode();
	}

	void render() override { // displayScene 역할
		world.render(shader);
	}

	void update() override { // timer 역할
		// 조명 및 카메라 위치 변경

		// 월드 업데이트
		world.update();
	}

	void reset() { // 시작때로 초기화
		world.reset();
	}
};



#endif // !MODE_HPP


