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

const std::string Mouse_click_sound{ "resource\\sound\\bkgmusic.wav" };
const std::string Title_mode_background{ "resource\\sound\\titlesound.wav" };
const std::string Play_mode_background{ "resource\\sound\\bkgmusic.wav" };

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
	virtual int get_stage() {
		// �ƹ��͵� ����.
		return 0;
	}
};

class Game_Frame_Work {
	std::vector<std::shared_ptr<Mode>>  stack;
	int stage;
	bool input[128]{ false };
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

	int get_stage() {
		return stack[stack.size() - 1].get()->get_stage();
	}

	void change_mode(const std::shared_ptr<Mode>& mode) {
		if( stack.size() > 0) {
			std::cout << "���� �� ����" << '\n';
			stack[stack.size() - 1].reset();
			stack.pop_back();
		}
		std::cout << "���� �� ����" << '\n';
		stack.push_back(mode);
		stack[stack.size() - 1].get()->init();

		if (input['a'] or input['A']) {
			stack[stack.size() - 1].get()->handle_events('a', "DOWN");
		}
		if (input['d'] or input['D']) {
			stack[stack.size() - 1].get()->handle_events('d', "DOWN");
		}
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
		input[key] = state == "DOWN" ? true : false;
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
	FMOD::Sound* background_sound;
public:
	Play_mode(int stage) : stage{ stage } {}

	~Play_mode() {
		bool isPlay{ false };
		channel->isPlaying(&isPlay);
		if (isPlay) {
			channel->stop();
		}

		background_sound->release();
		background_sound = nullptr;

		Mode::~Mode();
	}

	int get_stage() { return stage; }
	// ������ �� �� ȣ�� �ʼ�.
	void init() {
		perspective = true;
		// ���� ���� ����.
		ssystem->createSound(Play_mode_background.c_str(), FMOD_LOOP_NORMAL, 0, &background_sound);	// FMOD_LOOP_NORMAL(�ݺ� ���) , FMOD_DEFAULT (1�� ���)
		channel->setVolume(0.3f);	// ä�� �Ҹ� ũ�� ����
		ssystem->playSound(background_sound, 0, false, &channel);	// �� ä�ο� sound1�� ��½�Ŵ.

		// ������Ʈ �ʱ�ȭ
		shader.enableTexture();
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
			ball.get()->setTranslation({ 3.0f, map.get()->getHeight()+  5.0f + ball.get()->getScale().y, 0.0f });
			ball.get()->backup();
			auto send = std::dynamic_pointer_cast<Object>(ball);
			world.add_object(send);
			world.add_collision_pair("Ball:Pizza", send, NULLPTR);
			world.add_collision_pair("Ball:Cube", send, NULLPTR);
		}


		{	//���� �ʱ�ȭ
			if (!light)
				light = std::make_shared<Light>();
			shader.setLight(true);
			shader.Colorselect(false);
			// Light = new Object(CUBE);
			light.get()->setRotate({ 0.0f, 0.0f, 0.0f });
			light.get()->setTranslation({ 0.0f, map.get()->getHeight() + 5.0f, 10.0f });	//light_pos
			light.get()->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });			//light_color
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
		if (world.update()) {
			return;
		}
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
			shader.setLight(true);
			shader.Colorselect(false);
			if(!light)
				light = std::make_shared<Light>();
			camera.setPos({ 0.0f, 0.0f, 1.0f });
			camera.setDir({ 0.0f, 0.0f, -1.0f });
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
	std::shared_ptr<Square> background;
public:
	GLuint intro_texture{0};
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
		perspective = false;

		ssystem->createSound(Title_mode_background.c_str(), FMOD_LOOP_NORMAL, 0, &background_sound);	// FMOD_LOOP_NORMAL(�ݺ� ���) , FMOD_DEFAULT (1�� ���)
		ssystem->createSound(Mouse_click_sound.c_str(), FMOD_DEFAULT, 0, &click_sound);	// FMOD_LOOP_NORMAL(�ݺ� ���) , FMOD_DEFAULT (1�� ���)

		channel->setVolume(0.3f);	// ä�� �Ҹ� ũ�� ����
		{
			bool isPlay{ false };
			channel->isPlaying(&isPlay);
			if (isPlay) {
				channel->stop();
			}
		}
		ssystem->playSound(background_sound, 0, false, &channel);	// �� ä�ο� sound1�� ��½�Ŵ.

		if (!intro_texture) {
			shader.use();
			intro_texture = loadTexture("resource\\tilte.png");
		}
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

		background = std::make_shared<Square>(intro_texture);
		world.add_object(background);
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
			//���� �ʱ�ȭ
			shader.Colorselect(Shader::lightOption = false);
			light = std::make_shared<Light>();
			camera.setPos({ 0.0f, 0.0f, 1.0f });
			camera.setDir({ 0.0f, 0.0f, -1.0f });
			break;
		case 's': case 'S':
			game_framework.get()->change_mode(std::make_shared<Play_mode>(0));
			break;
		}
	}

	void handle_events(float mx, float my) override {
		// ssystem->playSound(click_sound, 0, false, nullptr);	// ä�������� ���� ��� �˾Ƽ� ä�� ������ ��������� �˾Ƽ� ä���� ������. 
		game_framework.get()->change_mode(std::make_shared<Play_mode>(0));
	}
	void reset() { // ���۶��� �ʱ�ȭ
		world.reset();
	}
};

//GLuint loadTexture(const char* imagePath)
//{
//	// �̹��� ������ �ε�
//	int width, height, channels;
//	unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);
//	if (!imageData) {
//		std::cerr << "Failed to load image: " << imagePath << std::endl;
//		return 0;
//	}
//
//	// OpenGL �ؽ�ó ����
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// �ؽ�ó �Ķ���� ����
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	// �ؽ�ó ������ ����
//	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	if (channels == 3)
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
//	else
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
//
//	// �̹��� ������ ����
//	stbi_image_free(imageData);
//
//	return textureID;
//}


#endif // !MODE_HPP


