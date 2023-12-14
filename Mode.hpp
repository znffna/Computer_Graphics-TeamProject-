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
#include "Camera.hpp"
#include "Shader.hpp"

const std::string Mouse_click_sound{ "resource\\sound\\bkgmusic.wav" };
const std::string Title_mode_background{ "resource\\sound\\titlesound.wav" };
const std::string Play_mode_background{ "resource\\sound\\bkgmusic.wav" };

extern std::shared_ptr<Light> light;

class Mode {
public:
	Mode() {} // setup 역할
	~Mode() { world.clear(); } // world를 비운다.

	void change_mode() {
		
	}
	virtual void init() {	//초기화 역할.

	}

	virtual void render() { // displayScene 역할
		
	}

	virtual void update() { // timer 역할

	}
	virtual void handle_events(unsigned int key, const std::string& state) {
		// 키보드 입력 처리
		// state : "DOWN", "UP"
	}
	virtual void handle_events(float mx, float my) {
		// 마우스 클릭 입력 처리 
	}
	virtual int get_stage() {
		// 아무것도 안함.
		return 0;
	}
};

class Game_Frame_Work {
	std::vector<std::shared_ptr<Mode>>  stack;
	int stage;
	bool input[128]{ false };
public:

	// 멤버 함수
	Game_Frame_Work() {
		{ // 사운드 관련 옵션s
			result = FMOD::System_Create(&ssystem);		//--- 사운드 시스템 생성
			if (result != FMOD_OK) {
				std::cout << "사운드 시스템 생성 오류! " << '\n';
				exit(0);	// 오류 체크
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
			std::cout << "현재 맵 삭제" << '\n';
			stack[stack.size() - 1].reset();
			stack.pop_back();
		}
		std::cout << "현재 맵 생성" << '\n';
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
		// 키보드 입력 처리
		input[key] = state == "DOWN" ? true : false;
		if (key == 'q' or key == 'Q') {
			quit();
		}
		stack[stack.size() - 1].get()->handle_events(key, state);
	}

	void handle_events(float mx, float my) {
		// 마우스 클릭 입력 처리 
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
	// 생성후 한 번 호출 필수.
	void init() {
		perspective = true;
		// 사운드 설정 먼저.
		ssystem->createSound(Play_mode_background.c_str(), FMOD_LOOP_NORMAL, 0, &background_sound);	// FMOD_LOOP_NORMAL(반복 재생) , FMOD_DEFAULT (1번 출력)
		channel->setVolume(0.3f);	// 채널 소리 크기 조절
		ssystem->playSound(background_sound, 0, false, &channel);	// 뒤 채널에 sound1을 출력시킴.

		// 오브젝트 초기화
		shader.enableTexture();
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
			ball.get()->setTranslation({ 3.0f, map.get()->getHeight()+  5.0f + ball.get()->getScale().y, 0.0f });
			ball.get()->backup();
			auto send = std::dynamic_pointer_cast<Object>(ball);
			world.add_object(send);
			world.add_collision_pair("Ball:Pizza", send, NULLPTR);
			world.add_collision_pair("Ball:Cube", send, NULLPTR);
		}


		{	//조명 초기화
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

	void render() override { // displayScene 역할
		world.render(shader);
	}

	void update() override { // timer 역할
		// 월드 업데이트
		if (world.update()) {
			return;
		}
		// 조명 및 카메라 위치 변경
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
		case 'r': case 'R':	// 초기화 입력
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
		// 마우스 클릭 입력 처리 
		// playmode에서는 처리할게 없다.
	}

	void reset() { // 시작때로 초기화
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

	void init() override {	//초기화 역할.
		perspective = false;

		ssystem->createSound(Title_mode_background.c_str(), FMOD_LOOP_NORMAL, 0, &background_sound);	// FMOD_LOOP_NORMAL(반복 재생) , FMOD_DEFAULT (1번 출력)
		ssystem->createSound(Mouse_click_sound.c_str(), FMOD_DEFAULT, 0, &click_sound);	// FMOD_LOOP_NORMAL(반복 재생) , FMOD_DEFAULT (1번 출력)

		channel->setVolume(0.3f);	// 채널 소리 크기 조절
		{
			bool isPlay{ false };
			channel->isPlaying(&isPlay);
			if (isPlay) {
				channel->stop();
			}
		}
		ssystem->playSound(background_sound, 0, false, &channel);	// 뒤 채널에 sound1을 출력시킴.

		if (!intro_texture) {
			shader.use();
			intro_texture = loadTexture("resource\\tilte.png");
		}
		// (단, 이경우 무조건 해당 sound가 끝까지 플레이가 되어야만 중지된다(도중에 stop 불가) -> 효과음에만 사용.)
		// 오브젝트 초기화
		world.clear();
		{	//조명 초기화
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

	void render() override { // displayScene 역할
		world.render(shader);
	}

	void update() override { // timer 역할
		// 월드 업데이트
		world.update();
	}

	void handle_events(unsigned int key, const std::string& state) override {
		switch (key) {
		case 'r': case 'R':	// 초기화 입력
			//조명 초기화
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
		// ssystem->playSound(click_sound, 0, false, nullptr);	// 채널지정을 안할 경우 알아서 채널 생성후 재생끝날시 알아서 채널이 삭제됨. 
		game_framework.get()->change_mode(std::make_shared<Play_mode>(0));
	}
	void reset() { // 시작때로 초기화
		world.reset();
	}
};

//GLuint loadTexture(const char* imagePath)
//{
//	// 이미지 데이터 로드
//	int width, height, channels;
//	unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);
//	if (!imageData) {
//		std::cerr << "Failed to load image: " << imagePath << std::endl;
//		return 0;
//	}
//
//	// OpenGL 텍스처 생성
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// 텍스처 파라미터 설정
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	// 텍스처 데이터 전송
//	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	if (channels == 3)
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
//	else
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
//
//	// 이미지 데이터 해제
//	stbi_image_free(imageData);
//
//	return textureID;
//}


#endif // !MODE_HPP


