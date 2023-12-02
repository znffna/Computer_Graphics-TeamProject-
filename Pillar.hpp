#include "Object.hpp"

#ifndef PILLAR_HPP
#define PILLAR_HPP

class Pillar : public Object {
public:
	Pillar(glm::vec3 scale) : Object(PILLAR) {
		Object::setScale(scale);
		backup();
	}

	void handle_events(unsigned char key) override {
		switch (key) {
		case 'r': case 'R':
			rollback();
			break;
		}
	}

	void update() override {
		// 가운데 기둥은 아무것도 하지 않음.
	}

	void render() const override {
		draw();
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		// 가운데 기둥은 아무것도 하지 않음.
	}
};

#endif // !PILLAR_HPP
