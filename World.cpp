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
    // ��ü���� �ִϸ���Ʈ ��Ŵ.
    for (std::shared_ptr<Object>& object : objects) {
        object.get()->update();
    }

    // �浹���� ����.
    return handle_collisions();
}

void World::handle_events(unsigned char key, const std::string& state)
{
    // ��� ��ü�� ���� �����Ұ� �ִٸ� ���� -> RŰ�� ���� �ʱ�ȭ ������ �Ҷ�?
    // ��ü�� ���� ������ �Է� �̺�Ʈ ����.
    for (std::shared_ptr<Object>& object : objects) {
        object.get()->handle_events(key, state);
    }
}

// ���
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

void World::reset()
{
    for (auto p : objects) {
        p.get()->rollback();
    }
}

// �浹 üũ �˻� 
bool World::collide(std::shared_ptr<Object>& first, std::shared_ptr<Object>& second)
{
    // ù �Ķ���� down_casting
    auto first1 = std::dynamic_pointer_cast<Ball>(first);
    auto first2 = std::dynamic_pointer_cast<Pizza>(first);
    auto first3 = std::dynamic_pointer_cast<Cube>(first);

    // ������ �Ķ���� down_casting
    auto second1 = std::dynamic_pointer_cast<Ball>(second);
    auto second2 = std::dynamic_pointer_cast<Pizza>(second);
    auto second3 = std::dynamic_pointer_cast<Cube>(second);

    if (first1 and second2) {   // Ball : Pizza
        return Check_collision(first1, second2);
    }
    else if (first1 and second3) { // Ball : Cube
        return Check_collision(first1, second3);
    }
    std::cout << "�浹���� �Լ��� ����. ("<< typeid(first).name() << ", " << typeid(second).name() << ")" << '\n';
    return false;
}

bool World::Check_collision(std::shared_ptr<Ball>& ball, std::shared_ptr<Pizza>& pizza) {
    if (pizza.get()->getInvaild()) {
        // invaild �������ϰ�� pass �ع�����.
        return false;
    }
    float ball_rad = ball.get()->getRotate().y;
    float pizza_rad = pizza.get()->getRotate().y;
    degree_range_normalization(pizza_rad);
    // x,z���� �ϴ� ball�� ���� ���
    float theta = glm::asin(ball.get()->getScale().x / ball.get()->getRadius());
    //TODO 360���� 0�� ���̿����� ���� ���� ���� �ʿ�
    // 0.0< ball_rad < 30.0f �� ��� 360.0 < ball_rad + 360.0f < 390.0 �� ��쵵 üũ�ؾ� �Ѵ�.

    if ((pizza_rad - 30.0f - theta <= ball_rad and ball_rad < pizza_rad + theta)or (pizza_rad - 30.0f - theta <= ball_rad - 360.0f and ball_rad - 360.0f < pizza_rad + theta + 360.0f)) {
        // y�� �� �� ����
        float ball_mid = ball.get()->getTranslation().y;
        float ball_height = ball.get()->getScale().y;
        float pizza_mid = pizza.get()->getTranslation().y;
        float pizza_height = pizza.get()->getScale().y;

        // Ball �� �ٴ�y���� pizza�� ��ĥ�� true ����.
        if (ball_mid - ball_height < pizza_mid + pizza_height and ball_mid + ball_height > pizza_mid - pizza_height) {
            if (debug)
            {
                std::cout << "�浹���� (Ball : Pizza)" << '\n';
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
        // �ϴ� �߽ɰ��� �Ÿ��� size �պ��� ������� true;
        return true;
    }
    return false;
}

// �浹 ���� ����Ʈ�� �߰�
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
    //if a : // a�� ���� ��, ��, a�� None �� �ƴϸ�..
    //    collision_pairs[group][0].append(a)
    //if b:
    //    collision_pairs[group][1].append(b)
}

int World::handle_collisions()
{
    // ��ϵ� ��� �浹 ��Ȳ�� ���ؼ� �浹 �˻� �� �浹 ó�� ����.

    for (auto& o : collision_pairs) {
        auto& key = o.first;
        auto& data = o.second;

        auto& first = data.at(0);
        auto& second = data.at(1);
        for (auto f_o : first) {
            int cnt{ 0 };
            for (auto s_o : second) {
                if (collide(f_o, s_o)) {
                    // �浹�� �ش� group�� ������ ����.
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

