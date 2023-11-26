#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include "Default.hpp"
#include "Pizza.hpp"
#include "game_world.hpp"
#include <fstream>

class Map {
	// ���Ͽ��� �о�� �����Ͱ� ����Ǵ� ������
	std::vector<float> floor_rad;
	std::vector<std::vector<int>> floor_member;

	// ������ ������ ��ü���� ����Ű�� ������
	std::vector<std::vector< std::shared_ptr<Object> >> map;	// �������� ����
	std::shared_ptr<Object> pillar;	// ��� ���
public:
	static bool debug;	// ����� ��� ����

	// ���� �����ϴ� �Լ�
	void outputMap(const std::string& filename);

	// �� ���� �о���� �Լ�.
	void loadMap(const std::string& filename);
	// �� �ڵ� ����� ���� �Լ�
	void exampleMap();

	// �ҷ��� �����ͷ� ���� ������Ʈ �����ϴ� �Լ�
	void makeMap();
};

extern Map map;


#endif // !MAP_HPP
