#pragma once

#ifndef MAP_HPP
#define MAP_HPP

class Map {
	std::vector<std::vector< std::shared_ptr<Object> >> map;

public:
	Map(const std::string& filename);
	// �� ���� �о���� �Լ�.
	void LoadMap(const std::string& filename) {
		//TODO �ڵ� ¥����
	}

	void render() {
		//TODO �ڵ� ¥����

	}

	void update() {
		//TODO �ڵ� ¥����

	}

	// void SaveMap(); // �� ������ ����� �Լ�.

};


#endif // !MAP_HPP
