
#ifndef CUBE_HPP
#define CUBE_HPP
#include "Object.hpp"

class Cube : public Object {
public:
	static GLuint texture;
	Cube(glm::vec3 position, glm::vec3 rotate, glm::vec3 scale) : Object(CUBE) {
		if(!texture)
			texture = (loadTexture("image1.png"));

		setTranslation(position);
		setRotate(rotate);
		setScale(scale);
		backup();
	}

	void update() override {
		//TODO 아이템을 계속 회전시킴.
		addRotate({ 0.0f, 5.0f, 0.0f });
	}

	void render() const override {

		glBindVertexArray(getVao());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		draw();
	}

	void handle_events(unsigned char key, const std::string& state) override {
		switch (key) {
		case 'r': case 'R':
			rollback();
			break;
		}
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		if (group == "ball:cube") {
			//std::shared_ptr<Object> tmp = shared_from_this();
			setInvaild(true);
			//world.remove_object(tmp);
		}
	}
};

#endif // !CUBE_HPP
