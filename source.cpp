#define _CRT_SECURE_NO_WARRINGS

#include "Default.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

//glew32.lib freeglut.lib
const char title[] = "[[팀 프로젝트]]";
const std::string User_guide[] = {
"-------------------",
"--[[팀 프로젝트]]--",
"-------------------"
//"paste_here",
};


// 사용할 obj 저장할 포인터
std::shared_ptr<Mesh> CUBE;
std::shared_ptr<Mesh> PYRAMID;
std::shared_ptr<Mesh> SPHERE;

// 사용할 obj의 파일 위치
const std::string cube_storage_location= "resource\\cube.obj";
const std::string pyramid_storage_location = "resource\\pyramid.obj";
const std::string sphere_storage_location = "resource\\sphere.obj";

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
static bool drawstyle{ true };	//false : 와이어(line) 객체/ true : 솔리드(triangle) 객체
static bool depthcheck{ true };	//은면제거 유무
static bool perspective{ true };	//투영 방식(true : 원근 투영)

//마우스 좌표값
static float mousex{ 0.0f };		//마우스의 x값
static float mousey{ 0.0f };		//마우스의 y값

//세이더 클래스 생성
static Shader shader;

//카메라 클래스 생성
static Camera camera;

// 조명 위치 적용할 Object
Object* Light;




//--------------------------------------------------------
//--- 실습용 함수 선언
//--------------------------------------------------------
GLvoid setup();				//--- main에서 최초로 생성할때 부르는 초기화 함수
void DebugPrintVBOContents(GLuint vbo, int numVertices, int vertexSizeInBytes);	//--- GPU에 있는 버퍼에서 현재 바인드된 vao에 바인드된 vbo, ebo를 가져와 콘솔에 출력함.
void Timer_option(const int&, const bool&);
void Change_switch(bool&);

//--------------------------------------------------------
//--- 실습용 전역변수 선언
//-------------------------------------------------------
std::vector<Object> world;	//--- 출력될 오브젝트 모음.

int timer_stop{ 0 };	//0일때 timer 꺼짐.
bool timers[10]{ false };	//--- 해당 타이머 스위치
bool reverse[10]{ false };//--- 해당 타이머의 역방향 여부

//------------------------------------
//메인 함수 정의
//------------------------------------

//--- main
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_row, window_col);
	glutCreateWindow(title);
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//--- 세이더 생성
	shader.make_shaderProgram();

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
	//glutKeyboardUpFunc(KeyboardUp);
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

	{	// 가져다 사용할 obj 읽어오기
		CUBE = std::make_shared<Mesh>(cube_storage_location);
		PYRAMID = std::make_shared<Mesh>(pyramid_storage_location);
		SPHERE = std::make_shared<Mesh>(sphere_storage_location);
	}

	{	//카메라 위치 초기화
		camera.reset();
		camera.setPos({ 0.0f, 0.0f, 25.0f * sqrt(2)});
	}

	{	//조명 초기화
		Light = new Object(CUBE);
		Light->setRotate({ 0.0f, 0.0f, 0.0f });
		Light->setTranslation({ 0.0f, 0.0f, 10.0f });	//light_pos
		Light->setColor({ 1.0f, 1.0f, 1.0f });			//light_color
	}
	
	{	// 오브젝트 초기화
		Object tmp(SPHERE);
		world.push_back(std::move(tmp));
	}
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
	shader.setLight(Shader::lightOption);

	//--- 기본 색상 설정
	shader.Colorselect(1);

	glFrontFace(GL_CCW);	//시계 좌표계
	
	//--- 조명 위치 출력
	{	
		shader.Colorselect(uniform_color);
		shader.setColor({Light->getColor()});
		glm::vec3 tmp_translation = Light->getTranslation();
		Light->setTranslation(Light->getTranslation() + (glm::normalize(Light->getTranslation()) * glm::vec3{ 1.5f }));
		shader.draw_object(*Light);
		Light->setTranslation(tmp_translation);

	}

	//--- 오브젝트 출력
	{
		shader.Colorselect(uniform_color);
		Mesh::draw_option = drawstyle;
		int cnt{};
		
		for (const Object& o : world) {
			shader.setColor({ o.getColor()});
			shader.draw_object(o);
			++cnt;
		}

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

	Shader::debug = true;
	// 조명 옵션 설정 
	shader.setUniform(Light->getTranslation(), "lightPos");
	shader.setUniform(Light->getColor(), "lightColor");
	shader.setUniform(camera.getPos(), "viewPos");
	Shader::debug = false;
	RenderWorld(camera, Projective_PERSPECTIVE);

	//--- 화면에 출력하기
	glutSwapBuffers(); 
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

int move_key{ 0 };
bool move_dir[]{ false, false, false, false };

//--- 키보드 콜백 함수
GLvoid Keyboard(unsigned char key, int x, int y) {
	static int shape{ 0 };
	//std::cout << key << "가 눌림" << std::endl;	
	switch (key) {
	// 조명 제거
	case 'l': case 'L':
		shader.setLight(!Shader::lightOption);
		break;
	// 은면 제거
	case 'h': case 'H':	
		depthcheck = !depthcheck;
		break;
	//투영 선택(직각/원근)
	case 'p': case 'P':	
		perspective = !perspective;
		break;
	//default option
	case 'm': case 'M':
		drawstyle = !drawstyle;
		break;
	case 'q': case 'Q': glutLeaveMainLoop(); break; //--- 프로그램 종료			
	}
	
	glutPostRedisplay();
}


//--- 키보드 콜백 함수
GLvoid KeyboardUp(unsigned char key, int x, int y) {
	//std::cout << key << "가 눌림" << std::endl;	
	switch (key) {
	default:
		break;
	}
	//glutPostRedisplay();
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
	
		camera.Pos_rotate(glm::vec3(camera.getSensitivity(), camera.getSensitivity(), camera.getSensitivity()) * glm::vec3(dy, -dx, 0.0f));

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

	////----------------카메라 변환------------------------
	//// x/X: 좌우로 이동
	//if (timers[0]) {
	//	int sign = reverse[0] ? 1 : -1;
	//	for (Object& o : world) {
	//		o.rotate.x += sign * 1.0f;
	//	}
	//}
	//// r: 조명을 객체의 중심 y축에 대하여 양/음 방향으로 공전시키기
	//if (timers[1]) {
	//	int sign = reverse[1] ? 1 : -1;
	//	Light->translation_rotate({ 0.0f, sign * 1.0f, 0.0f });
	//	Light->addRotate_y(sign * 1.0f);
	//}
	//// y / Y: 카메라가 현재 위치에서 화면 중심 y축을 기준으로 공전
	//if (timers[2]) {
	//	int sign = reverse[2] ? 1 : -1;
	//	//camera.rotate_Pos_y(sign * 5.0f);
	//	for (Object& o : world) {
	//		o.rotate.y += sign * 1.0f;
	//	}
	//}
	//// z / Z : 조명을 객체에 가깝게/멀게 이동하기
	//if (timers[3]) {
	//	int sign = reverse[3] ? 1 : -1;
	//	Object& light = *Light;
	//	glm::vec3 dir = glm::vec3{ static_cast<float>(sign * 0.03f)  } *glm::normalize(light.translation);
	//	light.translation += dir;
	//}
	
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

//--- 배경 색상을 랜덤하게 바꾸는 함수
void change_background() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> urd(0.0, 1.0);

	background_color = { urd(gen), urd(gen), urd(gen) };
}


class TIMER {
	//디버그용 출력
	void show_timerName(const unsigned int idx) {
		std::cout << "timer[" << idx << "] = " << timer_name[idx];
	}

	//디버그용 출력
	void show_boolean(const bool b) {
		std::cout << b ? " True " : " False ";
	}
public:
	size_t size{};						//--- 현재 적용된 타이머 갯수
	std::vector<bool> timers;			//--- index값의 타이머를 키고 껏는지.
	std::vector<bool> reverse;			//--- index값의 타이머에 주어지는 옵션.(대문자 = false, 소문자 = true)
	std::vector<std::string> timer_name;//--- index값의 역할 저장
	std::map <unsigned int, std::string> index;	// 타이머에 index와 이름을 연결.

	//--- Timer 를 추가함.
	void insert(const std::string& timerName, const bool& timer_power = false, const bool& timer_reverse = false) {
		timers.push_back(timer_power);
		reverse.push_back(timer_reverse);
		timer_name.push_back(timerName);
		index.insert({ size, timerName });
		size = timers.size();
	}

	void show_list() {
		for (int i = 0; i < size; ++i) {
			show_timerName(i);
			std::cout << ", 타이머 상황: ";
			show_boolean(timers[i]);
			std::cout << ", 타이머 리버스 : ";
			show_boolean(reverse[i]);
			std::cout << '\n';
		}
	}

	//--- 타이머를 변경 시킨다.
	void switching_timer(const int& idx, const bool& option) {
		if (idx >= size) {		//입력 오류 방지
			std::cout << "idx가 주어진 timers 수를 벗어남. ( " << idx << " > " << size << ") " << '\n';
			return;
		}

		if (timers[idx] and reverse[idx] == option) {	// 현재 실행중인 타이머를 누를시.
			timers[idx] = false;						// 해당 타이머를 중지 시킨다.
		}
		else if (timers[idx]) {							// 현재 실행중인 타이머인데 대/소문자 차이일 경우.
			reverse[idx] = option;						// reverse 값이 바뀐다.
		}
		else {											// 실행중이지 않은 타이머를 누를 경우.
			timers[idx] = true;							// 해당 타이머를 키고,
			reverse[idx] = option;						// 해당 reverse 값을 넣어준다.
		}
	}

	void reset() {
		timers.clear();
		reverse.clear();
		timer_name.clear();
		index.clear();
		std::cout << "Timer 리셋 완료" << '\n';
	}
};

void Timer_option(const int& type, const bool& option) {
	if (timers[type] and reverse[type] == option) {
		timers[type] = false;
	}
	else if (timers[type]) {
		reverse[type] = option;
	}
	else {
		timers[type] = true;
		reverse[type] = option;
	}
}

void Change_switch(bool& variable) {
	variable = variable == true ? false : true;
}
