#pragma once

#ifndef MAP_HPP
#define MAP_HPP

class Map {
	std::vector<std::vector< std::shared_ptr<Object> >> map;

public:
	Map(const std::string& filename);
	// 맵 파일 읽어오는 함수.
	void LoadMap(const std::string& filename) {
		//TODO 코드 짜세요
	}

	void render() {
		//TODO 코드 짜세요

	}

	void update() {
		//TODO 코드 짜세요

	}

	// void SaveMap(); // 맵 파일을 만드는 함수.

};


#endif // !MAP_HPP
