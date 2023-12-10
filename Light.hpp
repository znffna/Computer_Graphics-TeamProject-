


#ifndef LIGHT_HPP
#define LIGHT_HPP
#include "Object.hpp"

class Light : public Object {
	bool out_put{ false };
public:
	Light() : Object(CUBE) {}
	Light(const glm::vec3& pos, const glm::vec3& color) : Object(CUBE) {
		setTranslation(pos);
		setColor(color);
	}

	// interface
	void setOut(bool rhs) { out_put = rhs; }
	bool getOut() { return out_put; }

	void update() override {
		// Light�� mode Ŭ�������� update ��ų ����
	}

	void render() const override {
		if(out_put)
			draw();
	}

	void rollback() const {
		// �ƹ��͵� ���Ѵ�.
	};

	virtual void handle_events(unsigned char key, const std::string& state) override {
		// �׷��� ����.
	}

	virtual void handle_collision(const std::string& group, const std::shared_ptr<Object>& other)override {
		// �׷��� ����.
	}
};

extern std::shared_ptr<Light> light;



#endif // !LIGHT_HPP

