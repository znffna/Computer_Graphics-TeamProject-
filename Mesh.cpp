#include "Mesh.hpp"
#include "Default.hpp"
#include "Shader.hpp"

bool Mesh::debug = true;
bool Mesh::draw_option = true;

//--- 생성자
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

	origin_translation = { 0.0f, 0.0f, 0.0f };
	origin_scale = { 1.0f, 1.0f, 1.0f };
}
//소멸자
Mesh::~Mesh() {
	//일단 버그나니까 주석처리
	delGPUbuffers();
	vertex_pos.clear();
	vertex_normal.clear();
	vertex_color.clear();
	uvs.clear();

	vertex_index.clear();
	uv_index.clear();
	normal_index.clear();
}
//복사 생성자
Mesh::Mesh(const Mesh& other) {
	std::cout << "mesh복사 생성자 불림" << '\n';
	for (const glm::vec3& v : other.vertex_pos) {
		vertex_pos.push_back(v);
	}
	for (const glm::vec3& v : other.vertex_normal) {
		vertex_normal.push_back(v);
	}
	for (const glm::vec3& v : other.vertex_color) {
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
//복사 할당 연산자
Mesh& Mesh::operator=(const Mesh& other) {
	if (this != &other) {
		//기존에 가지고 있던 GPU랑 vector 초기화
		delGPUbuffers();
		vertex_pos.clear();
		vertex_normal.clear();
		vertex_color.clear();
		uvs.clear();

		vertex_index.clear();
		uv_index.clear();
		normal_index.clear();

		// 깊은 복사 진행
		for (const glm::vec3& v : other.vertex_pos) {
			vertex_pos.push_back(v);
		}
		for (const glm::vec3& v : other.vertex_normal) {
			vertex_normal.push_back(v);
		}
		for (const glm::vec3& v : other.vertex_color) {
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
//이동 생성자
Mesh::Mesh(Mesh&& other) noexcept {
	for (const glm::vec3& v : other.vertex_pos) {
		vertex_pos.push_back(v);
	}
	for (const glm::vec3& v : other.vertex_normal) {
		vertex_normal.push_back(v);
	}
	for (const glm::vec3& v : other.vertex_color) {
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

	// 얇은 복사 진행
	vao = other.vao;
	vbo[0] = other.vbo[0];
	vbo[1] = other.vbo[1];
	vbo[2] = other.vbo[2];
	ebo = other.ebo;

	name = other.name;

	origin_translation = other.origin_translation;
	origin_scale = other.origin_scale;

	// 이제 기존에 있던것들을 제거
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
//이동 할당 생성자
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
		for (const glm::vec3& v : other.vertex_color) {
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
		// 얇은 복사 진행
		vao = other.vao;
		vbo[0] = other.vbo[0];
		vbo[1] = other.vbo[1];
		vbo[2] = other.vbo[2];
		ebo = other.ebo;

		name = other.name;

		origin_translation = other.origin_translation;
		origin_scale = other.origin_scale;

		// 이제 기존에 있던것들을 제거
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
		glGenBuffers(3, vbo);
		existVbo = true;
	}
}
void Mesh::delVbo() {
	if (existVbo) {
		glDeleteBuffers(3, vbo);
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

// cpu memory에 저장된 정보를 GPU에 Update
void Mesh::push_GPU() {

	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;

	std::vector<unsigned int> real_index;
	for (int i = 0; i < vertex_index.size(); ++i) {
		real_index.push_back(i);
	}

	{	// vertex_index 에 맞게 vertexs 생성
		if (debug)
			std::cout << "face 값 출력 ---" << '\n';
		for (int i = 0; i < vertex_index.size(); ++i) {
			if (debug) 
				std::cout << "vertex_index[" << i << "] : " << vertex_index[i] << '\n';
			vertexs.push_back(vertex_pos[vertex_index[i]]);
			colors.push_back(vertex_color[vertex_index[i]]);
			if (debug) {
				std::cout << i << "번째 정점 위치: "; print_vec3(vertex_pos[vertex_index[i]]);
			}
		}
		if (debug)
			std::cout << "vertexs 의 크기 : " << vertexs.size() << '\n';
	}

	{	// normal_index 에 맞게 normals 생성
		if (debug)
			std::cout << "face 값 출력 ---" << '\n';
		for (int i = 0; i < normal_index.size(); ++i) {
			normals.push_back(vertex_normal[normal_index[i]]);
			if (debug) {
				std::cout << i << "번째 normal값 : "; print_vec3(vertex_normal[normal_index[i]]);
			}
		}
		if (debug)
			std::cout << "normals 의 크기 : " << normals.size() << '\n';
	}

	{
		genGPUbuffers();
		glBindVertexArray(vao); //--- VAO를 바인드하기


		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- 변수 vertex_pos에서 버텍스 데이터 값을 버퍼에 복사한다.
		glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(glm::vec3), vertexs.data(), GL_STATIC_DRAW);
		//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 0번을 사용가능하게 함
		glEnableVertexAttribArray(0);

		//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (노멀)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- 변수 normal에서 버텍스 노멀을 복사한다.
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

		//--- 3번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		//--- 변수 colors에서 버텍스 색상을 복사한다.
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 2번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 2번을 사용 가능하게 함.
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, real_index.size() * sizeof(unsigned int), real_index.data(), GL_STATIC_DRAW);
		
		glBindVertexArray(0); //--- VAO를 바인드하기
	}

	//--- GL 오류시 출력하도록 하는 디버깅코드
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			// 오류 처리 또는 오류 메시지 출력
			std::cerr << "OpenGL 오류 발생: " << error << std::endl;
		}
	}
}

//---면을 채워서 출력
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

//---선으로 출력
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

//---정점 갯수에 따라 최대치로 출력
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

//---mesh에 정보를 세팅
void Mesh::setMesh(const int& mesh, const float& radius) {
	//if (exist()) {
	//	clear();
	//}

	switch (mesh) {
	case MESH_AXIS:
		axis();
		name = "axis";
		break;
	case MESH_TETRAHEDRON:
		tetrahedron();
		name = "tetrahedron";
		break;
	case MESH_SPIRAL:
		spiral();
		name = "spiral";
		break;
	case MESH_CUBE:
		ReadObj("resource\\cube.obj");
		name = "cube";
		break;
	case MESH_PYRAMID:
		ReadObj("resource\\pyramid.obj");
		name = "pyramid";
		break;
	case MESH_SPHERE:
		ReadObj("resource\\sphere.obj");
		name = "SPHERE";
		break;
	case MESH_CIRCLE:
		circle(radius);
		name = "circle";
		break;
	case MESH_TRIANGLE:	case MESH_SQUARE:	case MESH_PENTAGON:	case MESH_HEXAGON:	case MESH_HEPTAGON:	case MESH_OCTAGON:
		//std::cout << "생성되는 도형 : " << mesh - MESH_TRIANGLE + 3 << '\n';
		polygon(mesh - MESH_TRIANGLE + 3);
		break;
	}
}

//---세팅된 정보를 삭제
void Mesh::clear() {
	if (exist()) {
		//polygon은 genbuffer를 존재시엔 하지 않음.(덮어쓰기 됨)
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

//.obj 파일 읽기 샘플 (버전에 따라 수정 필요)
void Mesh::ReadObj(const char* filename) {
	//std::cout << "filename : '" << filename << "'" << '\n';
	std::ifstream file( filename );

	if (!file) {
		std::cout << filename << "##파일 오픈 실패 ##" << '\n';
		return;
	}

	//--- 1. 기존 데이터 삭제
	vertex_pos.clear();
	vertex_normal.clear();
	vertex_color.clear();
	uvs.clear();

	vertex_index.clear();
	uv_index.clear();
	normal_index.clear();

	//--- 2. 할당된 메모리에 각 버텍스, index, uv 정보 입력

	//if (!file.is_open()) {
	//	std::cerr << "Failed to open the file." << std::endl;
	//	return;
	//}

	//std::vector<std::vector<int>> faces;  // 면 데이터
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
				vertex_color.push_back(glm::vec3{ 0.5f } * (vertex + glm::vec3{ 1.0f }));
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
					std::cout << "vertex_index.size() : " << vertex_index.size() << '\n';
				// faceIndices.push_back(vertexIndex);
				// 여기서 텍스처 좌표와 노멀이 필요하다면 처리 추가				
			}

			//faces.push_back(faceIndices);
		}
		else {
			// 다른 라인일 경우, 그냥 넘어감
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	file.close();

	if (vertex_normal.size() == 0) {
		if (debug) {
			std::cout << "obj파일에 정점 노멀 존재하지 않음 ..." << '\n';
			std::cout << "벡터 생성중 ..." << '\n';
		}

		for (int i = 0; i < vertex_index.size()/3; ++i) {
			if(debug)
				std::cout << "i = "<< i << ", normal : ";

			glm::vec3 vector[2];
			vector[0] = vertex_pos[vertex_index[3 * i + 1]] - vertex_pos [vertex_index[3 * i]];
			vector[1] = vertex_pos[vertex_index[3 * i + 2]] - vertex_pos [vertex_index[3 * i]];
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

	

	//--- 3. 저장된 정보 debug 출력
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
		for (const glm::vec3& v : vertex_color) {
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
	////--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
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
	////--- 2. 메모리 할당
	//glm::vec3* vertex = new glm::vec3[vertexnum]();
	//glm::vec3* color = new glm::vec3[vertexnum]();
	//glm::uvec3* face = new glm::uvec3[facenum]();
	//glm::vec3* uvdata = new glm::vec3[facenum]();
	//glm::vec3* normal = new glm::vec3[facenum]();
	//glm::vec2* uv = new glm::vec2[uvnum]();
	////std::cout << "2. 메모리 할당 완료" << '\n';
	//char bind[128];
	////--- 3. 할당된 메모리에 각 버텍스, 페이스, uv 정보 입력
	//while (!feof(path)) {
	//	fscanf(path, "%s", bind);
	//	if (bind[0] == 'v' && bind[1] == '\0') {
	//		fscanf(path, "%f %f %f\n",
	//			&vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
	//		{
	//			//색상(랜덤)
	//			{/*
	//				std::random_device rd;
	//				std::mt19937 gen(rd());
	//				std::uniform_real_distribution<float> urd(0.0, 1.0);
	//				color[vertIndex].r = urd(gen);
	//				color[vertIndex].g = urd(gen);
	//				color[vertIndex].b = urd(gen);
	//			*/}
	//			//색상(정점 위치값 대입)
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

	//--- 필요한 경우 읽어온 값을 전역 변수 등에 저장
	push_GPU();
}

//--- 정사면체 mesh값으로 초기화 시켜줌.
void Mesh::tetrahedron() {
	glm::vec3* vertex_pos = new glm::vec3[4];

	vertex_pos[0].x = 0.0f;
	vertex_pos[0].y = glm::sin(glm::radians(60.0)) * 1.0f;
	vertex_pos[0].z = 0.0f;

	vertex_pos[1].x = cos(glm::radians(90.0)) * 1.0f;
	vertex_pos[1].y = glm::sin(glm::radians(60.0)) * -1.0f * 1 / 2;
	vertex_pos[1].z = sin(glm::radians(90.0)) * 1.0f;

	vertex_pos[2].x = cos(glm::radians(210.0)) * 1.0f;
	vertex_pos[2].y = glm::sin(glm::radians(60.0)) * -1.0f * 1 / 2;
	vertex_pos[2].z = sin(glm::radians(210.0)) * 1.0f;

	vertex_pos[3].x = cos(glm::radians(330.0)) * 1.0f;
	vertex_pos[3].y = glm::sin(glm::radians(60.0)) * -1.0f * 1 / 2;
	vertex_pos[3].z = sin(glm::radians(330.0)) * 1.0f;

	glm::vec3* vertex_color = new glm::vec3[4];



	//색상(랜덤)

	//색상(정점 위치값 대입)
	{
		for (int i = 0; i < 4; i++) {
			vertex_color[i].r = vertex_pos[i].x;
			vertex_color[i].g = vertex_pos[i].y;
			vertex_color[i].b = vertex_pos[i].z;
		}
	}

	glm::uvec3* face = new glm::uvec3[4];

	face[0].x = 0;
	face[0].y = 1;
	face[0].z = 2;

	face[1].x = 0;
	face[1].y = 2;
	face[1].z = 3;

	face[2].x = 0;
	face[2].y = 3;
	face[2].z = 1;

	face[3].x = 1;
	face[3].y = 2;
	face[3].z = 3;

	genGPUbuffers();
	glBindVertexArray(vao); //--- VAO를 바인드하기


	/*	std::cout << "sizeof(glm::vec3) : " << sizeof(glm::vec3) << '\n';
		std::cout << "sizeof(vertex) : " << sizeof(vertex) << '\n';
		std::cout << "vertexnum * sizeof(glm::vec3) :" << vertexnum * sizeof(glm::vec3) << '\n';*/

		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float		
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), vertex_pos, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), vertex_color, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(glm::uvec3), face, GL_STATIC_DRAW);

	glBindVertexArray(0); //--- VAO를 바인드하기

	origin_translation = { 0.0f, 0.0f, 0.0f };
	origin_scale = { 1.0f, 1.0f, 1.0f };

	delete[] vertex_pos;
	delete[] vertex_color;
}

//--- x, y, z축 mesh값으로 초기화 시켜줌.
void Mesh::axis() {
	glm::vec3 vertex_pos[6];
	glm::vec3 vertex_color[6];
	GLuint vertex_index[6];

	const float LEN{ 1.0f };
	for (int i = 0; i < 6; i++) {
		vertex_pos[i].x = i / 2 == 0 ? 1.0f : 0.0f;
		vertex_pos[i].x *= i % 2 == 0 ? -1 : 1;
		vertex_pos[i].y = i / 2 == 1 ? 1.0f : 0.0f;
		vertex_pos[i].y *= i % 2 == 0 ? -1 : 1;
		vertex_pos[i].z = i / 2 == 2 ? 1.0f : 0.0f;
		vertex_pos[i].z *= i % 2 == 0 ? -1 : 1;

		vertex_color[i].x = i / 2 == 0 ? 1.0f : 0.0f;
		vertex_color[i].y = i / 2 == 1 ? 1.0f : 0.0f;
		vertex_color[i].z = i / 2 == 2 ? 1.0f : 0.0f;

		vertex_index[i] = i;
	}


	{
		genGPUbuffers();
		glBindVertexArray(vao); //--- VAO를 바인드하기


		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
		glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
		//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
		//--- triShape 배열의 사이즈: 9 * float		
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), vertex_pos, GL_STATIC_DRAW);
		//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 0번을 사용가능하게 함
		glEnableVertexAttribArray(0);

		//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- 변수 colors에서 버텍스 색상을 복사한다.
		//--- colors 배열의 사이즈: 9 *float
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), vertex_color, GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), vertex_index, GL_STATIC_DRAW);*/

		glBindVertexArray(0); //--- VAO를 바인드하기
	}

	name = "axis";
}

void Mesh::spiral() {	
	std::vector<float> vertex_pos;
	std::vector<float> vertex_color;

	float LEN{ 0.0f };
	float degree{ 0.0f };
	int count{ 0 };
	while (LEN < 1.0f) {
		vertex_pos.push_back(cos(glm::radians(degree)) * LEN);	//x
		vertex_pos.push_back(0.0f);	//y
		vertex_pos.push_back(sin(glm::radians(degree)) * LEN);	//z


		vertex_color.push_back(0.4f);
		vertex_color.push_back(0.64f);
		vertex_color.push_back(1.0f);
		/*color.push_back(random_number(0.0f, 1.0f));
		color.push_back(random_number(0.0f, 1.0f));
		color.push_back(random_number(0.0f, 1.0f));*/
		degree += 1.0f;
		if (degree >= 360.0f) {
			count++;
			degree -= 360.0f;
		}
		LEN += 0.0003f;
	}


	{
		genGPUbuffers();
		glBindVertexArray(vao); //--- VAO를 바인드하기


		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
		//--- triShape 배열의 사이즈: 9 * float		
		glBufferData(GL_ARRAY_BUFFER, vertex_pos.size() * sizeof(float), vertex_pos.data(), GL_STATIC_DRAW);
		//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 0번을 사용가능하게 함
		glEnableVertexAttribArray(0);

		//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- 변수 colors에서 버텍스 색상을 복사한다.
		//--- colors 배열의 사이즈: 9 *float
		glBufferData(GL_ARRAY_BUFFER, vertex_color.size() * sizeof(float), vertex_color.data(), GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

		glBindVertexArray(0); //--- VAO를 바인드하기
	}

	name = "spiral";
}

void Mesh::circle(const float& radius) {
	std::vector<float> vertex_pos;
	std::vector<float> vertex_color;

	float LEN{ 1.0f };
	float degree{ 0.0f };
	int count{ 0 };
	while (degree < 360.0f) {
		vertex_pos.push_back(cos(glm::radians(degree)) * LEN);	//x
		vertex_pos.push_back(sin(glm::radians(degree)) * LEN);	//y
		vertex_pos.push_back(0.0f);	//z


		vertex_color.push_back(0.4f);
		vertex_color.push_back(0.64f);
		vertex_color.push_back(1.0f);
		/*color.push_back(random_number(0.0f, 1.0f));
		color.push_back(random_number(0.0f, 1.0f));
		color.push_back(random_number(0.0f, 1.0f));*/
		degree += 1.0f;
		
	}


	{
		genGPUbuffers();
		glBindVertexArray(vao); //--- VAO를 바인드하기


		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
		//--- triShape 배열의 사이즈: 9 * float		
		glBufferData(GL_ARRAY_BUFFER, vertex_pos.size() * sizeof(float), vertex_pos.data(), GL_STATIC_DRAW);
		//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 0번을 사용가능하게 함
		glEnableVertexAttribArray(0);

		//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- 변수 colors에서 버텍스 색상을 복사한다.
		//--- colors 배열의 사이즈: 9 *float
		glBufferData(GL_ARRAY_BUFFER, vertex_color.size() * sizeof(float), vertex_color.data(), GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

		glBindVertexArray(0); //--- VAO를 바인드하기
	}

	name = "circle";
}

void Mesh::polygon(const int& polygon) {
	//name = polygon + "각형";
	//std::cout << "생성할 polygon :" << polygon << "\n";
	vertex_pos.clear();
	vertex_color.clear();
	vertex_index.clear();
	std::vector<float> vertex_pos;
	std::vector<float> vertex_color;
	std::vector<unsigned int> vertex_index;

	float LEN{ 1.0f };
	float degree{ 360.0f / polygon };
	unsigned int count{ 0 };

	while (count < polygon) {
		//vertex.push_back({cos(glm::radians(degree * count)) * LEN, sin(glm::radians(degree * count)) * LEN, 0.0f});	//x
		vertex_pos.push_back(cos(glm::radians(degree * count)) * LEN);	//y
		vertex_pos.push_back(sin(glm::radians(degree * count)) * LEN);	//y
		vertex_pos.push_back(0.0f);	//z
		this-> vertex_pos.push_back({ cos(glm::radians(degree * count)) * LEN, sin(glm::radians(degree * count)) * LEN, 0.0f });	//x
		

		vertex_color.push_back(rainbow[count % 8].x);
		vertex_color.push_back(rainbow[count % 8].y);
		vertex_color.push_back(rainbow[count % 8].z);
		this->vertex_color.push_back({ rainbow[count % 8].x, rainbow[count % 8].y, rainbow[count % 8].z });	//x
		/*color.push_back(random_number(0.0f, 1.0f));
		color.push_back(random_number(0.0f, 1.0f));
		color.push_back(random_number(0.0f, 1.0f));*/
		if (count >= 2) {
			vertex_index.push_back(0);
			vertex_index.push_back(count - 1);
			vertex_index.push_back(count);
			this->vertex_index.push_back(0);
			this->vertex_index.push_back(count - 1);
			this->vertex_index.push_back(count);
		}
		count++;
	}
	{
		genGPUbuffers();
		glBindVertexArray(vao); //--- VAO를 바인드하기


		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
		//--- triShape 배열의 사이즈: 9 * float		

		glBufferData(GL_ARRAY_BUFFER, vertex_pos.size() * sizeof(float), vertex_pos.data(), GL_STATIC_DRAW);

		//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 0번을 사용가능하게 함
		glEnableVertexAttribArray(0);

		//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- 변수 colors에서 버텍스 색상을 복사한다.
		//--- colors 배열의 사이즈: 9 *float
		glBufferData(GL_ARRAY_BUFFER, vertex_color.size() * sizeof(float), vertex_color.data(), GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

	/*	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_index.size() * sizeof(unsigned int), vertex_index.data(), GL_STATIC_DRAW);*/


		glBindVertexArray(0); //--- VAO를 바인드하기
	}

	switch (polygon) {
	case 3:
		name = "삼각형";
		break;
	case 4:
		name = "사각형";
		break;
	case 5:
		name = "오각형";
		break;
	case 6:
		name = "육각형";
		break;
	case 7:
		name = "칠각형";
		break;
	case 8:
		name = "팔각형";
		break;
	}
}

bool Mesh::exist() const {
	if (!name.compare("None")) {
		//std::cout << "현재 mesh에 저장된 값 없음." << '\n';
		return false;
	}
	//std::cout << "현재 mesh에 저장된 값 :" << name << '\n';
	return true;
}

std::string Mesh::getName() const {
	return name;
}

void Mesh::Object_Space_Transform(glm::mat4& transformMatrix) const {
	transformMatrix = glm::scale(transformMatrix, origin_scale);		//--- 확대축소 변환(Scaling)
	transformMatrix = glm::translate(transformMatrix, origin_translation); //--- 이동(Translation)	
}


void Mesh::line_initBuffers(const glm::vec3& start, const glm::vec3& end) {

	vertex_pos.clear();

	std::vector<float> vertex_color;
	std::vector<unsigned int> vertex_index;

	{	//값을 집어넣음.
		//--정점
		vertex_pos.push_back({ start.x, start.y, 0.0f });
		//vertex.push_back(start.x);
		//vertex.push_back(start.y);
		//vertex.push_back(0.0f);

		vertex_pos.push_back({ end.x, end.y, 0.0f });
		//vertex.push_back(end.x);
		//vertex.push_back(end.y);
		//vertex.push_back(0.0f);
		//--색상
		vertex_color.push_back(0.0f);
		vertex_color.push_back(0.0f);
		vertex_color.push_back(0.0f);

		vertex_color.push_back(0.0f);
		vertex_color.push_back(0.0f);
		vertex_color.push_back(0.0f);

		vertex_index.push_back(0);
		vertex_index.push_back(1);
	}

	{
		genGPUbuffers();
		glBindVertexArray(vao); //--- VAO를 바인드하기

		//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
		//--- triShape 배열의 사이즈: 9 * float		
		glBufferData(GL_ARRAY_BUFFER, vertex_pos.size() * sizeof(glm::vec3), vertex_pos.data(), GL_STATIC_DRAW);
		//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 0번을 사용가능하게 함
		glEnableVertexAttribArray(0);

		//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		//--- 변수 colors에서 버텍스 색상을 복사한다.
		//--- colors 배열의 사이즈: 9 *float
		glBufferData(GL_ARRAY_BUFFER, vertex_color.size() * sizeof(float), vertex_color.data(), GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

	/*	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_index.size() * sizeof(unsigned int), vertex_index.data(), GL_STATIC_DRAW);*/


		glBindVertexArray(0); //--- VAO를 바인드하기
	}
	name = "Line";
}
