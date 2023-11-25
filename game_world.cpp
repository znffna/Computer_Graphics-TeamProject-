#include "game_world.hpp"

bool World::debug = false;

World world;

/*
    std::vector<std::shared_ptr<Object>> objects;
    std::map<std::string, std::pair< std::vector<std::shared_ptr<Object>>, std::vector<std::shared_ptr<Object>>> > collision_pairs;

*/
void World::add_object(std::shared_ptr<Object>& object)
{
    objects.push_back(object);
}

void World::add_objects(std::vector<std::shared_ptr<Object>>& object_vector)
{
    for (std::shared_ptr<Object>& object : object_vector) {
        objects.push_back(object);
    }
}
    
void World::update()
{
    // ��ü���� �ִϸ���Ʈ ��Ŵ.
    for (std::shared_ptr<Object>& object : objects) {
        object.get()->update();
    }
}

// ���
void World::render(Shader& shader)
{
    for (std::shared_ptr<Object>& object : objects) {
        shader.setColor({ object.get()->getColor() });
        shader.draw_object(*object.get());
        object.get()->render();
    }
}

void World::remove_collision_object(std::shared_ptr<Object>& object)
{
    for (auto iter = collision_pairs.begin(); iter != collision_pairs.end(); ++iter) {
        // map���� ���� ������ ����
        auto key = iter->first;
        auto list = iter->second;

        auto& first = list.at(0);
        auto& second = list.at(1);
        // ã�� ��� ����
        {
            auto first_search = std::find(first.begin(), first.end(), object);
            if (first_search != first.end()) {
                first.erase(first_search);
            }
            auto second_search = std::find(second.begin(), second.end(), object);
            if (second_search != second.end()) {
                second.erase(second_search);
            }
        }
    }
}


void World::remove_object(std::shared_ptr<Object>& object)
{
    auto search = std::find(objects.begin(), objects.end(), object);
    if (search != objects.end()) {
        objects.erase(search);  // �ð��� ���忡�� �����.
        remove_collision_object(object);  // �浹 �׷쿡�� ���� �Ϸ�
        // del o;  // ��ü ��ü�� ������ �޸𸮿��� ����. - std::shared_ptr�� ����Ű�� �ְ� ������ �˾Ƽ� ���ŵ�.
        return;
    }           
    // �Ʒ� cout �� ����Ǹ� �ش� �Ű������� world�� �������� ����.
    std::cout << "���� ��ü�� ������ �� ����" << '\n';
    //std::cout << "Cannot delete non existing object";
}


void World::clear()
{
    objects.clear();
    collision_pairs.clear();
}

// �浹 üũ �˻� (AABB)
bool World::collide(std::shared_ptr<Object>& a, std::shared_ptr<Object>& b)
{
    std::vector<float> a_bb = a.get()->get_bb();
    std::vector<float> b_bb = b.get()->get_bb();
    //la, ba, ra, ta = a.get_bb()
    //lb, bb, rb, tb = b.get_bb()

    // x
    if (a_bb[0] > b_bb[2]) return false;
    if (a_bb[2] < b_bb[0]) return false;
    // y
    if (a_bb[3] < b_bb[1]) return false;
    if (a_bb[1] > b_bb[3]) return false;
    // z
    if (a_bb[5] < b_bb[4]) return false;
    if (a_bb[4] > b_bb[5]) return false;

    return true;
}

// �浹 ���� ����Ʈ�� �߰�
void World::add_collision_pair(std::string& group, std::shared_ptr<Object>& a, std::shared_ptr<Object>& b)
{
    auto it = collision_pairs.find(group);

    if (it == collision_pairs.end()) {
        std::unordered_set<std::shared_ptr<Object>> tmp1;
        std::unordered_set<std::shared_ptr<Object>> tmp2;
        std::vector<std::unordered_set<std::shared_ptr<Object>>> tmp3;
        tmp3.push_back(tmp1);
        tmp3.push_back(tmp2);

        collision_pairs.emplace(group, tmp3);
        if(debug)
        {
            std::cout << "New group : '" << group << "' added." << '\n';
            std::cout << "New group.size() : '" << collision_pairs.size() << '\n';
            std::cout << "New group.first.size() : '" << collision_pairs[group].at(0).size() << '\n';
            std::cout << "New group.second.size() : '" << collision_pairs[group].at(1).size() << '\n';
        }
    }

    if (a.get()) {
        collision_pairs[group][0].insert(a);
    }

    if (b.get()) {
        collision_pairs[group][1].insert(b);
    }

    //if (group not in collision_pairs)
    //{
    //    collision_pairs[group] = [[],[]]
    //}
    //if a : // a�� ���� ��, ��, a�� None �� �ƴϸ�..
    //    collision_pairs[group][0].append(a)
    //if b:
    //    collision_pairs[group][1].append(b)
}


void World::handle_collisions()
{
    // ��ϵ� ��� �浹 ��Ȳ�� ���ؼ� �浹 �˻� �� �浹 ó�� ����.

    for (auto& o : collision_pairs) {
        auto& key = o.first;
        auto& data = o.second;

        auto& first = data.at(0);
        auto& second = data.at(1);

        for (auto f_o : first) {
            for (auto s_o : second) {
                if (collide(f_o, s_o)) {
                    // �浹�� �ش� group�� ������ ����.
                    f_o.get()->handle_collision(key, s_o);
                    s_o.get()->handle_collision(key, f_o);
                }
            }
        }
    }
    

    //for group, pairs in collision_pairs.items() : # key 'boy:ball', value [[], []]
    //    for a in pairs[0]:
    //        for (b in pairs[1]) {
    //              if (collide(a, b)) {
    //              a.handle_collision(group, b);
    //              b.handle_collision(group, a);
    //         }
    //}
    //return
}


void World::exit() {
    clear();
}

