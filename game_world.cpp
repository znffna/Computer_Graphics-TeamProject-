#include "game_world.h"

World world;

void World::add_objects(std::vector<Object>& object_list)
{
    for (Object& o : object_list) {
        objects.push_back(&object_list);
    }
}
    
void World::update()
{
    // ��ü���� �ִϸ���Ʈ ��Ŵ.
    for (std::vector<Object>* layer : objects) {
        for (Object& o : (*layer)) {
            o.update();
        }
    }
}

// ���
void World::render()
{
    for (std::vector<Object>* layer : objects) {
        for (Object& o : (*layer)) {
            o.draw();
        }
    }
}

void World::remove_collision_object(Object& o)
{
    for (auto iter = collision_pairs.begin(); iter != collision_pairs.end(); ++iter) {
        auto pairs = iter->second;
        auto key = iter->first;

        auto first = pairs.first;
        auto second = pairs.second;

        auto first_search = std::find(first->begin(), first->end(), o);
        if (first_search != first->end()) {
            first->erase(first_search);
        }

        auto second_search = std::find(second->begin(), second->end(), o);
        if (second_search != second->end()) {
            second->erase(second_search);
        }
    }

    //for (pairs in collision_pairs) {
    //    if (o in pairs[0]) {
    //        pairs[0].remove(o);
    //    }
    //    if (o in pairs[1]) {
    //        pairs[1].remove(o);
    //    }            
    //}
}


void World::remove_object(Object& o)
{
    for (std::vector<Object>* layer : objects) {
        auto search = std::find(layer->begin(), layer->end(), o);
        if (search != layer.end()) {
            layer->erase(search);  // �ð��� ���忡�� �����.
            remove_collision_object(o);  // �浹 �׷쿡�� ���� �Ϸ�
            // del o;  // ��ü ��ü�� ������ �޸𸮿��� ����. - std::shared_ptr�� ����Ű�� �ְ� ������ �˾Ƽ� ���ŵ�.
            return;
        }           
    }
    // �Ʒ� cout �� ����Ǹ� �ش� �Ű������� world�� �������� ����.
    std::cout << "Cannot delete non existing object";
}


void World::clear()
{
    objects.clear();
    collision_pairs.clear();
}


bool World::collide(Object& a, Object& b)
{
    std::vector<float> a_bb = a.get_bb();
    std::vector<float> b_bb = b.get_bb();
    //la, ba, ra, ta = a.get_bb()
    //lb, bb, rb, tb = b.get_bb()

    if (a_bb[0] > b_bb[2]) return false;
    if (a_bb[2] < b_bb[0]) return false;
    if (a_bb[3] < b_bb[1]) return false;
    if (a_bb[1] > b_bb[3]) return false;

    return true;
}


void World::add_collision_pair(std::string& group, Object& a, Object& b)
{
    if (group not in collision_pairs)
    {
            std::cout << f"New group : {group} added." << '\n';
                collision_pairs[group] = [[],[]]
    }
    if a : // a�� ���� ��, ��, a�� None �� �ƴϸ�..
        collision_pairs[group][0].append(a)
    if b:
        collision_pairs[group][1].append(b)
}


void World::handle_collisions()
{
    // ��ϵ� ��� �浹 ��Ȳ�� ���ؼ� �浹 �˻� �� �浹 ó�� ����.

    //for group, pairs in collision_pairs.items() : # key 'boy:ball', value [[], []]
    //    for a in pairs[0]:
    //for (b in pairs[1]) {
    //    if (collide(a, b)) {
    //        a.handle_collision(group, b);
    //        b.handle_collision(group, a);
    //    }
    //}
    //return
}


void World::exit() {
		
}

