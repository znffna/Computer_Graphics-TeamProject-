#include "Map.hpp"

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
void Map::loadMap(const std::string& filename)
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
		char input{0};	// ���� ���� ���� � �����͸� �������� �ľ��ϱ� ����.
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
		else if (input == 'i') {
			// �����ϴ� ���� ����
			int floor;	// �������� �ִ� ��
			in >> floor;
			floor_item.push_back(floor);

		}
		else if (input == 'h') {
			in >> floor_height;
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

	// ������ ����
	{
		//item.push_back()
	}
}

//void Map::Map1()
//{
//	// ���� ������ �ʱ�ȭ
//	floor_rad.clear();
//	floor_member.clear();
//
//	for (int i = 0; i < 90; ++i) {
//		float rad = i * 12.0f;
//		floor_rad.push_back(rad);
//
//		std::vector<int> typelist;
//		for (int j = 0; j < 12; ++j) {
//			int type;
//			if (j < 3) {
//				type = 1; //���
//			}
//			else {
//				type = 2;
//			}
//			typelist.push_back(type);
//		}
//
//		floor_member.push_back(std::move(typelist));
//	}
//}

void Map::Map1()
{
	floor_rad.clear();
	floor_member.clear();

	for (int i = 0; i < 40; ++i) {
		float rad = 30.0f;
		floor_rad.push_back(rad);

		std::vector<int> typelist;

		for (int j = 0; j < 12; ++j) {

			int type = random_number(2, 4);
		
			typelist.push_back(type);
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

			tmp.get()->setTranslation({ 0.0f, start_height - floor_height * i, 0.0f });
			tmp.get()->setScale({ 5.0f, 0.2f, 5.0f });

			switch (floor_member[i][j]) {
			case 0:
				tmp.get()->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				break;		// finish		//��
			case 5:
				tmp.get()->setColor({ 0.0f, 0.0f, 0.0f, 1.0f });
				break;		// finish		//����

			case 1:continue;// ���� x ��� 

			case 2:tmp.get()->setColor({ 1.0f, 0.0f, 0.0f, 1.0f });
				break;		// die			// ����

			case 3:
				tmp.get()->setColor( { 0.75f, 0.75f, 0.75f, 1.0f });
				break;		// break��	// ȸ��
			case 4:
				tmp.get()->setColor({ 0.0f, 1.0f, 0.0f, 1.0f });
				break;		// ������		// �ʷ�
			}
			floor.push_back(tmp);

			tmp.get()->backup();

			world.add_object(tmp);
			auto o_tmp = std::static_pointer_cast<Object>(tmp);
			world.add_collision_pair("Ball:Pizza", NULLPTR, o_tmp);
		}
		map.push_back(std::move(floor));
	}
	// �߾� ��� ����		
	{
		std::shared_ptr<Pillar> tmp = std::make_shared<Pillar>(glm::vec3{ 1.5f, start_height * 1.5f, 1.5f });
		tmp.get()->setTranslation({ 0.0f, 0.0f, 0.0f });
		tmp.get()->setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		tmp.get()->backup();
		auto o_tmp = std::static_pointer_cast<Object>(tmp);
		world.add_object(o_tmp);
		//world.push_back(temp);
	}
	// ������ ����
	{
		for (int i = 0; i < floor_item.size(); ++i) {

			float rad = random_number(0.0f, 360.0f);
			std::shared_ptr<Cube> tmp = std::make_shared<Cube>(glm::vec3(3.0f, start_height - floor_height * floor_item[i]* 0.5 ,3.0f),  glm::vec3(0.0f, rad, 0.0f),  glm::vec3(0.5f) );
			tmp.get()->setColor({ 0.0f, 0.0f, 1.0f, 1.0f });
			tmp.get()->backup();
			item.push_back(tmp);
			world.add_object(tmp);
			auto o_tmp = std::static_pointer_cast<Object>(tmp);
			world.add_collision_pair("Ball:Cube", NULLPTR, o_tmp);
		}
	}

}