#include "Object.hpp"


std::shared_ptr<Object> NULLPTR;

//생성자
Object::Object(const std::shared_ptr<Mesh>& p) 	//생성자
{
	mesh = p;
}

// 복사 생성자
Object::Object(const Object& other)
{
	after_translation = other.after_translation;
	after_rotate = other.after_rotate;
	after_scale = other.after_scale;

	translation = other.translation;
	rotate = other.rotate;
	scale = other.scale;

	mesh = other.mesh;
	color = other.color;
}
// 복사 할당 연산자
Object& Object::operator=(const Object& other)
{
	if (this != &other) {
		after_translation = other.after_translation;
		after_rotate = other.after_rotate;
		after_scale = other.after_scale;

		translation = other.translation;
		rotate = other.rotate;
		scale = other.scale;

		mesh = other.mesh;
		color = other.color;
	}
	return *this;
}
// 이동 생성자
Object::Object(Object&& other) noexcept
{
	after_translation = std::move(other.after_translation);
	after_rotate = std::move(other.after_rotate);
	after_scale = std::move(other.after_scale);

	translation = std::move(other.translation);
	rotate = std::move(other.rotate);
	scale = std::move(other.scale);

	mesh = std::move(other.mesh);
	color = std::move(other.color);
}
// 이동 할당 연산자
Object& Object::operator=(Object&& other) noexcept
{
	if (this != &other) {
		after_translation = std::move(other.after_translation);
		after_rotate = std::move(other.after_rotate);
		after_scale = std::move(other.after_scale);

		translation = std::move(other.translation);
		rotate = std::move(other.rotate);
		scale = std::move(other.scale);

		mesh = std::move(other.mesh);
		color = std::move(other.color);
	}
	return *this;
}

// 소멸자
Object::~Object() {}

//Object 설정 초기화

void Object::transform_reset()
{
	after_translation = { 0.0f ,0.0f, 0.0f };
	after_rotate = { 0.0f ,0.0f, 0.0f };
	after_scale = { 1.0f ,1.0f, 1.0f };

	scale = { 0.3f ,0.3f, 0.3f };
	rotate = { 0.0f ,0.0f, 0.0f };
	translation = { 0.0f ,0.0f, 0.0f };
}

void Object::changemesh(const std::shared_ptr<Mesh>& rhs)
{
	mesh = rhs;
}

void Object::show_state() const
{
	std::cout << "--------------Object-------------------" << '\n';
	std::cout << "Translation : {"; print_vec3(translation); std::cout << "} " << '\n';
	std::cout << "Rotate : {"; print_vec3(rotate); std::cout << "} " << '\n';
	std::cout << "Scale : {"; print_vec3(scale); std::cout << "} " << '\n';
	std::cout << "After_Translation : {"; print_vec3(after_translation); std::cout << "} " << '\n';
	std::cout << "After_Rotate : {"; print_vec3(after_rotate); std::cout << "} " << '\n';
	std::cout << "After_Scale : {"; print_vec3(after_scale); std::cout << "} " << '\n';
	std::cout << "Mesh : " << (mesh ? (mesh.get()->get_name()) : "nullptr") << '\n';
	std::cout << "---------------------------------------" << '\n';
}

// --- getter / setter --- 
void Object::setTranslation(const glm::vec3& vector = {0.0f, 0.0f, 0.0f})
{
	translation = vector;
}
void Object::setRotate(const glm::vec3& vector = { 0.0f, 0.0f, 0.0f })
{
	rotate = vector;
}
void Object::setScale(const glm::vec3& vector = { 1.0f, 1.0f, 1.0f })
{
	scale = vector;
}

void Object::addTranslation(const glm::vec3& vector)
{
	translation += vector;
}
void Object::addRotate(const glm::vec3& vector)
{
	rotate += vector;
	rotate.x = degree_range_normalization(rotate.x);
	rotate.y = degree_range_normalization(rotate.y);
	rotate.z = degree_range_normalization(rotate.z);
}
void Object::addScale(const glm::vec3& vector)
{
	scale += vector;
}

glm::vec3 Object::getRotate() const
{
	return rotate;
}
glm::vec3 Object::getTranslation() const
{
	return translation;
}
glm::vec3 Object::getScale() const
{
	return scale;
}

void Object::backup()
{
	reset_translation = translation;
	reset_rotate = rotate;
	reset_scale = scale;
}

void Object::rollback()
{
	translation = reset_translation;
	rotate = reset_rotate;
	scale = reset_scale;
	setInvaild(false);
}


void Object::setColor(const glm::vec4& rhs)
{
	color = rhs;
}

glm::vec4 Object::getColor() const
{
	return color;
}


//--- 위치를 주어진 각도만큼 회전해서 저장함.
void Object::translation_rotate(const glm::vec3& degrees) {
	glm::mat4 transform{ 1.0f };
	transform = glm::rotate(transform, glm::radians(degrees.x), x_axis);
	transform = glm::rotate(transform, glm::radians(degrees.y), y_axis);
	transform = glm::rotate(transform, glm::radians(degrees.z), z_axis);
	translation = transform * glm::vec4{ translation, 1.0f };
}

bool Object::Collision(const Object& other) const {

	float other_l = other.translation.x - other.scale.x;
	float other_r = other.translation.x + other.scale.x;
	float other_b = other.translation.y - other.scale.y;
	float other_t = other.translation.y + other.scale.y;

	float this_l = translation.x - scale.x;
	float this_r = translation.x + scale.x;
	float this_b = translation.y - scale.y;
	float this_t = translation.y + scale.y;

	// 왼쪽 끝보다 우측끝이 더 왼쪽.
	if (other_l > this_r) {	return false; }
	// 우측 끝보다 좌측끝이 더 우측.
	if (other_r < this_l) {	return false; }
	// 마우스의 상단보다 폴리곤 하단이 더 위
	if (other_b > this_t) {	return false; }
	// 마우스의 하단보다 폴리곤 상단이 더 아래
	if (other_t < this_b) {	return false; }
	return true;
}


void Object::World_Transform(glm::mat4& transformMatrix) const {
	World_after_Translation(transformMatrix);
	World_after_rotate(transformMatrix);

	{
		transformMatrix = glm::translate(transformMatrix, translation); //--- 이동(Translation)	
		transformMatrix = glm::rotate(transformMatrix, glm::radians(rotate.x), x_axis);	//--- x축 기준 회전(Rotate)
		transformMatrix = glm::rotate(transformMatrix, glm::radians(rotate.y), y_axis);	//--- y축 기준 회전(Rotate)
		transformMatrix = glm::rotate(transformMatrix, glm::radians(rotate.z), z_axis);	//--- y축 기준 회전(Rotate)
		transformMatrix = glm::scale(transformMatrix, scale);		//--- 확대축소 변환(Scaling)			
	}
}

void Object::Normal_Transform(glm::mat4& transformMatrix) const {
	World_after_rotate(transformMatrix);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(rotate.x), x_axis);	//--- x축 기준 회전(Rotate)
	transformMatrix = glm::rotate(transformMatrix, glm::radians(rotate.y), y_axis);	//--- y축 기준 회전(Rotate)
	transformMatrix = glm::rotate(transformMatrix, glm::radians(rotate.z), z_axis);	//--- y축 기준 회전(Rotate)
}



void Object::World_after_Scale(glm::mat4& transformMatrix) const {
	transformMatrix = glm::scale(transformMatrix, scale);		//--- 확대축소 변환(Scaling)					
}

void Object::World_after_Translation(glm::mat4& transformMatrix) const {
	transformMatrix = glm::translate(transformMatrix, after_translation); //--- 이동(Translation)	
}

void Object::World_after_rotate(glm::mat4& transformMatrix) const {
	transformMatrix = glm::rotate(transformMatrix, glm::radians(after_rotate.x), x_axis);	//--- x축 기준 회전(Rotate)
	transformMatrix = glm::rotate(transformMatrix, glm::radians(after_rotate.y), y_axis);	//--- y축 기준 회전(Rotate)
	transformMatrix = glm::rotate(transformMatrix, glm::radians(after_rotate.z), z_axis);	//--- y축 기준 회전(Rotate)
}

// 다형성을 위한 기본 옵션

void Object::draw() const
{
	mesh.get()->AUTO_Draw();
}

void Object::render() const
{
	//std::cout << "Object의 render() 호출" << '\n';
	draw();
}

void Object::handle_events(unsigned char key, const std::string&) {
	std::cout << "handle_events() 함수가 override 안된 경우 출력되는 줄." << '\n';
}

void Object::update()
{
	std::cout << "update() 함수가 override 안된 경우 출력되는 줄." << '\n';
}

int Object::handle_collision(const std::string& group, const std::shared_ptr<Object>& other)
{
	std::cout << "handle_collision() 함수가 override 안된 경우 출력되는 줄." << '\n';
	//std::cout << "Object의 handle_collision() 호출" << '\n';
	//std::cout << "아직 충돌 핸들판정에 오버로딩 된 값이 없음. " << '\n';
	return 0;
}
