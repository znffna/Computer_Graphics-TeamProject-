#include "Object.hpp"

class Pillar : public Object {
public:
	Pillar(glm::vec3 scale) : Object(PILLAR) {
		Object::setScale(scale);
		backup();
	}

	void update() override {
		// 가운데 기둥은 아무것도 하지 않음.
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		// 가운데 기둥은 아무것도 하지 않음.
	}
};