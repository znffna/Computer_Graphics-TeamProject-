#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Default.hpp"
#include "Mesh.hpp"

class Object : public std::enable_shared_from_this<Object>{
private:
	void World_after_Scale(glm::mat4&) const;
	void World_after_Translation(glm::mat4&) const;
	void World_after_rotate(glm::mat4&) const;

	bool invaild{ false }; // true일시 game_world의 모든 부분에서 무시.

	glm::vec3 after_translation{ 0.0f };			//이동 백터
	glm::vec3 after_rotate{ 0.0f };			//회전 백터(각 축에대한 degree값)
	glm::vec3 after_scale{ 1.0f };			//축소/확대 백터

	glm::vec3 translation{ 0.0f };	//이동 백터
	glm::vec3 rotate{ 0.0f };			//회전 백터
	glm::vec3 scale{ 1.0f };			//축소/확대 백터

	glm::vec3 color{ 1.0f };		//오브젝트 출력 색깔

	// 출력할 3D 오브젝트
	std::shared_ptr<Mesh> mesh{ nullptr };

public: // ---- 멤버 함수 ----	
	//--- special Function
	Object(const std::shared_ptr<Mesh>&);	//생성자
	virtual ~Object();

	Object(const Object& other);
	Object& operator=(const Object& other);

	Object(Object&& other) noexcept;
	Object& operator=(Object&& other) noexcept;

	//--- operator overloading


	// Object 설정 초기화
	void changemesh(const std::shared_ptr<Mesh>&);
	void transform_reset();

	// 상태 출력
	void show_state() const;

	// interface function
	void setInvaild(bool rhs) { invaild = rhs; }
	bool getInvaild() { return invaild; }

	void setTranslation(const glm::vec3& vector);
	void setRotate(const glm::vec3& vector);
	void setScale(const glm::vec3& vector);	

	void addTranslation(const glm::vec3& vector);
	void addRotate(const glm::vec3& vector);
	void addScale(const glm::vec3& vector);


	glm::vec3 getTranslation() const ;
	glm::vec3 getRotate() const;
	glm::vec3 getScale() const;
	// 색상 변경 함수 getter / setter
	void setColor(const glm::vec3&);
	glm::vec3 getColor() const;

	// Position 변경 함수.
	void translation_rotate(const glm::vec3&);

	

	//각 과제마다 쓰는 임시 method
	bool Collision(const Object& other) const ;

	//World Transform 적용 함수
	void World_Transform(glm::mat4&) const;
	void Normal_Transform(glm::mat4&) const;

	// 실제 object를 출력
	void draw() const;

	// 다형성을 위한 기본 옵션
	void render() const;
	virtual void update();
	virtual void handle_collision(const std::string& group, const std::shared_ptr<Object>& other);
};

extern std::shared_ptr<Object> NULLPTR;

class Ball : public Object {
	float velocity{ 5.0f };		// 좌우 이동 속도(rad)
	int move_dir{ 0 };			// 좌우 이동 방향(0: 이동안함)

	float fall_velocity{ 0.0f };// 내려가는 속도 (가변)
	float fall_accelation{ 0.2f };// 중력역할
public:
	Ball(): Object(SPHERE) {}
	// interface function
	float getVelocity() { return velocity; }
	void setVelocity(float rhs) { velocity = rhs; }

	float getFallVelocity() { return fall_velocity; }
	void setFallVelocity(float rhs) { fall_velocity = rhs; }

	float getFallAccelation() { return fall_accelation; }
	void setFallAccelation(float rhs) { fall_accelation = rhs; }

	// method

	// 좌우 회전 이동
	void horizontal_move() {	
		Object::translation_rotate({ 0.0f,  move_dir * velocity, 0.0f });
	}

	// vertical_move( 상하 수직 운동 )
	void falling() {
		fall_velocity -= fall_accelation; // 시간개념은 안넣었음.
		addTranslation({ 0.0f, fall_velocity, 0.0f });
	}

	void update() override {
		//std::cout << "Ball.update() 호출" << '\n';
		horizontal_move();	// 좌우 이동 갱신
		falling();	// 상하 이동 갱신
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "Ball:Pizza") {
			//TODO 볼의 속도를 초기화
			fall_velocity = 2.0f;
		}
		if (group == "Ball:Cube") {
			//TODO 아이템을 획득
		}
	}

};

class Cube : public Object{
public:
	Cube(glm::vec3 position, glm::vec3 rotate, glm::vec3 scale) : Object(CUBE) {
		Object::setTranslation(position);
		Object::setRotate(rotate);
		Object::setScale(scale);
	}

	void update() override {
		//TODO 아이템을 계속 회전시킴.
		Object::addRotate({ 0.0f, 5.0f, 0.0f });
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "ball:cube") {
			//std::shared_ptr<Object> tmp = shared_from_this();
			setInvaild(true);
			//world.remove_object(tmp);
		}
	}
};

class Pillar : public Object {
public:
	Pillar(glm::vec3 scale) : Object(PILLAR) {
		Object::setScale(scale);
	}

	void update() override {
		// 가운데 기둥은 아무것도 하지 않음.
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		// 가운데 기둥은 아무것도 하지 않음.
	}
};




#endif