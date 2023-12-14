#include "World.hpp"

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
void World::add_object(std::shared_ptr<Pizza>& object)
{
    objects.push_back(object);
}
void World::add_object(std::shared_ptr<Cube>& object)
{
    objects.push_back(object);
}
void World::add_object(std::shared_ptr<Square>& object)
{
    objects.push_back(object);
}


void World::add_objects(std::vector<std::shared_ptr<Object>>& object_vector)
{
    for (std::shared_ptr<Object>& object : object_vector) {
        objects.push_back(object);
    }
}
    
int World::update()
{
    // 객체들을 애니메이트 시킴.
    for (std::shared_ptr<Object>& object : objects) {
        object.get()->update();
    }

    // 충돌판정 시작.
    return handle_collisions();
}

void World::handle_events(unsigned char key, const std::string& state)
{
    // 모든 객체에 따로 적용할게 있다면 실행 -> R키를 눌러 초기화 같은거 할때?
    // 객체들 마다 가지는 입력 이벤트 실행.
    for (std::shared_ptr<Object>& object : objects) {
        object.get()->handle_events(key, state);
    }
}

// 출력
void World::render(Shader& shader)
{
    int cnt{0};
    for (std::shared_ptr<Object>& object : objects) {
        shader.setColor({ object.get()->getColor() });
        shader.draw_object(*object.get());
        if (object.get()->getTexture()) {
            shader.enableTexture();
        }
        else {
            shader.disableTexture();
        }
        object.get()->render();
        cnt++;
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

void World::reset()
{
    for (auto p : objects) {
        p.get()->rollback();
    }
}

// 충돌 체크 검사 
bool World::collide(std::shared_ptr<Object>& first, std::shared_ptr<Object>& second)
{
    // 첫 파라미터 down_casting
    auto first1 = std::dynamic_pointer_cast<Ball>(first);
    auto first2 = std::dynamic_pointer_cast<Pizza>(first);
    auto first3 = std::dynamic_pointer_cast<Cube>(first);

    // 세컨드 파라미터 down_casting
    auto second1 = std::dynamic_pointer_cast<Ball>(second);
    auto second2 = std::dynamic_pointer_cast<Pizza>(second);
    auto second3 = std::dynamic_pointer_cast<Cube>(second);

    if (first1 and second2) {   // Ball : Pizza
        return Check_collision(first1, second2);
    }
    else if (first1 and second3) { // Ball : Cube
        return Check_collision(first1, second3);
    }
    std::cout << "충돌판정 함수가 없음. ("<< typeid(first).name() << ", " << typeid(second).name() << ")" << '\n';
    return false;
}

bool World::Check_collision(std::shared_ptr<Ball>& ball, std::shared_ptr<Pizza>& pizza) {
    if (pizza.get()->getInvaild()) {
        // invaild 데이터일경우 pass 해버린다.
        return false;
    }
    float ball_rad = ball.get()->getRotate().y;
    float pizza_rad = pizza.get()->getRotate().y;
    degree_range_normalization(pizza_rad);
    // x,z축이 일단 ball과 같은 경우
    float theta = glm::asin(ball.get()->getScale().x / ball.get()->getRadius());
    //TODO 360도와 0도 사이에서의 각도 계산식 개선 필요
    // 0.0< ball_rad < 30.0f 의 경우 360.0 < ball_rad + 360.0f < 390.0 의 경우도 체크해야 한다.

    if ((pizza_rad - 30.0f - theta <= ball_rad and ball_rad < pizza_rad + theta)or (pizza_rad - 30.0f - theta <= ball_rad - 360.0f and ball_rad - 360.0f < pizza_rad + theta + 360.0f)) {
        // y축 값 비교 시작
        float ball_mid = ball.get()->getTranslation().y;
        float ball_height = ball.get()->getScale().y;
        float pizza_mid = pizza.get()->getTranslation().y;
        float pizza_height = pizza.get()->getScale().y;

        // Ball 의 바닥y값이 pizza와 겹칠시 true 리턴.
        if (ball_mid - ball_height < pizza_mid + pizza_height and ball_mid + ball_height > pizza_mid - pizza_height) {
            if (debug)
            {
                std::cout << "충돌판정 (Ball : Pizza)" << '\n';
                std::cout << "Ball.rad :" << ball_rad << " : Pizza.rad :" << pizza_rad << '\n';
            }
            return true;
        }
    }
    return false;
}

bool World::Check_collision(std::shared_ptr<Ball>& ball, std::shared_ptr<Cube>& cube) {
    if(debug)
        std::cout << "Check_collision(ball:cube)" << '\n';
    glm::vec3 ball_pos = ball.get()->getTranslation();
    glm::vec3 cube_pos = cube.get()->getTranslation();

    if (glm::length(ball_pos - cube_pos) < ball.get()->getScale().x + (cube.get()->getScale().x * sqrt(3))) {
        // 일단 중심간의 거리가 size 합보다 작은경우 true;
        return true;
    }
    return false;
}

// 충돌 판정 리스트에 추가
void World::add_collision_pair(const std::string& group, std::shared_ptr<Object>& a, std::shared_ptr<Object>& b)
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
            std::cout << "New group : " << group << "' added." << '\n';
            std::cout << "New group.size() : " << collision_pairs.size() << '\n';
            std::cout << "New group.first.size() : " << collision_pairs[group].at(0).size() << '\n';
            std::cout << "New group.second.size() : " << collision_pairs[group].at(1).size() << '\n';
        }
    }

    if (a.get()) {
        collision_pairs[group][0].insert(a);
        if (debug)
        {
            std::cout << "group : " << group << '\n';
            std::cout << "New group.first.size() : " << collision_pairs[group].at(0).size() << '\n';
            std::cout << "New group.second.size() : " << collision_pairs[group].at(1).size() << '\n';
        }
    }

    if (b.get()) {
        collision_pairs[group][1].insert(b);
        if (debug)
        {
            std::cout << "group : " << group << '\n';
            std::cout << "New group.first.size() : " << collision_pairs[group].at(0).size() << '\n';
            std::cout << "New group.second.size() : " << collision_pairs[group].at(1).size() << '\n';
        }
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

int World::handle_collisions()
{
    // 등록된 모든 충돌 상황에 대해서 충돌 검사 및 충돌 처리 수행.

    for (auto& o : collision_pairs) {
        auto& key = o.first;
        auto& data = o.second;

        auto& first = data.at(0);
        auto& second = data.at(1);
        for (auto f_o : first) {
            int cnt{ 0 };
            for (auto s_o : second) {
                if (collide(f_o, s_o)) {
                    // 충돌시 해당 group과 상대방을 리턴.
                    int result{ 0 };
                    result += f_o.get()->handle_collision(key, s_o);
                    if (result > 0) {
                        return 1;
                    }
                    result += s_o.get()->handle_collision(key, f_o);
                    if (result > 0) {
                        return 1;
                    }
                }
            }
        }
    }
    
    return 0;
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

void World::clear() {
    objects.clear();
    collision_pairs.clear();
}

