#include "Default.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
	void setUniform(const glm::mat4& matrix, const char* uName) const;
	void setUniform(const int& int_value, const char* uName) const;

public:
	GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
	GLuint vertexShader, fragmentShader;	//--- ���̴� ��ü
	GLuint shaderProgramID;					//--- ���̴� ���α׷�

	static bool debug;
	static bool lightOption;

	Shader();

	~Shader();
	//���̴� ��ü ������ ����Լ�
	char* filetobuf(const char* );	//--- ������ �о���� �Լ�
	//void make_shaderProgram();			//--- ���̴� ���α׷� �����ϱ�
	void make_shaderProgram(const char* vertex_pos = "vertex.glsl", const char* fragment = "fragment.glsl");	//--- ���̴� ���α׷� �����ϱ�
	void make_vertexShaders(const char*);			//--- ���ؽ� ���̴� ��ü �����
	void make_fragmentShaders(const char*);		//--- �����׸�Ʈ ���̴� ��ü �����
	// ���̴� ��� �Լ�
	void use() const;
	unsigned int getUniformLocate(const char* ) const;

	// ���̴� uniform ���� �Լ�
	void setColor(const glm::vec3& vertex_color = { 1.0f, 1.0f, 1.0f }, const char* uName = "uColor") const;
	void Colorselect(const int& vertex_color = 1) const;
	void setUniform(const glm::vec3& vector, const char* uName) const;


	// ���̴� ���� ��� ����
	void setLight(const bool light = true);

	// ��ȯ�� ���� �Լ�
	void worldTransform(const Object&);
	void worldTransform(const Object&, const glm::mat4&);
	void normalTransform(const Object& obj) const;

	void worldTransform(const Camera& camera) const;
	void viewTransform(const Camera&);
	void perspectiveTransform(const Camera&);
	void orthoTransform(const Camera&);

	// ������Ʈ ��� �Լ�
	void draw_object(const Object&);
};

extern Shader shader;

#endif //