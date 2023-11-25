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
    // 객체들을 애니메이트 시킴.
    for (std::shared_ptr<Object>& object : objects) {
        object.get()->update();
    }
}

// 출력
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
        // map에서 값을 꺼낸걸 정리
        auto key = iter->first;
        auto list = iter->second;

        auto& first = list.at(0);
        auto& second = list.at(1);
        // 찾을 경우 제거
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
        objects.erase(search);  // 시각적 월드에서 지운다.
        remove_collision_object(object);  // 충돌 그룹에서 삭제 완료
        // del o;  // 객체 자체를 완전히 메모리에서 제거. - std::shared_ptr은 가르키는 애가 없으면 알아서 제거됨.
        return;
    }           
    // 아래 cout 이 실행되면 해당 매개변수는 world에 존재하지 않음.
    std::cout << "없는 객체를 제거할 수 없음" << '\n';
    //std::cout << "Cannot delete non existing object";
}


void World::clear()
{
    objects.clear();
    collision_pairs.clear();
}

// 충돌 체크 검사 (AABB)
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

// 충돌 판정 리스트에 추가
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
    //if a : // a가 있을 떄, 즉, a가 None 이 아니면..
    //    collision_pairs[group][0].append(a)
    //if b:
    //    collision_pairs[group][1].append(b)
}


void World::handle_collisions()
{
    // 등록된 모든 충돌 상황에 대해서 충돌 검사 및 충돌 처리 수행.

    for (auto& o : collision_pairs) {
        auto& key = o.first;
        auto& data = o.second;

        auto& first = data.at(0);
        auto& second = data.at(1);

        for (auto f_o : first) {
            for (auto s_o : second) {
                if (collide(f_o, s_o)) {
                    // 충돌시 해당 group과 상대방을 리턴.
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

