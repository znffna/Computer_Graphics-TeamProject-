#include "Map.hpp"
#include "Default.hpp"
#include "Object.hpp"
#include "Game_world.hpp"

bool Map::debug{ false };
Map map;


// 맵을 추출하는 함수
void Map::outputMap(const std::string& filename)
{
	std::ofstream out{ filename };
	for (int i = 0; i < floor_rad.size(); ++i) {
		out << "f " << floor_rad[i] << ' ';
		for (int p : floor_member[i]) {
			out << p << ' ';
		}
		out << '\n';
	}
}

// 맵 파일 읽어오는 함수.
void Map::loadMap(const std::string & filename)
{
	std::ifstream in{ filename };
	if (not in) {
		std::cout << filename << "을 열지 못함." << '\n';
		return;
	}
	// 기존 데이터 초기화
	floor_rad.clear();
	floor_member.clear();

	// 데이터 읽어 오기
	while (!in.eof()) {
		char input;	// 현재 읽을 줄이 어떤 데이터를 저장한지 파악하기 위함.
		in >> input;

		if (input == 'f') {	//floor 줄을 읽어야 할 경우
			// 각도를 읽어 저장
			float rad;
			in >> rad;
			floor_rad.push_back(rad);

			// 타입을 읽어 저장
			std::vector<int> typelist;
			for (int i = 0; i < 12; ++i) {
				int type;
				in >> type;
				typelist.push_back(std::move(type));
			}
			floor_member.push_back(std::move(typelist));
		}
	}

	if (debug) {
		std::cout << filename << "의 데이터 읽기 완료." << '\n';
		std::cout << "읽어온 층수 : " << floor_rad.size() << '\n';
	}
}
// 맵 코드 만드는 예제 함수
void Map::exampleMap()
{
	// 기존 데이터 초기화
	floor_rad.clear();
	floor_member.clear();
	// 각층 구조 생성
	for (int i = 0; i < 30; ++i) {
		// 해당 층의 회전 정도
		float rad = random_number(0.0f, 30.0f);	// 이건 0.0f ~ 360.0f 까지 조절 해도 됨.

		floor_rad.push_back(rad);

		// 해당 층의 각 조각 모음
		std::vector<int> typelist;
		for (int j = 0; j < 12; ++j) {
			// 도형 종류를 여기서 결정 (임시코드는 랜덤값으로)
			typelist.push_back(random_number(0, 3));
		}

		floor_member.push_back(std::move(typelist));
	}
}

// 불러온 데이터로 실제 오브젝트 생성하는 함수
void Map::makeMap()
{
	if (floor_rad.size() == 0) {
		std::cout << "map.makeMap() : 현재 불러온 데이터가 없음" << '\n';
		return;
	}
	float start_height = 5.0f * floor_rad.size() / 2.0f;
	// 각층 구조 생성
	for (int i = 0; i < floor_rad.size(); ++i) {
		std::vector< std::shared_ptr<Object> > floor;
		float rad = floor_rad[i];
		for (int j = 0; j < floor_member[i].size(); ++j) {
			std::shared_ptr<Pizza> tmp = std::make_shared<Pizza>(30.0f * j + rad, floor_member[i][j]);
			tmp.get()->setTranslation({ 0.0f, start_height - 5.0f * i, 0.0f });
			tmp.get()->setScale({ 5.0f, 0.2f, 5.0f });
			tmp.get()->setColor(rainbow[floor_member[i][j] % 8]);
			floor.push_back(tmp);

			world.add_object(tmp);
			auto o_tmp = std::static_pointer_cast<Object>(tmp);
			world.add_collision_pair("Ball:Pizza", NULLPTR, o_tmp);
		}
		map.push_back(std::move(floor));
	}
	// 중앙 기둥 생성		
	{
		std::shared_ptr<Object> tmp = std::make_shared<Object>(PILLAR);
		tmp.get()->setTranslation({ 0.0f, 0.0f, 0.0f });
		tmp.get()->setScale({ 1.5f, start_height, 1.5f });
		tmp.get()->setColor({ 1.0f, 1.0f, 1.0f });

		world.add_object(tmp);
		//world.push_back(temp);
	}
}