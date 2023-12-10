#include "Default.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
	void setUniform(const glm::mat4& matrix, const char* uName) const;
	void setUniform(const int& int_value, const char* uName) const;

public:
	GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
	GLuint vertexShader, fragmentShader;	//--- 세이더 객체
	GLuint shaderProgramID;					//--- 셰이더 프로그램

	static bool debug;
	static bool lightOption;

	Shader();

	~Shader();
	//세이더 객체 생성용 멤버함수
	char* filetobuf(const char* );	//--- 파일을 읽어오는 함수
	//void make_shaderProgram();			//--- 세이더 프로그램 생성하기
	void make_shaderProgram(const char* vertex_pos = "vertex.glsl", const char* fragment = "fragment.glsl");	//--- 세이더 프로그램 생성하기
	void make_vertexShaders(const char*);			//--- 버텍스 세이더 객체 만들기
	void make_fragmentShaders(const char*);		//--- 프래그먼트 세이더 객체 만들기
	// 세이더 사용 함수
	void use() const;
	unsigned int getUniformLocate(const char* ) const;

	// 세이더 uniform 변경 함수
	void setColor(const glm::vec3& vertex_color = { 1.0f, 1.0f, 1.0f }, const char* uName = "uColor") const;
	void Colorselect(const int& vertex_color = 1) const;
	void setUniform(const glm::vec3& vector, const char* uName) const;


	// 세이더 조명 사용 여부
	void setLight(const bool light = true);

	// 변환값 적용 함수
	void worldTransform(const Object&);
	void worldTransform(const Object&, const glm::mat4&);
	void normalTransform(const Object& obj) const;

	void worldTransform(const Camera& camera) const;
	void viewTransform(const Camera&);
	void perspectiveTransform(const Camera&);
	void orthoTransform(const Camera&);

	// 오브젝트 출력 함수
	void draw_object(const Object&);
};

extern Shader shader;

#endif //