#include "Shader.hpp"

bool Shader::debug = true;
bool Shader::lightOption = true;

Shader::Shader() {	
	//--- �ҽ��ڵ� ���� ����
	vertexSource = nullptr;
	fragmentSource = nullptr;
	//--- ���̴� ��ü
	vertexShader = 0;
	fragmentShader = 0;
	//--- ���̴� ���α׷�
	shaderProgramID = 0;

}

Shader::~Shader() {
	delete[] vertexSource;
	delete[] fragmentSource;
	glDeleteProgram(shaderProgramID);
}

//--- ������ �о���� �Լ�
char* Shader::filetobuf(const char* file) {
	std::ifstream in(file, std::ios_base::binary);

	if (!in)
	{
		std::cout << file << "���� ��ã��" << std::endl;
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
	//std::cout << "buf = " << buf << '\n';	//�о�� ���� �ֿܼ� ���
	return buf;
}


//--- ���̴� ���α׷� �����ϱ�
void Shader::make_shaderProgram(const char* vertex_pos, const char* fragment) {
	make_vertexShaders(vertex_pos); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(fragment); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}
//void Shader::make_shaderProgram() {
//	make_vertexShaders("vertex.glsl"); //--- ���ؽ� ���̴� �����
//	make_fragmentShaders("fragment.glsl"); //--- �����׸�Ʈ ���̴� �����
//	//-- shader Program
//	shaderProgramID = glCreateProgram();
//	glAttachShader(shaderProgramID, vertexShader);
//	glAttachShader(shaderProgramID, fragmentShader);
//	glLinkProgram(shaderProgramID);
//	//--- ���̴� �����ϱ�
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program ����ϱ�
//	glUseProgram(shaderProgramID);
//}

//--- ���ؽ� ���̴� ��ü �����
void Shader::make_vertexShaders(const char* vertex_pos) {
	vertexSource = filetobuf(vertex_pos);
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

//--- �����׸�Ʈ ���̴� ��ü �����
void Shader::make_fragmentShaders(const char* fragment) {
	fragmentSource = filetobuf(fragment);
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void Shader::use() const {
	glUseProgram(shaderProgramID);
}

unsigned int Shader::getUniformLocate(const char* uName) const {
	GLint uniformLocation = glGetUniformLocation(shaderProgramID, uName);
	if(debug)
	{
		if (uniformLocation == -1) {
			std::cerr << uName << ": ������ ���� ã�� ����!" << std::endl;
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
void Shader::setUniform(const glm::mat4& matrix, const char* uName) const {
	glUniformMatrix4fv(getUniformLocate(uName), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setUniform(const int& int_value, const char* uName) const {
	glUniform1i(getUniformLocate(uName), int_value);
}

void Shader::setLight(const bool light) {
	lightOption = light;
	setUniform(light, "lightOption");
}



void Shader::setColor(const glm::vec3& vertex_color, const char* uniform) const {
	setUniform(vertex_color, "uColor");
}

void Shader::Colorselect(const int& vertex_color) const {
	setUniform(vertex_color, "vColor");
}



void Shader::worldTransform(const Object& obj) {
	//--- ��� �ɼ�
	glm::mat4 World_transformMatrix(1.0f);

	obj.World_Transform(World_transformMatrix);

	setUniform(World_transformMatrix, "modelTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));
	
}
void Shader::worldTransform(const Object& obj, const glm::mat4& transMatrix) {
	//--- ��� �ɼ�
	glm::mat4 World_transformMatrix(1.0f);

	World_transformMatrix = transMatrix * World_transformMatrix;

	obj.World_Transform(World_transformMatrix);

	setUniform(World_transformMatrix, "modelTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));

}

void Shader::worldTransform(const Camera& camera) const {
	//--- ��� �ɼ�
	glm::mat4 World_transformMatrix(1.0f);

	camera.World_transform(World_transformMatrix);

	setUniform(World_transformMatrix, "modelTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));

}

void Shader::normalTransform(const Object& obj) const {
	//--- ��� �ɼ�
	glm::mat4 World_transformMatrix(1.0f);

	obj.Normal_Transform(World_transformMatrix);

	setUniform(World_transformMatrix, "normalTransform");
	//glUniformMatrix4fv(getUniformLocate("modelTransform"), 1, GL_FALSE, glm::value_ptr(World_transformMatrix));

}

//�� ��ȯ
void Shader::viewTransform(const Camera& camera) {
	glm::mat4 view = glm::mat4(1.0f);
	view = camera.View_transform();
	setUniform(view, "viewTransform");
	//glUniformMatrix4fv(getUniformLocate("viewTransform"), 1, GL_FALSE, &view[0][0]);
}

//���� ���� ��ȯ
void Shader::perspectiveTransform(const Camera& camera) {
	glm::mat4 projection = camera.perspective_transform();
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- ���� ��ȯ �� ����
	setUniform(projection, "projectionTransform");
	//glUniformMatrix4fv(getUniformLocate("projectionTransform"), 1, GL_FALSE, &projection[0][0]);
}

//���� ���� ��ȯ
void Shader::orthoTransform(const Camera& camera) {
	glm::mat4 projection = camera.ortho_transform();
	//unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- ���� ��ȯ �� ����
	setUniform(projection, "projectionTransform");
	//glUniformMatrix4fv(getUniformLocate("projectionTransform"), 1, GL_FALSE, &projection[0][0]);
}


void Shader::draw_object(const Object& object) 
{
	normalTransform(object);
	worldTransform(object);
	//object.draw();
}