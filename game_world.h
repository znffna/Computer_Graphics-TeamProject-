#pragma once


#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include "Default.hpp"
#include "Object.hpp"

struct World {
    std::vector<std::vector<std::shared_ptr<Object>>> objects;
    std::map<std::string, std::pair< std::vector<std::shared_ptr<Object>>, std::vector<std::shared_ptr<Object>>> > collision_pairs;

    // 객체 추가
    void add_objects(std::vector<Object>& object);

    // 갱신
    void update();

    // 출력
    void render();

    void remove_collision_object(Object& o);
    void remove_object(Object& o);
    void clear();
    bool collide(Object& a, Object& b);
    void add_collision_pair(std::string& group, Object& a, Object& b);
    void handle_collisions();
    void exit();
};

extern World world;


#endif // !GAME_WORLD_HPP
