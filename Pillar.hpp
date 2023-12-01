#include "Object.hpp"

class Pillar : public Object {
public:
	Pillar(glm::vec3 scale) : Object(PILLAR) {
		Object::setScale(scale);
		backup();
	}

	void handle_events(unsigned char key) override {
		rollback();
	}

	void update() override {
		// ��� ����� �ƹ��͵� ���� ����.
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		// ��� ����� �ƹ��͵� ���� ����.
	}
};