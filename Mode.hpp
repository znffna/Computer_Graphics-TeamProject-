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
#include "Camera.hpp"
#include "Shader.hpp"


extern Camera camera;
extern Shader shader;
extern std::shared_ptr<Light> light;

class Mode {
public:
	Mode() {} // setup ����
	~Mode() { world.clear(); } // world�� ����.

	void change_mode() {
		
	}
	virtual void init() {	//�ʱ�ȭ ����.

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

class Game_Frame_Work {
	std::vector<std::shared_ptr<Mode>>  stack;
public:

	// ��� �Լ�
	Game_Frame_Work() {
		{ // ���� ���� �ɼ�s
			result = FMOD::System_Create(&ssystem);		//--- ���� �ý��� ����
			if (result != FMOD_OK) {
				std::cout << "���� �ý��� ���� ����! " << '\n';
				exit(0);	// ���� üũ
			}

			ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
			channel->stop();	//
		}
	}

	~Game_Frame_Work() {
	
	}

	void change_mode(std::shared_ptr<Mode> mode) {
		if( stack.size() > 0) {
			stack[stack.size() - 1].reset();
			stack.pop_back();
		}

		stack.push_back(mode);
		stack[stack.size() - 1].get()->init();
	}

	void push_mode(std::shared_ptr<Mode>& mode) {
		stack.push_back(mode);	
		stack[stack.size() - 1].get()->init();
	}

	void pop_mode() {
		if (stack.size() > 0) {
			stack.pop_back();
		}
	}

	void quit() {
		glutLeaveMainLoop();
	}

	void update() {
		stack[stack.size()-1].get()->update();
	}

	void render() {
		for (auto m : stack) {
			m.get()->render();
		}
	}

	void handle_events(unsigned int key, const std::string& state) {
		// Ű���� �Է� ó��
		if (key == 'q' or key == 'Q') {
			quit();
		}
		stack[stack.size() - 1].get()->handle_events(key, state);
	}

	void handle_events(float mx, float my) {
		// ���콺 Ŭ�� �Է� ó�� 
		stack[stack.size() - 1].get()->handle_events(mx, my);
	}

};

extern std::shared_ptr<Game_Frame_Work> game_framework;

class Play_mode : public Mode {
	std::vector<GLuint> texture;
	std::shared_ptr<Ball> ball;
	std::shared_ptr<Map> map;
	std::vector<std::shared_ptr<Cube>> items;
	int stage;
public:
	Play_mode(int stage) : stage{ stage } {}

	~Play_mode() {
		Mode::~Mode();
	}

	// ������ �� �� ȣ�� �ʼ�.
	void init() {
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
			if (!light)
				light = std::make_shared<Light>();
			shader.Colorselect(Shader::lightOption = true);
			// Light = new Object(CUBE);
			light.get()->setRotate({ 0.0f, 0.0f, 0.0f });
			light.get()->setTranslation({ 0.0f, map.get()->getHeight() + 5.0f, 10.0f });	//light_pos
			light.get()->setColor({ 1.0f, 1.0f, 1.0f });			//light_color
		}

		{
			camera.setPos({ 0.0f, map.get()->getHeight() + 5.0f, 25.0f * sqrt(2) });
		}
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
	FMOD::Sound* background_sound;
	FMOD::Sound* click_sound;


	Title_mode() {
		
	}

	~Title_mode() {
		bool isPlay{ false };
		channel->isPlaying(&isPlay);
		if (isPlay) {
			channel->stop();
		}

		background_sound->release();
		background_sound = nullptr;
		
		click_sound->release();
		click_sound = nullptr;

		Mode::~Mode();
	}

	void init() override {	//�ʱ�ȭ ����.
		ssystem->createSound("Illusion.mp3", FMOD_LOOP_NORMAL, 0, &background_sound);	// FMOD_LOOP_NORMAL(�ݺ� ���) , FMOD_DEFAULT (1�� ���)
		ssystem->createSound("Illusion.mp3", FMOD_DEFAULT, 0, &click_sound);	// FMOD_LOOP_NORMAL(�ݺ� ���) , FMOD_DEFAULT (1�� ���)

		channel->setVolume(0.3f);	// ä�� �Ҹ� ũ�� ����
		ssystem->playSound(background_sound, 0, false, &channel);	// �� ä�ο� sound1�� ��½�Ŵ.

		// (��, �̰�� ������ �ش� sound�� ������ �÷��̰� �Ǿ�߸� �����ȴ�(���߿� stop �Ұ�) -> ȿ�������� ���.)
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
			game_framework.get()->change_mode(std::make_shared<Play_mode>(0));
			break;
		}
	}

	void handle_events(float mx, float my) override {
		ssystem->playSound(click_sound, 0, false, nullptr);	// ä�������� ���� ��� �˾Ƽ� ä�� ������ ��������� �˾Ƽ� ä���� ������. 
		if (0.3f < mx and mx < 0.7f) {
			game_framework.get()->change_mode(std::make_shared<Play_mode>(0));
		}
	}
	void reset() { // ���۶��� �ʱ�ȭ
		world.reset();
	}
};




#endif // !MODE_HPP


