#include "Default.hpp"

#ifndef MESH_HPP
#define MESH_HPP

struct Mesh {
private:
	std::string name;
	//obj 파일 불러오는 함수
	void ReadObj(const char*);

	bool existVao{ false };
	bool existVbo{ false };
	bool existEbo{ false };

public:
	//---멤버 변수
	GLuint vao;
	GLuint vbo[3];

	GLuint ebo;

	std::vector<glm::vec3> vertex_pos;		// 기본 정점 위치 데이터
	std::vector<glm::vec3> vertex_normal;	// 기본 정점 노멀 데이터
	std::vector<glm::vec3> vertex_color;	// 기본 정점 색상 데이터
	std::vector<glm::vec2> uvs;				// 기본 텍스처(UV) 좌표 데이터

	std::vector<unsigned int> vertex_index;	// 정점 index
	std::vector<unsigned int> uv_index;		// 텍스쳐 index
	std::vector<unsigned int> normal_index;	// 노멀 index

	glm::vec3 origin_translation;	// 도형의 중심으로 이동시키는 벡터
	glm::vec3 origin_scale;			// 도형의 크기를 정규화 하는 과정

	static bool debug;		// 디버그 출력 flag : true - 출력
	static bool draw_option;// 출력 타입 : true - FILL, false - LINE

	//---멤버 함수
	//생성자
	Mesh();
	Mesh(const std::string& filename);
	//복사 생성자
	Mesh(const Mesh& other);
	//복사 할당 연산자
	Mesh& operator=(const Mesh& other);
	//이동 생성자
	Mesh(Mesh&& other) noexcept;
	//이동 할당 생성자
	Mesh& operator=(Mesh&& other) noexcept;
	//소멸자
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

// 사용할 obj 저장할 포인터
extern std::shared_ptr<Mesh> CUBE;
extern std::shared_ptr<Mesh> PYRAMID;
extern std::shared_ptr<Mesh> SPHERE;
extern std::shared_ptr<Mesh> PIZZA;
extern std::shared_ptr<Mesh> PILLAR;


#endif //


