#pragma once


#ifndef GAME_WORLD_HPP
#define GAME_WORLD_HPP

#include "Default.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "Pizza.hpp"
#include "Ball.hpp"
#include "Cube.hpp"
#include "Pillar.hpp"

#include "Light.hpp"

#include <unordered_set>

struct World {
    static bool debug;  // ����׿�

    std::vector<std::shared_ptr<Object>> objects;
    std::map<std::string, std::vector <std::unordered_set <std::shared_ptr<Object>> > > collision_pairs;

    // ��ü �߰�
    void add_object(std::shared_ptr<Object>& object);
    void add_object(std::shared_ptr<Pizza>& object);


    void add_objects(std::vector<std::shared_ptr<Object>>& object);
    void remove_object(std::shared_ptr<Object>& o);

    // ����
    void update();
    void handle_events(unsigned char, const std::string&);
    void handle_collisions();
    // ���
    void render(Shader& shader);

    // World �ʱ�ȭ(����°� �ƴ϶� �����ϴ� Object���� ���� �ʱ�ȭ)
    void reset();

    // �浹����
    bool collide(std::shared_ptr<Object>& first, std::shared_ptr<Object>& second);
    void add_collision_pair(const std::string& group, std::shared_ptr<Object>& a, std::shared_ptr<Object>& b);
    void remove_collision_object(std::shared_ptr<Object>& object);

    bool Check_collision(std::shared_ptr<Ball>& ball, std::shared_ptr<Pizza>& pizza);
    bool Check_collision(std::shared_ptr<Ball>& ball, std::shared_ptr<Cube>& cube);

    // World ���� ( �̰� ��¥ clear )
    void clear();
};

extern World world;

#endif // !GAME_WORLD_HPP
