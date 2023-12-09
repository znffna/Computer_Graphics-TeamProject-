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

	void change_mode() {
		
	}

	virtual void render() { // displayScene ����
		
	}

	virtual void update() { // timer ����

	}
	virtual void handle_events(unsigned int key, const std::string& state) {
		// Ű���� �Է� ó��
		// state : "DOWN", "UP"
	}
	virtual void handle_events(float mx, float my) {
		// ���콺 Ŭ�� �Է� ó�� 
	}
};
extern std::shared_ptr<Mode> current_mode;

class Play_mode : public Mode {
	std::vector<GLuint> texture;
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
			light = std::make_shared<Light>();
			shader.Colorselect(Shader::lightOption = true);
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

	void handle_events(float mx, float my) {
		// ���콺 Ŭ�� �Է� ó�� 
		// playmode������ ó���Ұ� ����.
	}

	void reset() { // ���۶��� �ʱ�ȭ
		world.reset();
	}
};

class Title_mode : public Mode {
public:
	GLuint intro_texture;

	Title_mode() {
		// ������Ʈ �ʱ�ȭ
		world.clear();
		{	//���� �ʱ�ȭ
			shader.Colorselect(Shader::lightOption = false);
			light = std::make_shared<Light>();
		}

		{
			camera.setPos({ 0.0f, 0.0f, 1.0f });
			camera.setDir({ 0.0f, 0.0f, -1.0f });
		}
		
	}

	~Title_mode() {
		Mode::~Mode();
		light.reset();
	}

	void render() override { // displayScene ����
		world.render(shader);
	}

	void update() override { // timer ����
		// ���� ������Ʈ
		world.update();
	}

	void handle_events(unsigned int key, const std::string& state) override {
		switch (key) {
		case 'r': case 'R':	// �ʱ�ȭ �Է�
			break;
		case 's': case 'S':
			{
				current_mode.reset();
				current_mode = std::make_shared<Play_mode>(0);
			}
			break;
		}
	}

	void handle_events(float mx, float my) override {
		if (0.3f < mx and mx < 0.7f) {
			current_mode.reset();
			current_mode = std::make_shared<Play_mode>(0);
		}
	}
	void reset() { // ���۶��� �ʱ�ȭ
		world.reset();
	}
};




#endif // !MODE_HPP


