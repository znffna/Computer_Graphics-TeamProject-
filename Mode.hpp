//-------------------------------------------------------
//  Mode.hpp
//  2023. 12. 02
// 
//  ���� ��带 �����ϴ� class
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
	Mode() {} // setup ����

	virtual void render() { // displayScene ����
		
	}

	virtual void update() { // timer ����

	}
};

class Play_mode : public Mode {
	std::shared_ptr<Ball> ball;
	std::shared_ptr<Map> map;
	
public:
	Play_mode(int stage) {
		// ������Ʈ �ʱ�ȭ
		{	// �ʱ��� �ε�
			map = std::make_shared<Map>();
			switch (stage) {
			default:
				map.get()->loadMap("waterslide.map");
				break;
			}
			map.get()->makeMap();
		}

		{	// ������ �� ����
			ball = std::make_shared<Ball>();
			ball.get()->setTranslation({ 3.0f, map.get()->getHeight() + ball.get()->getScale().y, 0.0f });
			auto send = std::dynamic_pointer_cast<Object>(ball);
			world.add_object(send);
			world.add_collision_pair("Ball:Pizza", send, NULLPTR);
		}

		{	//���� �ʱ�ȭ
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

	void render() override { // displayScene ����

	}

	void update() override { // timer ����

	}

	void reset() { // ���۶��� �ʱ�ȭ
		world.reset();
	}
};



#endif // !MODE_HPP

