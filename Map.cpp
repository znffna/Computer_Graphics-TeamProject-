#include "Map.hpp"
#include "Default.hpp"
#include "Object.hpp"
#include "Game_world.hpp"

bool Map::debug{ false };
Map map;


// ���� �����ϴ� �Լ�
void Map::outputMap(const std::string& filename)
{
	std::ofstream out{ filename };
	for (int i = 0; i < floor_rad.size(); ++i) {
		out << "f " << floor_rad[i] << ' ';
		for (int p : floor_member[i]) {
			out << p << ' ';
		}
		out << '\n';
	}
}

// �� ���� �о���� �Լ�.
void Map::loadMap(const std::string & filename)
{
	std::ifstream in{ filename };
	if (not in) {
		std::cout << filename << "�� ���� ����." << '\n';
		return;
	}
	// ���� ������ �ʱ�ȭ
	floor_rad.clear();
	floor_member.clear();

	// ������ �о� ����
	while (!in.eof()) {
		char input;	// ���� ���� ���� � �����͸� �������� �ľ��ϱ� ����.
		in >> input;

		if (input == 'f') {	//floor ���� �о�� �� ���
			// ������ �о� ����
			float rad;
			in >> rad;
			floor_rad.push_back(rad);

			// Ÿ���� �о� ����
			std::vector<int> typelist;
			for (int i = 0; i < 12; ++i) {
				int type;
				in >> type;
				typelist.push_back(std::move(type));
			}
			floor_member.push_back(std::move(typelist));
		}
	}

	if (debug) {
		std::cout << filename << "�� ������ �б� �Ϸ�." << '\n';
		std::cout << "�о�� ���� : " << floor_rad.size() << '\n';
	}
}
// �� �ڵ� ����� ���� �Լ�
void Map::exampleMap()
{
	// ���� ������ �ʱ�ȭ
	floor_rad.clear();
	floor_member.clear();
	// ���� ���� ����
	for (int i = 0; i < 30; ++i) {
		// �ش� ���� ȸ�� ����
		float rad = random_number(0.0f, 30.0f);	// �̰� 0.0f ~ 360.0f ���� ���� �ص� ��.

		floor_rad.push_back(rad);

		// �ش� ���� �� ���� ����
		std::vector<int> typelist;
		for (int j = 0; j < 12; ++j) {
			// ���� ������ ���⼭ ���� (�ӽ��ڵ�� ����������)
			typelist.push_back(random_number(0, 3));
		}

		floor_member.push_back(std::move(typelist));
	}
}

// �ҷ��� �����ͷ� ���� ������Ʈ �����ϴ� �Լ�
void Map::makeMap()
{
	if (floor_rad.size() == 0) {
		std::cout << "map.makeMap() : ���� �ҷ��� �����Ͱ� ����" << '\n';
		return;
	}
	float start_height = 5.0f * floor_rad.size() / 2.0f;
	// ���� ���� ����
	for (int i = 0; i < floor_rad.size(); ++i) {
		std::vector< std::shared_ptr<Object> > floor;
		float rad = floor_rad[i];
		for (int j = 0; j < floor_member[i].size(); ++j) {
			std::shared_ptr<Pizza> tmp = std::make_shared<Pizza>(30.0f * j + rad, floor_member[i][j]);
			tmp.get()->setTranslation({ 0.0f, start_height - 5.0f * i, 0.0f });
			tmp.get()->setScale({ 5.0f, 0.2f, 5.0f });
			tmp.get()->setColor(rainbow[floor_member[i][j] % 8]);
			floor.push_back(tmp);

			world.add_object(tmp);
			auto o_tmp = std::static_pointer_cast<Object>(tmp);
			world.add_collision_pair("Ball:Pizza", NULLPTR, o_tmp);
		}
		map.push_back(std::move(floor));
	}
	// �߾� ��� ����		
	{
		std::shared_ptr<Object> tmp = std::make_shared<Object>(PILLAR);
		tmp.get()->setTranslation({ 0.0f, 0.0f, 0.0f });
		tmp.get()->setScale({ 1.5f, start_height, 1.5f });
		tmp.get()->setColor({ 1.0f, 1.0f, 1.0f });

		world.add_object(tmp);
		//world.push_back(temp);
	}
}