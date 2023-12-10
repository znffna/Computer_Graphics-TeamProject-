#include "Mesh.hpp"
#include "Default.hpp"
#include "Shader.hpp"

bool Mesh::debug = false;
bool Mesh::draw_option = false;

// ����� obj ������ ������
std::shared_ptr<Mesh> CUBE;
std::shared_ptr<Mesh> PYRAMID;
std::shared_ptr<Mesh> SPHERE;
std::shared_ptr<Mesh> PIZZA;
std::shared_ptr<Mesh> PILLAR;


// ����� obj�� ���� ��ġ
const std::string cube_storage_location = "resource\\cube.obj";
const std::string pyramid_storage_location = "resource\\pyramid.obj";
const std::string sphere_storage_location = "resource\\sphere.obj";
const std::string pizza_storage_location = "resource\\pizza.obj";
const std::string pillar_storage_location = "resource\\pillar.obj";

void Read_ObjectFile(){
	// ������ ����� obj �о����
	CUBE = std::make_shared<Mesh>(cube_storage_location);
	PYRAMID = std::make_shared<Mesh>(pyramid_storage_location);
	SPHERE = std::make_shared<Mesh>(sphere_storage_location);
	PIZZA = std::make_shared<Mesh>(pizza_storage_location);	
	PILLAR = std::make_shared<Mesh>(pillar_storage_location);
}

//--- ������
Mesh::Mesh() {
	name = "None";

	vao = 0;
	for (GLuint& i : vbo) i = 0;
	ebo = 0;

	vertex_pos.clear();
	vertex_normal.clear();
	vertex_color.clear();
	uvs.clear();

	vertex_index.clear();
	uv_index.clear();
	normal_index.clear();
}
Mesh::Mesh(const std::string& filename)
{
	genGPUbuffers();
	ReadObj(filename.c_str());
	push_GPU();
}
//�Ҹ���
Mesh::~Mesh() {
	//�ϴ� ���׳��ϱ� �ּ�ó��
	delGPUbuffers();
	vertex_pos.clear();
	vertex_normal.clear();
	vertex_color.clear();
	uvs.clear();

	vertex_index.clear();
	uv_index.clear();
	normal_index.clear();
}
//���� ������
Mesh::Mesh(const Mesh& other) {
	std::cout << "mesh���� ������ �Ҹ�" << '\n';
	for (const glm::vec3& v : other.vertex_pos) {
		vertex_pos.push_back(v);
	}
	for (const glm::vec3& v : other.vertex_normal) {
		vertex_normal.push_back(v);
	}
	for (const glm::vec4& v : other.vertex_color) {
		vertex_color.push_back(v);
	}
	for (const glm::vec2& v : other.uvs) {
		uvs.push_back(v);
	}

	for (const unsigned int& i : other.vertex_index) {
		vertex_index.push_back(i);
	}
	for (const unsigned int& i : other.uv_index) {
		uv_index.push_back(i);
	}
	for (const unsigned int& i : other.normal_index) {
		normal_index.push_back(i);
	}

	push_GPU();

	name = other.name;

	origin_translation = other.origin_translation;
	origin_scale = other.origin_scale;
}
//���� �Ҵ� ������
Mesh& Mesh::operator=(const Mesh& other) {
	if (this != &other) {
		//������ ������ �ִ� GPU�� vector �ʱ�ȭ
		delGPUbuffers();
		vertex_pos.clear();
		vertex_normal.clear();
		vertex_color.clear();
		uvs.clear();

		vertex_index.clear();
		uv_index.clear();
		normal_index.clear();

		// ���� ���� ����
		for (const glm::vec3& v : other.vertex_pos) {
			vertex_pos.push_back(v);
		}
		for (const glm::vec3& v : other.vertex_normal) {
			vertex_normal.push_back(v);
		}
		for (const glm::vec4& v : other.vertex_color) {
			vertex_color.push_back(v);
		}
		for (const glm::vec2& v : other.uvs) {
			uvs.push_back(v);
		}

		for (const unsigned int& i : other.vertex_index) {
			vertex_index.push_back(i);
		}
		for (const unsigned int& i : other.uv_index) {
			uv_index.push_back(i);
		}
		for (const unsigned int& i : other.normal_index) {
			normal_index.push_back(i);
		}

		push_GPU();

		name = other.name;

		origin_translation = other.origin_translation;
		origin_scale = other.origin_scale;
	}
	return *this;
}
//�̵� ������
Mesh::Mesh(Mesh&& other) noexcept {
	for (const glm::vec3& v : other.vertex_pos) {
		vertex_pos.push_back(v);
	}
	for (const glm::vec3& v : other.vertex_normal) {
		vertex_normal.push_back(v);
	}
	for (const glm::vec4& v : other.vertex_color) {
		vertex_color.push_back(v);
	}
	for (const glm::vec2& v : other.uvs) {
		uvs.push_back(v);
	}

	for (const unsigned int& i : other.vertex_index) {
		vertex_index.push_back(i);
	}
	for (const unsigned int& i : other.uv_index) {
		uv_index.push_back(i);
	}
	for (const unsigned int& i : other.normal_index) {
		normal_index.push_back(i);
	}

	// ���� ���� ����
	vao = other.vao;
	vbo[0] = other.vbo[0];
	vbo[1] = other.vbo[1];
	vbo[2] = other.vbo[2];
	ebo = other.ebo;

	name = other.name;

	origin_translation = other.origin_translation;
	origin_scale = other.origin_scale;

	// ���� ������ �ִ��͵��� ����
	other.vertex_pos.clear();
	other.vertex_normal.clear();
	other.vertex_color.clear();
	other.uvs.clear();

	other.vertex_index.clear();
	other.uv_index.clear();
	other.normal_index.clear();

	other.existVao = false;
	other.existVbo = false;
	other.existEbo = false;

	other.vao = -1;
	other.vbo[0] = -1;
	other.vbo[1] = -1;
}
//�̵� �Ҵ� ������
Mesh& Mesh::operator=(Mesh&& other) noexcept {
	if (this != &other) {
		delGPUbuffers();
		vertex_pos.clear();
		vertex_normal.clear();
		vertex_color.clear();
		uvs.clear();

		vertex_index.clear();
		uv_index.clear();
		normal_index.clear();



		for (const glm::vec3& v : other.vertex_pos) {
			vertex_pos.push_back(v);
		}
		for (const glm::vec3& v : other.vertex_normal) {
			vertex_normal.push_back(v);
		}
		for (const glm::vec4& v : other.vertex_color) {
			vertex_color.push_back(v);
		}
		for (const glm::vec2& v : other.uvs) {
			uvs.push_back(v);
		}

		for (const unsigned int& i : other.vertex_index) {
			vertex_index.push_back(i);
		}
		for (const unsigned int& i : other.uv_index) {
			uv_index.push_back(i);
		}
		for (const unsigned int& i : other.normal_index) {
			normal_index.push_back(i);
		}
		// ���� ���� ����
		vao = other.vao;
		vbo[0] = other.vbo[0];
		vbo[1] = other.vbo[1];
		vbo[2] = other.vbo[2];
		ebo = other.ebo;

		name = other.name;

		origin_translation = other.origin_translation;
		origin_scale = other.origin_scale;

		// ���� ������ �ִ��͵��� ����
		other.vertex_pos.clear();
		other.vertex_normal.clear();
		other.vertex_color.clear();
		other.uvs.clear();

		other.vertex_index.clear();
		other.uv_index.clear();
		other.normal_index.clear();

		other.existVao = false;
		other.existVbo = false;
		other.existEbo = false;

		other.vao = -1;
		other.vbo[0] = -1;
		other.vbo[1] = -1;
		other.vbo[2] = -1;
		other.vbo[3] = -1;

	}
	return *this;
}

//GPU option
void Mesh::genVao() {
	if (!existVao) {
		glGenVertexArrays(1, &vao);
		existVao = true;
	}
}
void Mesh::delVao() {
	if (existVao) {
		glDeleteVertexArrays(1, &vao);
		existVao = false;
	}
}

void Mesh::genVbo() {
	if (!existVbo) {
		glGenBuffers(4, vbo);
		existVbo = true;
	}
}
void Mesh::delVbo() {
	if (existVbo) {
		glDeleteBuffers(4, vbo);
		existVbo = false;
	}
}

void Mesh::genEbo() {
	if (!existEbo) {
		glGenBuffers(1, &ebo);
		existEbo = true;
	}
}
void Mesh::delEbo() {
	if (existEbo) {
		glDeleteBuffers(1, &ebo);
		existEbo = false;
	}
}

void Mesh::genGPUbuffers() {
	genVao();
	genVbo();
	genEbo();
}
void Mesh::delGPUbuffers() {
	delEbo();
	delVbo();
	delVao();
}

// cpu memory�� ����� ������ GPU�� Update
void Mesh::push_GPU() {

	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uv;

	std::vector<unsigned int> real_index;
	for (int i = 0; i < vertex_index.size(); ++i) {
		real_index.push_back(i);
	}

	{	// vertex_index �� �°� vertexs ����
		if (debug)
			std::cout << "face �� ��� ---" << '\n';
		for (int i = 0; i < vertex_index.size(); ++i) {
			if (debug)
				std::cout << "vertex_index[" << i << "] : " << vertex_index[i] << '\n';
			vertexs.push_back(vertex_pos[vertex_index[i]]);
			colors.push_back(vertex_color[vertex_index[i]]);
			if (debug) {
				std::cout << i << "��° ���� ��ġ: "; print_vec3(vertex_pos[vertex_index[i]]);
			}
		}
		if (debug)
			std::cout << "vertexs �� ũ�� : " << vertexs.size() << '\n';
	}

	{	// normal_index �� �°� normals ����
		if (debug)
			std::cout << "face �� ��� ---" << '\n';
		for (int i = 0; i < normal_index.size(); ++i) {
			normals.push_back(vertex_normal[normal_index[i]]);
			if (debug) {
				std::cout << i << "��° normal�� : "; print_vec3(vertex_normal[normal_index[i]]);
			}
		}
		if (debug)
			std::cout << "normals �� ũ�� : " << normals.size() << '\n';
	}

	{
		// uvs index �� �´� uvs ����
		for (int i = 0; i < uv_index.size(); ++i) {
			uv.push_back(uvs[uv_index[i]]);
		}
	}


	{
		glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�


		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- ���� vertex_pos���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
		glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(glm::vec3), vertexs.data(), GL_STATIC_DRAW);
		//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute �ε��� 0���� ��밡���ϰ� ��
		glEnableVertexAttribArray(0);

		//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (���)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- ���� normal���� ���ؽ� ����� �����Ѵ�.
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute �ε��� 1���� ��� �����ϰ� ��.
		glEnableVertexAttribArray(1);

		//--- 3��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
		//--- ������ attribute �ε��� 2���� ����Ѵ�: ���ؽ� �� 4*float
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute �ε��� 2���� ��� �����ϰ� ��.
		glEnableVertexAttribArray(2);

		//--- 3��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
		//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
		glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), uv.data(), GL_STATIC_DRAW);
		//--- ������ attribute �ε��� 2���� ����Ѵ�: ���ؽ� �� 2*float
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute �ε��� 2���� ��� �����ϰ� ��.
		glEnableVertexAttribArray(3);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, real_index.size() * sizeof(unsigned int), real_index.data(), GL_STATIC_DRAW);

		glBindVertexArray(0); //--- VAO�� ���ε��ϱ�
	}

	//--- GL ������ ����ϵ��� �ϴ� ������ڵ�
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			// ���� ó�� �Ǵ� ���� �޽��� ���
			std::cerr << "OpenGL ���� �߻�: " << error << std::endl;
		}
	}
}

//---���� ä���� ���
void Mesh::Fill_Draw() const 
{
	if (vertex_pos.size() < 3) {
		if (vertex_pos.size() == 1) {
			glDrawArrays(GL_POINTS, 0, 1);
		}
		else {
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
	else {
		for (int i = 0; i < vertex_index.size() / 3; i++) {

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
		}
	}
}

//---������ ���
void Mesh::LINE_Draw() const 
{
	if (vertex_pos.size() < 3) {
		if (vertex_pos.size() == 1) {
			glDrawArrays(GL_POINTS, 0, 1);
		}
		else {
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
	else {		
		for (int i = 0; i < vertex_index.size() / 3; i++) {
			glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
		}		
	}
}

//---���� ������ ���� �ִ�ġ�� ���
void Mesh::AUTO_Draw() const {
	glBindVertexArray(vao);

	if (vertex_pos.size() < 3) {
		if (vertex_pos.size() == 1) {
			glDrawArrays(GL_POINTS, 0, 1);
		}
		else {
			glDrawArrays(GL_LINES, 0, 2);
		}
	}
	else {
		if (draw_option) {
			glDrawElements(GL_TRIANGLES, vertex_index.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			for (int i = 0; i < vertex_index.size() / 3; i++) {
				glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(i * 3 * sizeof(unsigned int)));
			}
		}
	}
}

//---mesh�� ������ ����
void Mesh::setMesh(const std::string& filename)
{
	ReadObj(filename.c_str());
	push_GPU();
}

//---���õ� ������ ����
void Mesh::clear() {
	if (exist()) {
		//polygon�� genbuffer�� ����ÿ� ���� ����.(����� ��)
		//glDeleteBuffers(1, &ebo);
		//glDeleteBuffers(2, vbo);
		//glDeleteVertexArrays(1, &vao);
	}
	name = "None";
	vertex_pos.clear();

	vao = 0;
	for (GLuint& i : vbo) i = 0;
	//for (GLuint& i : ebo) i = 0;
	ebo = 0;
	origin_translation = { 0.0f, 0.0f, 0.0f };
	origin_scale = { 1.0f, 1.0f, 1.0f };
}

//.obj ���� �б� ���� (������ ���� ���� �ʿ�)
void Mesh::ReadObj(const char* filename) {
	//std::cout << "filename : '" << filename << "'" << '\n';
	std::ifstream file(filename);

	if (!file) {
		std::cout << filename << "##���� ���� ���� ##" << '\n';
		return;
	}

	//--- 1. ���� ������ ����
	vertex_pos.clear();
	vertex_normal.clear();
	vertex_color.clear();
	uvs.clear();

	vertex_index.clear();
	uv_index.clear();
	normal_index.clear();

	//--- 2. �Ҵ�� �޸𸮿� �� ���ؽ�, index, uv ���� �Է�

	//if (!file.is_open()) {
	//	std::cerr << "Failed to open the file." << std::endl;
	//	return;
	//}

	//std::vector<std::vector<int>> faces;  // �� ������
	glm::vec3 vertex_size[2];	//0 : min(x, y, z), 1 : max(x, y, z) 

	size_t count{};
	while (!file.eof()) {
		char lineHeader;
		file >> lineHeader;

		if (lineHeader == 'v') {
			char tmp = file.get();
			if (tmp == ' ') {
				glm::vec3 vertex;
				file >> vertex.x >> vertex.y >> vertex.z;
				vertex_pos.push_back(vertex);
				vertex_color.push_back(glm::vec4{ (glm::vec3{ 0.5f } *(vertex + glm::vec3{ 1.0f })),1.0f });
				if (debug)
					std::cout << "vertex_pos.size() : " << vertex_pos.size() << '\n';
			}
			else if (tmp == 't') {
				glm::vec2 uv;
				file >> uv.x >> uv.y;
				uvs.push_back(uv);
				if (debug)
					std::cout << "uvs.size() : " << uvs.size() << '\n';
			}
			else if (tmp == 'n') {
				glm::vec3 normal;
				file >> normal.x >> normal.y >> normal.z;
				vertex_normal.push_back(normal);
				if (debug)
					std::cout << "vertex_normal.size() : " << vertex_normal.size() << '\n';
			}
		}
		else if (lineHeader == 'f') {
			//std::vector<int> faceIndices;
			int vertexIndex, uvIndex, normalIndex;
			char slash;

			for (int i = 0; i < 3; ++i) {
				file >> vertexIndex >> slash >> uvIndex >> slash >> normalIndex;
				vertex_index.push_back(vertexIndex - 1);
				uv_index.push_back(uvIndex - 1);
				normal_index.push_back(normalIndex - 1);

				if (debug)
				{
					std::cout << "vertex_index.size() : " << vertex_index.size() << '\n';
					std::cout << "vertex_index[" << vertex_index.size() - 1 << "]: " << vertex_index[vertex_index.size() - 1] << '\n';
				}
				// faceIndices.push_back(vertexIndex);
				// ���⼭ �ؽ�ó ��ǥ�� ����� �ʿ��ϴٸ� ó�� �߰�				
			}
			if (debug)
			{
			}
			//faces.push_back(faceIndices);
		}
		else if (lineHeader == 'g') {
			file >> name;
			if (debug) {
				std::cout << "polygon_name : " << name << '\n';
			}
		}
		else {
			// �ٸ� ������ ���, �׳� �Ѿ
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	file.close();

	if (vertex_normal.size() == 0) {
		if (debug) {
			std::cout << "obj���Ͽ� ���� ��� �������� ���� ..." << '\n';
			std::cout << "���� ������ ..." << '\n';
		}

		for (int i = 0; i < vertex_index.size() / 3; ++i) {
			if (debug)
				std::cout << "i = " << i << ", normal : ";

			glm::vec3 vector[2];
			vector[0] = vertex_pos[vertex_index[3 * i + 1]] - vertex_pos[vertex_index[3 * i]];
			vector[1] = vertex_pos[vertex_index[3 * i + 2]] - vertex_pos[vertex_index[3 * i]];
			glm::vec3 normal = glm::cross(vector[0], vector[1]);
			normal = glm::normalize(normal);
			if (debug) {

				print_vec3(vertex_pos[vertex_index[3 * i]]);
				print_vec3(vertex_pos[vertex_index[3 * i + 1]]);
				print_vec3(vertex_pos[vertex_index[3 * i + 2]]);
				std::cout << " : ";
				print_vec3(normal);
			}

			vertex_normal.push_back(normal);
			normal_index.push_back(i);
			normal_index.push_back(i);
			normal_index.push_back(i);
		}

	}



	//--- 3. ����� ���� debug ���
	if (debug) {
		std::cout << "vertex_pos --" << vertex_pos.size() << '\n';
		for (const glm::vec3& v : vertex_pos) {
			print_vec3(v);
		}
		std::cout << "vertex_normal --" << vertex_normal.size() << '\n';
		for (const glm::vec3& v : vertex_normal) {
			print_vec3(v);
		}
		std::cout << "vertex_color --" << vertex_color.size() << '\n';
		for (const glm::vec4& v : vertex_color) {
			print_vec3(v);
		}
		std::cout << "uvs --" << uvs.size() << '\n';
		for (const glm::vec2& v : uvs) {
			std::cout << v.x << ", " << v.y << '\n';
		}
		int cnt{};
		std::cout << "vertex_index --" << vertex_index.size() << '\n';
		for (const unsigned int& ui : vertex_index) {
			std::cout << ui;
			++cnt;
			if (cnt % 3 == 0) {
				std::cout << '\n';
			}
			else {
				std::cout << ", ";
			}
		}
		std::cout << "uv_index --" << uv_index.size() << '\n';
		for (const unsigned int& ui : uv_index) {
			std::cout << ui;
			++cnt;
			if (cnt % 3 == 0) {
				std::cout << '\n';
			}
			else {
				std::cout << ", ";
			}
		}
		std::cout << "normal_index --" << normal_index.size() << '\n';
		for (const unsigned int& ui : normal_index) {
			std::cout << ui;
			++cnt;
			if (cnt % 3 == 0) {
				std::cout << '\n';
			}
			else {
				std::cout << ", ";
			}
		}
	}

	//char count[128];
	//int vertexnum = 0;
	//int facenum = 0;
	//int uvnum = 0;
	////--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	//while (!feof(path)) {
	//	fscanf(path, "%s", count);
	//	if (count[0] == 'v' && count[1] == '\0')
	//		vertexnum++;
	//	else if (count[0] == 'f' && count[1] == '\0')
	//		facenum++;
	//	else if (count[0] == 'v' && count[1] == 't' && count[3] == '\0')
	//		uvnum++;
	//	memset(count, '\0', sizeof(count));
	//}
	//rewind(path);
	//int vertIndex = 0;
	//int faceIndex = 0;
	//int uvIndex = 0;
	////--- 2. �޸� �Ҵ�
	//glm::vec3* vertex = new glm::vec3[vertexnum]();
	//glm::vec3* color = new glm::vec3[vertexnum]();
	//glm::uvec3* face = new glm::uvec3[facenum]();
	//glm::vec3* uvdata = new glm::vec3[facenum]();
	//glm::vec3* normal = new glm::vec3[facenum]();
	//glm::vec2* uv = new glm::vec2[uvnum]();
	////std::cout << "2. �޸� �Ҵ� �Ϸ�" << '\n';
	//char bind[128];
	////--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽�, uv ���� �Է�
	//while (!feof(path)) {
	//	fscanf(path, "%s", bind);
	//	if (bind[0] == 'v' && bind[1] == '\0') {
	//		fscanf(path, "%f %f %f\n",
	//			&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
	//		{
	//			//����(����)
	//			{/*
	//				std::random_device rd;
	//				std::mt19937 gen(rd());
	//				std::uniform_real_distribution<float> urd(0.0, 1.0);
	//				color[vertIndex].r = urd(gen);
	//				color[vertIndex].g = urd(gen);
	//				color[vertIndex].b = urd(gen);
	//			*/}
	//			//����(���� ��ġ�� ����)
	//			{
	//				color[vertIndex].r = vertIndex % 2 == 0 ? 0.0f : 1.0f;
	//				color[vertIndex].g = (vertIndex/2) % 2 == 0 ? 0.0f : 1.0f;
	//				color[vertIndex].b = (vertIndex/4) % 2 == 0 ? 0.0f : 1.0f;
	//			}
	//		}
	//		vertIndex++;
	//	}
	//	else if (bind[0] == 'f' && bind[1] == '\0') {
	//		unsigned int temp_face[3], temp_uv[3], temp_normal[3];
	//		/*fscanf(path, "%d//%d//%d %d//%d//%d %d//%d//%d\n",
	//			&temp_face[0], &temp_uv[0], &temp_normal[0],
	//			&temp_face[1], &temp_uv[1], &temp_normal[1],
	//			&temp_face[2], &temp_uv[2], &temp_normal[2]);*/
	//		fscanf(path, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
	//			&temp_face[0], &temp_normal[0],
	//			&temp_face[1], &temp_normal[1],
	//			&temp_face[2], &temp_normal[2]);
	//		face[faceIndex].x = temp_face[0] - 1;
	//		face[faceIndex].y = temp_face[1] - 1;
	//		face[faceIndex].z = temp_face[2] - 1;
	//		uvdata[faceIndex].x = temp_uv[0];
	//		uvdata[faceIndex].y = temp_uv[1];
	//		uvdata[faceIndex].z = temp_uv[2];
	//		normal[faceIndex].x = temp_normal[0];
	//		normal[faceIndex].y = temp_normal[1];
	//		normal[faceIndex].z = temp_normal[2];
	//		faceIndex++;
	//	}
	//	else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
	//		fscanf(path, "%f %f\n", &uv[uvIndex].x, &uv[uvIndex].y);
	//		uvIndex++;
	//	}
	//}
	//fclose(path);

	//--- �ʿ��� ��� �о�� ���� ���� ���� � ����
	push_GPU();
}

bool Mesh::exist() const {
	if (!name.compare("None")) {
		//std::cout << "���� mesh�� ����� �� ����." << '\n';
		return false;
	}
	//std::cout << "���� mesh�� ����� �� :" << name << '\n';
	return true;
}

