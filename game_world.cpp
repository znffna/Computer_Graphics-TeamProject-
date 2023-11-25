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
    // 객체들을 애니메이트 시킴.
    for (std::vector<Object>* layer : objects) {
        for (Object& o : (*layer)) {
            o.update();
        }
    }
}

// 출력
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
            layer->erase(search);  // 시각적 월드에서 지운다.
            remove_collision_object(o);  // 충돌 그룹에서 삭제 완료
            // del o;  // 객체 자체를 완전히 메모리에서 제거. - std::shared_ptr은 가르키는 애가 없으면 알아서 제거됨.
            return;
        }           
    }
    // 아래 cout 이 실행되면 해당 매개변수는 world에 존재하지 않음.
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
    if a : // a가 있을 떄, 즉, a가 None 이 아니면..
        collision_pairs[group][0].append(a)
    if b:
        collision_pairs[group][1].append(b)
}


void World::handle_collisions()
{
    // 등록된 모든 충돌 상황에 대해서 충돌 검사 및 충돌 처리 수행.

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

