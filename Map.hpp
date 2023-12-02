#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include "Default.hpp"
#include "Object.hpp"
#include "Pizza.hpp"
#include "World.hpp"
#include <fstream>

class Map {
	// ���Ͽ��� �о�� �����Ͱ� ����Ǵ� ������
	std::vector<float> floor_rad;
	std::vector<std::vector<int>> floor_member;

	// ������ ������ ��ü���� ����Ű�� ������
	std::vector<std::vector< std::shared_ptr<Object> >> map;	// �������� ���� (�ٴ�)
	std::vector<std::shared_ptr<Object>> item;					// �������� ���� (������)
	std::shared_ptr<Object> pillar;	// ��� ���
public:
	static bool debug;	// ����� ��� ����

	// ���� �����ϴ� �Լ�
	void outputMap(const std::string& filename);

	// �� ���� �о���� �Լ�.
	void loadMap(const std::string& filename);
	// �� �ڵ� ����� ���� �Լ�
	void exampleMap();
	void Map1();
	void Map2();

	// �ҷ��� �����ͷ� ���� ������Ʈ �����ϴ� �Լ�
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
