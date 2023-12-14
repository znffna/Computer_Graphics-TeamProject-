#define _CRT_SECURE_NO_WARRINGS

#include "Default.hpp"
#include "Mesh.hpp"
#include "Mode.hpp"
#include "World.hpp"
#include "Object.hpp"
#include "Pizza.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Map.hpp"


//glew32.lib freeglut.lib
const char title[] = "[[팀 프로젝트]]";
const std::string User_guide[] = {
"-------------------",
"--[[팀 프로젝트]]--",
"-------------------"
" - 타이틀 : 마우스 클릭으로 게임 시작",
" - 공 조작법 : A (좌)/ D(우)",
"-------------------"
//"paste_here",
};


//--------------------------------------------------------
//--- 메인 함수
//--------------------------------------------------------
GLvoid drawScene();													//--- 화면 그리기 콜백 함수
GLvoid Reshape(int w, int h);										//--- 다시 그리기 콜백 함수
GLvoid Mouse(int button, int state, int x, int y);					//--- 마우스 클릭 콜백 함수
GLvoid Keyboard(unsigned char key, int x, int y);					//--- 키보드 DOWN 콜백 함수
GLvoid KeyboardUp(unsigned char key, int x, int y);					//--- 키보드 UP   콜백 함수
GLvoid specialKeyboard(int key, int x, int y);						//--- 키보드 특수 콜백 함수
GLvoid Motion(int x, int y);										//--- 마우스 drag 콜백 함수
GLvoid handleMouseWheel(int wheel, int direction, int x, int y);	//--- 마우스 휠   콜백 함수
GLvoid Timer(int value);											//--- 타이머      콜백 함수

//--------------------------------------------------------
//--- 메인 변수 선언
//--------------------------------------------------------


//flag 및 베이스 변수들
static glm::vec3 background_color{ 0.7f, 0.7f, 0.7f };	//--- 배경 색깔
static bool leftdown{ false };								//--- 마우스 클릭상황 flag

//출력 옵션s
bool drawstyle{ true };	//false : 와이어(line) 객체/ true : 솔리드(triangle) 객체
bool depthcheck{ true };	//은면제거 유무
//마우스 좌표값
static float mousex{ 0.0f };		//마우스의 x값
static float mousey{ 0.0f };		//마우스의 y값

// 조명 위치 적용할 Object
//Object* Light;


//--------------------------------------------------------
//--- 실습용 함수 선언
//--------------------------------------------------------
GLvoid setup();				//--- main에서 최초로 생성할때 부르는 초기화 함수

//------------------------------------
//메인 함수 정의
//------------------------------------
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

//--- main
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 100);
	glutInitWindowSize(window_row, window_col);
	glutCreateWindow(title);

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//{
	//	//--- GL 디버그
	//	glEnable(GL_DEBUG_OUTPUT);
	//	glDebugMessageCallback(MessageCallback, 0);
	//}
	Shader::debug = false;
	//--- 사운드 시스템 초기화
	//{
	//	result = FMOD::System_Create(&ssystem);		//--- 사운드 시스템 생성
	//	if (result != FMOD_OK) {
	//		std::cout << "사운드 시스템 생성 오류! " << '\n';
	//		exit(0);	// 오류 체크
	//	}
	//	ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	//	ssystem->createSound("Illusion.mp3", FMOD_LOOP_NORMAL, 0, &sound1);	// FMOD_LOOP_NORMAL(반복 재생) , FMOD_DEFAULT (1번 출력)
	//	ssystem->createSound("Illusion.mp3", FMOD_DEFAULT, 0, &sound2);	// FMOD_LOOP_NORMAL(반복 재생) , FMOD_DEFAULT (1번 출력)
	//	// 예시 출력
	//	channel->stop();				// 채널에 출력중인 소리 중지
	//	channel->setVolume(0.3);	// 채널 소리 크기 조절
	//	ssystem->playSound(sound1, 0, false, &channel);	// 뒤 채널에 sound1을 출력시킴.
	//	ssystem->playSound(sound2, 0, false, nullptr);	// 채널지정을 안할 경우 알아서 채널 생성후 재생끝날시 알아서 채널이 삭제됨. 
	//																			// (단, 이경우 무조건 해당 sound가 끝까지 플레이가 되어야만 중지된다(도중에 stop 불가) -> 효과음에만 사용.)
	//	channel->stop();	//
	//}

	//--- 세이더 생성
	shader.make_shaderProgram();
	shader.setUniform(0, "useTexture");
	//shader.getUniformLocate("");
	// 가져다 사용할 obj 읽어오기
	Mesh::debug = false;
	Read_ObjectFile();
	
	//--- 기본 셋팅 초기화
	setup();
	// 디버그 세팅
	Mesh::debug = false;

	// 키보드 조작 명령어 출력
	for (const std::string& s : User_guide) {
		std::cout << s << '\n';
	}

	//--- 콜업 함수 설정
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(Mouse);
	glutMouseWheelFunc(handleMouseWheel);
	glutMotionFunc(Motion);

	glutTimerFunc(16, Timer, 0);

	//--- 메인 루프 진행
	glutMainLoop();
}

//--- main에서만 호출하는 최초 초기화 함수
GLvoid setup() {
	//전역변수 초기화	
	leftdown = false;		//--- 마우스 클릭상황 flag
	depthcheck = true;		//은면제거 유무
	drawstyle = true;	//false : 와이어(line) 객체/ true : 솔리드(triangle) 객체

	Mesh::debug = false;
	{	//카메라 위치 초기화
		camera.reset();
		camera.setPos({ 0.0f, 0.0f, 25.0f * sqrt(2)});
	}
	game_framework.get()->change_mode(std::make_shared<Title_mode>());
}

const int Projective_PERSPECTIVE{ 1 };
const int Projective_ORTHO{ 0 };

void RenderWorld(Camera& camera, int perspective) {
	const int vertex_color{ 1 };
	const int uniform_color{ 0 };
	//--- 렌더링 파이프라인에 세이더 불러오기
	shader.use();

	//--- 깊이 비교 켜기	
	depthcheck ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
	//---- 뷰 변환
	shader.viewTransform(camera);

	//---- 투영 변환	
	perspective? shader.perspectiveTransform(camera) : shader.orthoTransform(camera);	// true : 원근 투영 / false : 직각 투영

	//--- 조명 설정
	shader.setLight(Shader::lightOption);	// 조명 사용 유무

	//--- 기본 색상 설정
	shader.Colorselect(uniform_color);

	glFrontFace(GL_CCW);	//시계 좌표계
	
	//--- 조명 위치 출력
	{	
		shader.Colorselect(uniform_color);
		shader.setColor({ light.get()->getColor()});
		glm::vec3 tmp_translation = light.get()->getTranslation();
		light.get()->setTranslation(light.get()->getTranslation() + (glm::normalize(light.get()->getTranslation()) * glm::vec3{ 1.5f }));
		shader.draw_object(*light.get());
		light.get()->setTranslation(tmp_translation);

	}

	//--- 오브젝트 출력
	{
		shader.Colorselect(uniform_color);
		Mesh::draw_option = drawstyle;
		int cnt{};
		
		world.render(shader);
	}

	//--- GL 오류시 출력하도록 하는 디버깅코드
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			// 오류 처리 또는 오류 메시지 출력
			std::cerr << "OpenGL 오류 발생: " << error << std::endl;
		}
	}
}

//--- 그리기 콜백 함수
GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
	//glClearColor(0.7, 0.7, 0.7, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//기존 카메라 원근 투영
	glViewport(0, 0, window_row, window_col);

	shader.use();

	// 조명 옵션 설정 
	shader.setUniform(light.get()->getTranslation(), "lightPos");
	shader.setUniform(light.get()->getColor(), "lightColor");
	shader.setUniform(light.get()->getBright(), "lightBright");
	shader.setUniform(camera.getPos(), "viewPos");


	RenderWorld(camera, perspective);

	//--- 화면에 출력하기
	glutSwapBuffers(); 
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	window_row = w;
	window_col = h;
	glViewport(0, 0, w, h);
}


bool move_dir[128]{ false };
//--- 키보드 콜백 함수
GLvoid Keyboard(unsigned char key, int x, int y) {
	static int shape{ 0 };
	//std::cout << key << "가 눌림" << std::endl;	
	if (!move_dir[key]) {
		switch (key) {
		//case 'q': case 'Q':
		//	glutLeaveMainLoop();
		//	break; //--- 프로그램 종료			
		default:
			game_framework.get()->handle_events(key, "DOWN");
		}
	}
	move_dir[key] = true;
	glutPostRedisplay();
}


//--- 키보드 콜백 함수
GLvoid KeyboardUp(unsigned char key, int x, int y) {
	//std::cout << key << "가 눌림" << std::endl;	
	if (move_dir[key]) {
		switch (key) {
		default:
			game_framework.get()->handle_events(key, "UP");
		}
	}
	//glutPostRedisplay();
	move_dir[key] = false;
}

//--- 키보드 특수키 콜백 함수
GLvoid specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}

//--- 마우스 콜백 함수
GLvoid Mouse(int button, int state, int x, int y) {
	GLfloat mx = static_cast<float>(x) / (window_row / 2.0f) - 1.0f;
	GLfloat my = -(static_cast<float>(y) / (window_col / 2.0f) - 1.0f);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mousex = mx;
		mousey = my;
		leftdown = true;

		game_framework.get()->handle_events(mx, my);
	}
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {	
		leftdown = false;
	}

	glutPostRedisplay();
}

//--- 마우스 드래그 함수
GLvoid Motion(int x, int y) {
	//--- 마우스 모션 함수
	GLfloat mx = static_cast<float>(x) / (window_row / 2.0f) - 1.0f;
	GLfloat my = -(static_cast<float>(y) / (window_col / 2.0f) - 1.0f);
	
	if (leftdown) {
		float dx = mx - mousex;
		float dy = my - mousey;
		
		//camera.Dir_rotate(glm::vec3(camera.getSensitivity(), camera.getSensitivity(), camera.getSensitivity()) * glm::vec3(dy, -dx, 0.0f));

		mousex = mx;
		mousey = my;

		glutPostRedisplay();
	}
}
//--- 마우스 휠 드래그 함수
GLvoid handleMouseWheel(int wheel, int direction, int x, int y) {
	// wheel: 마우스 휠의 상태를 나타내는 매개변수 (일반적으로 0)
	// direction: 마우스 휠의 방향 (1: 위로, -1: 아래로)
	// x, y: 마우스 커서의 현재 위치
	if (direction > 0) {
		//휠을 위로 굴렸을 경우
		camera.zoom_in(0.25f);
	}
	else if(direction < 0){
		//휠을 아래로 굴렸을 경우
		camera.zoom_out(0.25f);
	}
	glutPostRedisplay();
}

//타이머 변수 선언

//--- 타이머 콜백 함수
GLvoid Timer(int value) { //--- 콜백 함수: 타이머 콜백 함수

	game_framework.get()->update();

	glutPostRedisplay();	
	glutTimerFunc(20, Timer, value); // 타이머함수 재 설정
}

//----------------------------------------
//실습용 함수 정의
//----------------------------------------

//--- GPU 버퍼에 있는 내용 출력
void DebugPrintVBOContents(GLuint vbo, int numVertices, int vertexSizeInBytes) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLfloat* data = new GLfloat[numVertices * vertexSizeInBytes / sizeof(GLfloat)];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * vertexSizeInBytes, data);

	for (int i = 0; i < numVertices * vertexSizeInBytes / sizeof(GLfloat); ++i) {
		std::cout << data[i] << " ";
		if ((i + 1) % (vertexSizeInBytes / sizeof(GLfloat)) == 0) {
			std::cout << std::endl;
		}
	}

	delete[] data;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
