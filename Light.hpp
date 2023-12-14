


#ifndef LIGHT_HPP
#define LIGHT_HPP
#include "Object.hpp"

class Light : public Object {
	bool out_put{ false };
	float lightBright{ 1.0f };
public:
	Light() : Object(CUBE) {}
	Light(const glm::vec3& pos, const glm::vec3& color) : Object(CUBE) {
		setTranslation(pos);
		setColor({ color, 1.0f });
	}

	// interface
	void setOut(bool rhs) { out_put = rhs; }
	bool getOut() { return out_put; }

	void setBright(float rhs) { lightBright = rhs; }
	float getBright() { return lightBright; }

	void update() override {
		// Light는 mode 클래스에서 update 시킬 예정
	}

	void render() const override {
		if(out_put)
			draw();
	}

	void rollback() const {
		// 아무것도 안한다.
	};

	virtual void handle_events(unsigned char key, const std::string& state) override {
		// 그런거 없다.
	}

	virtual int handle_collision(const std::string& group, const std::shared_ptr<Object>& other)override {
		// 그런거 없다.
		return 0;
	}
};

extern std::shared_ptr<Light> light;



#endif // !LIGHT_HPP

