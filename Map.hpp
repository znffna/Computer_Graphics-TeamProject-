#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include "Default.hpp"
#include "Object.hpp"
#include "Pizza.hpp"
#include "World.hpp"
#include <fstream>

class Map {
	// 파일에어 읽어온 데이터가 저장되는 변수들
	std::vector<float> floor_rad;
	std::vector<std::vector<int>> floor_member;

	// 실제로 생성된 객체들을 가르키는 변수들
	std::vector<std::vector< std::shared_ptr<Object> >> map;	// 스테이지 구성 (바닥)
	std::vector<std::shared_ptr<Object>> item;					// 스테이지 구성 (아이템)
	std::shared_ptr<Object> pillar;	// 가운데 기둥
public:
	static bool debug;	// 디버그 출력 여부

	// 맵을 추출하는 함수
	void outputMap(const std::string& filename);

	// 맵 파일 읽어오는 함수.
	void loadMap(const std::string& filename);
	// 맵 코드 만드는 예제 함수
	void exampleMap();
	void Map1();
	void Map2();

	// 불러온 데이터로 실제 오브젝트 생성하는 함수
	void makeMap();

	float getHeight() {
		if (map.empty()) {
			return 0.0f;
		}
		return map[0][0].get()->getTranslation().y;
	}
};

extern Map map;


#endif // !MAP_HPP
