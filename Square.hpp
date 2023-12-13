//-------------------------------------------------------
//  Square.hpp
//  2023. 12. 02
// 
//  배경출력용
//-------------------------------------------------------

#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "Object.hpp"
class Square : public Object {
	GLuint texture{ 0 };
public:
	Square() : Object(SQUARE) {
		backup();
	}
	Square(const GLuint texture) : Object(SQUARE), texture{ texture } {
		setTexture(texture);
		backup();
	}

	void handle_events(unsigned char key, const std::string& state) override {
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
		if (texture) {
			glBindVertexArray(getVao());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		draw();
	}

	void handle_collision(const std::string& group, const std::shared_ptr<Object>& other) override {
		// 가운데 기둥은 아무것도 하지 않음.
	}
};

#endif // !SQUARE_HPP
