#include "Default.hpp"

#ifndef MESH_HPP
#define MESH_HPP

struct Mesh {
private:
	std::string name;
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
	Mesh(const std::string& filename);
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
	void setMesh(const std::string&);
	void push_GPU();
	bool exist() const;
};

void Read_ObjectFile();

// ����� obj ������ ������
extern std::shared_ptr<Mesh> CUBE;
extern std::shared_ptr<Mesh> PYRAMID;
extern std::shared_ptr<Mesh> SPHERE;
extern std::shared_ptr<Mesh> PIZZA;
extern std::shared_ptr<Mesh> PILLAR;


#endif //


