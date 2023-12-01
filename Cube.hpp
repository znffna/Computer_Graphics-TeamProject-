#include "Object.hpp"

class Cube : public Object {
public:
	Cube(glm::vec3 position, glm::vec3 rotate, glm::vec3 scale) : Object(CUBE) {
		Object::setTranslation(position);
		Object::setRotate(rotate);
		Object::setScale(scale);
		backup();
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
