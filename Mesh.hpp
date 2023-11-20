#include "Default.hpp"

#ifndef MESH_HPP
#define MESH_HPP

//SetMesh�� ���Ǵ� ��ȣ
#define MESH_AXIS 0x00
#define MESH_TETRAHEDRON 0x01
#define MESH_SPIRAL 0x02
#define MESH_CUBE 0x03
#define MESH_PYRAMID 0x04
#define MESH_SPHERE 0x05

#define MESH_CIRCLE 0x10
#define MESH_TRIANGLE 0x11
#define MESH_SQUARE 0x12
#define MESH_PENTAGON 0x13
#define MESH_HEXAGON 0x14
#define MESH_HEPTAGON 0x15
#define MESH_OCTAGON 0x16

//---- Mesh ���� ----
// 1. ���� Mesh
// 2. Mesh�� �ҷ��� =>  setMesh( ^ ��ȣ ^); 
// ex) setMesh(MESH_AXIS) - ���� ����ϴ� mesh �ҷ���.
// 3. 

class Mesh {
private:
	std::string name;


	void spiral();
	void axis();
	void tetrahedron();
	void circle(const float&);
	void polygon(const int&);

	//obj ���� �ҷ����� �Լ�
	void ReadObj(const char*);

	bool existVao{ false };
	bool existVbo{ false };
	bool existEbo{ false };


public:
	//---��� ����
	GLuint vao;
	GLuint vbo[3];

	GLuint ebo;

	std::vector<glm::vec3> vertex_pos;		// �⺻ ���� ��ġ ������
	std::vector<glm::vec3> vertex_normal;	// �⺻ ���� ��� ������
	std::vector<glm::vec3> vertex_color;	// �⺻ ���� ���� ������
	std::vector<glm::vec2> uvs;				// �⺻ �ؽ�ó(UV) ��ǥ ������

	std::vector<unsigned int> vertex_index;	// ���� index
	std::vector<unsigned int> uv_index;		// �ؽ��� index
	std::vector<unsigned int> normal_index;	// ��� index

	glm::vec3 origin_translation;	// ������ �߽����� �̵���Ű�� ����
	glm::vec3 origin_scale;			// ������ ũ�⸦ ����ȭ �ϴ� ����

	static bool debug;		// ����� ��� flag : true - ���
	static bool draw_option;// ��� Ÿ�� : true - FILL, false - LINE

	//---��� �Լ�
	//������
	Mesh();
	//���� ������
	Mesh(const Mesh& other);
	//���� �Ҵ� ������
	Mesh& operator=(const Mesh& other);
	//�̵� ������
	Mesh(Mesh&& other) noexcept;
	//�̵� �Ҵ� ������
	Mesh& operator=(Mesh&& other) noexcept;
	//�Ҹ���
	~Mesh();

	void genVao();
	void genVbo();
	void genEbo();

	void delVao();
	void delVbo();
	void delEbo();

	void genGPUbuffers();
	void delGPUbuffers();

	std::string get_name() {
		return name;
	}

	void set_name(const char* string) {
		name = string;
	}

	//method
	void Fill_Draw() const;
	void LINE_Draw() const;
	void AUTO_Draw() const;

	void clear();
	void setMesh(const int&, const float& radius = 10.0f);
	void push_GPU();
	bool exist() const;

	std::string getName() const;
	void Object_Space_Transform(glm::mat4& ) const;

	//���콺�� Ŭ������ ������ ���������� ����
	void line_initBuffers(const glm::vec3& start, const glm::vec3& end);

};


#endif //


