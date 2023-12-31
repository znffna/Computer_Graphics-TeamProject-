#include "Shader.hpp"

Shader shader;

bool Shader::debug = false;
bool Shader::lightOption = true;

Shader::Shader() {	
	//--- 소스코드 저장 변수
	vertexSource = nullptr;
	fragmentSource = nullptr;
	//--- 세이더 객체
	vertexShader = 0;
	fragmentShader = 0;
	//--- 셰이더 프로그램
	shaderProgramID = 0;

}

Shader::~Shader() {
	delete[] vertexSource;
	delete[] fragmentSource;
	glDeleteProgram(shaderProgramID);
}

//--- 파일을 읽어오는 함수
char* Shader::filetobuf(const char* file) {
	std::ifstream in(file, std::ios_base::binary);

	if (!in)
	{
		std::cout << file << "파일 못찾음" << std::endl;
		exit(1);
	}

	in.seekg(0, std::ios_base::end);
	long len = in.tellg();
	char* buf = new char[len + 1];
	in.seekg(0, std::ios_base::beg);	//orignal
	//in.seekg(3, std::ios_base::beg);

	int cnt = -1;
	while (in >> std::noskipws >> buf[++cnt]) {}
	buf[len] = 0;
	//std::cout << "buf = " << buf << '\n';	//읽어온 파일 콘솔에 출력
	return buf;
}


//--- 세이더 프로그램 생성하기
void Shader::make_shaderProgram(const char* vertex_pos, const char* fragment) {
	make_vertexShaders(vertex_pos); //--- 버텍스 세이더 만들기
	make_fragmentShaders(fragment); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}
//void Shader::make_shaderProgram() {
//	make_vertexShaders("vertex.glsl"); //--- 버텍스 세이더 만들기
//	make_fragmentShaders("fragment.glsl"); //--- 프래그먼트 세이더 만들기
//	//-- shader Program
//	shaderProgramID = glCreateProgram();
//	glAttachShader(shaderProgramID, vertexShader);
//	glAttachShader(shaderProgramID, fragmentShader);
//	glLinkProgram(shaderProgramID);
//	//--- 세이더 삭제하기
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program 사용하기
//	glUseProgram(shaderProgramID);
//}

//--- 버텍스 세이더 객체 만들기
void Shader::make_vertexShaders(const char* vertex_pos) {
	vertexSource = filetobuf(vertex_pos);
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

//--- 프래그먼트 세이더 객체 만들기
void Shader::make_fragmentShaders(const char* fragment) {
	fragmentSource = filetobuf(fragment);
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void Shader::use() const {
	glUseProgram(shaderProgramID);
}

unsigned int Shader::getUniformLocate(const char* uName) const {
	use();
	GLint uniformLocation = glGetUniformLocation(shaderProgramID, uName);
	if(debug)
	{
		if (uniformLocation == -1) {
			std::cerr << uName << ": 유니폼 변수 찾기 실패!" << std::endl;
			//std::cerr << uName << "- Failed to get uniform variable location!" << std::endl;
		}
		else {
			//std::cout << uName << "- Uniform variable location: " << uniformLocation << std::endl;
		}
	}
	return uniformLocation;
	//return glGetUniformLocation(shaderProgramID, uName);
}

void Shader::setUniform(const glm::vec3& vector, const char* uName) const {
	glUniform3fv(getUniformLocate(uName), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const glm::vec4& vector, const char* uName) const {
	if (debug)
		std::cout << uName << " = {" << vector.r << ", " << vector.g << ", " << vector.b << ", " << vector.a << "} " << '\n';
	glUniform4fv(getUniformLocate(uName), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const glm::mat4& matrix, const char* uName) const {
	glUniformMatrix4fv(getUniformLocate(uName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const int& int_value, const char* uName) const {
	if (debug)
		std::cout << uName << " = {" << int_value << "} " << '\n';
	glUniform1i(getUniformLocate(uName), int_value);
}

void Shader::setUniform(const float& float_value, const char* uName) const {
	if (debug)
		std::cout << uName << " = {" << float_value << "} " << '\n';
	glUniform1f(getUniformLocate(uName), float_value);
}

void Shader::setLight(const bool light) {
	lightOption = light;
	setUniform(light, "lightOption");
}

void Shader::setColor(const glm::vec3& vertex_color, const char* uniform) const {
	setUniform(vertex_color, "uColor");
}
void Shader::setColor(const glm::vec4& vertex_color, const char* uniform) const {
	setUniform(vertex_color, "uColor");
}

void Shader::Colorselect(const int& vertex_color) const {
	setUniform(vertex_color, "vColor");
}



void Shader::worldTransform(const Object& obj) {
	//--- 출력 옵션
	glm::mat4 World_transformMatrix(1.0f);

	obj.World_Transform(World_transformMatrix);

	setUniform(World_transformMatrix, "modelTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));
	
}
void Shader::worldTransform(const Object& obj, const glm::mat4& transMatrix) {
	//--- 출력 옵션
	glm::mat4 World_transformMatrix(1.0f);

	World_transformMatrix = transMatrix * World_transformMatrix;

	obj.World_Transform(World_transformMatrix);

	setUniform(World_transformMatrix, "modelTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));

}

void Shader::worldTransform(const Camera& camera) const {
	//--- 출력 옵션
	glm::mat4 World_transformMatrix(1.0f);

	camera.World_transform(World_transformMatrix);

	setUniform(World_transformMatrix, "modelTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));

}

void Shader::normalTransform(const Object& obj) const {
	//--- 출력 옵션
	glm::mat4 World_transformMatrix(1.0f);

	obj.Normal_Transform(World_transformMatrix);

	setUniform(World_transformMatrix, "normalTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));

}

//뷰 변환
void Shader::viewTransform(const Camera& camera) {
	glm::mat4 view = glm::mat4(1.0f);
	view = camera.View_transform();
	setUniform(view, "viewTransform");
	//glUniformMatrix4fv(getUniformLocate("viewTransform"), 1, GL_FALSE, &view[0][0]);
}

//원근 투영 변환
void Shader::perspectiveTransform(const Camera& camera) {
	glm::mat4 projection = camera.perspective_transform();
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
	setUniform(projection, "projectionTransform");
	//glUniformMatrix4fv(getUniformLocate("projectionTransform"), 1, GL_FALSE, &projection[0][0]);
}

//직각 투영 변환
void Shader::orthoTransform(const Camera& camera) {
	glm::mat4 projection = camera.ortho_transform();
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
	setUniform(projection, "projectionTransform");
	//glUniformMatrix4fv(getUniformLocate("projectionTransform"), 1, GL_FALSE, &projection[0][0]);
}


void Shader::draw_object(const Object& object) 
{
	normalTransform(object);
	worldTransform(object);
	//object.draw();
}