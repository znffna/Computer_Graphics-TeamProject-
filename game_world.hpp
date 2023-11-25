#pragma once


#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include "Default.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include <unordered_set>

struct World {
    static bool debug;  // ����׿�

    std::vector<std::shared_ptr<Object>> objects;
    std::map<std::string, std::vector <std::unordered_set <std::shared_ptr<Object>> > > collision_pairs;

    // ��ü �߰�
    void add_object(std::shared_ptr<Object>& object);
    void add_objects(std::vector<std::shared_ptr<Object>>& object);
    void remove_object(std::shared_ptr<Object>& o);

    // ����
    void update();
    void handle_collisions();
    // ���
    void render(Shader& shader);

    // World �ʱ�ȭ
    void clear();

    // �浹����
    bool collide(std::shared_ptr<Object>& a, std::shared_ptr<Object>& b);
    void add_collision_pair(std::string& group, std::shared_ptr<Object>& a, std::shared_ptr<Object>& b);
    void remove_collision_object(std::shared_ptr<Object>& object);

    // World ����
    void exit();
};

extern World world;

#endif // !GAME_WORLD_HPP
