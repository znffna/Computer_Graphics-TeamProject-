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
extern std::shared_ptr<Light> light;

class Mode {
public:
	Mode() {} // setup ����
	~Mode() { world.clear(); } // world�� ����.

	virtual void render() { // displayScene ����
		
	}

	virtual void update() { // timer ����

	}
	virtual void handle_events(unsigned int key, const std::string& state) {
		// state : "DOWN", "UP"
	}
};

class Play_mode : public Mode {

	std::shared_ptr<Ball> ball;
	std::shared_ptr<Map> map;
	std::vector<std::shared_ptr<Cube>> items;
public:
	Play_mode(int stage) {
		// ������Ʈ �ʱ�ȭ
		{	// �ʱ��� �ε�
			map = std::make_shared<Map>();
			if (stage < 0) {
				//TODO �������� �� �����ϴ� �Լ� ȣ�� �� ��.
				// map.get()->exampleMap();
			}
			else {
				map.get()->loadMap(Map_filename[stage]);
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
			if(!light)
				light = std::make_shared<Light>();
			// Light = new Object(CUBE);
			light.get()->setRotate({0.0f, 0.0f, 0.0f});
			light.get()->setTranslation({ 0.0f, map.get()->getHeight() + 5.0f, 10.0f });	//light_pos
			light.get()->setColor({ 1.0f, 1.0f, 1.0f });			//light_color
		}

		{
			camera.setPos({ 0.0f, map.get()->getHeight() + 5.0f, 25.0f * sqrt(2) });
		}
	}

	~Play_mode() {
		Mode::~Mode();
		light.reset();
	}

	void render() override { // displayScene ����
		world.render(shader);
	}

	void update() override { // timer ����
		// ���� ������Ʈ
		world.update();
		// ���� �� ī�޶� ��ġ ����
		light.get()->setTranslation({ 0.0f, ball.get()->getTranslation().y + 5.0f, 25.0f * sqrt(2) });
		camera.setPos(ball.get()->getTranslation());
		camera.setPos(0, ball.get()->getTranslation().x * 5);
		camera.setPos(1, ball.get()->getTranslation().y + 6.0f);
		camera.setPos(2, ball.get()->getTranslation().z * 5);
		camera.setDir(glm::normalize(ball.get()->getTranslation() - camera.getPos()));
	}

	void handle_events(unsigned int key, const std::string& state) override {
		switch (key) {
		case 'a': case 'A': case'd': case 'D':
			ball.get()->handle_events(key, state);
			break;
		case 'r': case 'R':	// �ʱ�ȭ �Է�
			world.handle_events(key, state);
			break;
		}
	}

	void reset() { // ���۶��� �ʱ�ȭ
		world.reset();
	}
};

extern std::unique_ptr<Mode> current_mode;



#endif // !MODE_HPP


