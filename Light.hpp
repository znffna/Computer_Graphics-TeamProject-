


#ifndef LIGHT_HPP
#define LIGHT_HPP
#include "Object.hpp"

class Light : public Object {
	bool out_put{ false };
public:
	Light(const glm::vec3& pos, const glm::vec3& color) : Object(CUBE) {

	}

	// interface
	void setOut(bool rhs) { out_put = rhs; }
	bool getOut() { return out_put; }

	void update() override {
		// Light는 mode 클래스에서 update 시킬 예정
	}

	void render() const override {
		if(out_put)
			draw();
	}
};

extern std::unique_ptr<Light> light;
std::unique_ptr<Light> light;



#endif // !LIGHT_HPP

