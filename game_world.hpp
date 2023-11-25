#pragma once


#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include "Default.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include <unordered_set>

struct World {
    static bool debug;  // 디버그용

    std::vector<std::shared_ptr<Object>> objects;
    std::map<std::string, std::vector <std::unordered_set <std::shared_ptr<Object>> > > collision_pairs;

    // 객체 추가
    void add_object(std::shared_ptr<Object>& object);
    void add_objects(std::vector<std::shared_ptr<Object>>& object);
    void remove_object(std::shared_ptr<Object>& o);

    // 갱신
    void update();
    void handle_collisions();
    // 출력
    void render(Shader& shader);

    // World 초기화
    void clear();

    // 충돌판정
    bool collide(std::shared_ptr<Object>& a, std::shared_ptr<Object>& b);
    void add_collision_pair(std::string& group, std::shared_ptr<Object>& a, std::shared_ptr<Object>& b);
    void remove_collision_object(std::shared_ptr<Object>& object);

    // World 제거
    void exit();
};

extern World world;

#endif // !GAME_WORLD_HPP
