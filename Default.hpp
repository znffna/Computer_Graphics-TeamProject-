
#pragma once



#ifndef DEFAULT_HPP
#define DEFAULT_HPP

// Cpp header include
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <map>
// OpenGL header include
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
// glm header include
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


// fmod header include
#include "fmod.hpp"
#include "fmod_errors.h"

//축의 방향 벡터
const glm::vec3 x_axis{ 1.0f, 0.0f, 0.0f };
const glm::vec3 y_axis{ 0.0f, 1.0f, 0.0f };
const glm::vec3 z_axis{ 0.0f, 0.0f, 1.0f };
const glm::vec3 sign{ -1.0f , -1.0f, -1.0f };

const glm::vec3 rainbow[] = {
	{1.0f, 0.0f, 0.0f},		//빨강
	{0.0f, 1.0f, 0.0f},		//초록
	{1.0f, 0.5f, 0.0f},		//주황
	{1.0f, 1.0f, 0.0f},		//노랑
	{0.0f, 0.0f, 1.0f},		//파랑
	{0.0f, 0.02f, 1.0f},	//남색
	{0.4f, 0.0f, 1.0f},		//보라
	{0.5f, 0.1f, 0.15f}		//와인
};


//윈도우 크기 설정
extern float window_row;			//윈도우 크기(가로)
extern float window_col;			//윈도우 크기(세로)
//error가 있을시 받아서 출력하는 함수.
bool opengl_error();

// mt19937에 의한 랜덤 실수값 생성
float random_number(const float& , const float& );
// mt19937에 의한 랜덤 정수값 생성
int random_number(const int& , const int& );	
// 각도를 0~360 사이로 유지하는 정규화 함수
float degree_range_normalization(const float& );

// 2차 베지어 곡선 위치 계산 함수
glm::vec3 CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

// glm::vec3 출력 함수
void print_vec3(const glm::vec3&);

// 이미지 로드 함수
GLuint loadTexture(const char* imagePath);

// 사운드 관련 연습용 코드
extern FMOD::System* ssystem;	//Sound System 약자임.
extern FMOD::Sound* sound1, * sound2;	// 사용할 사운드가 동적할당될텐데 그걸 가르키는 포인터
extern FMOD::Channel* channel;
extern FMOD_RESULT result;
extern void* extradriverdata;
extern bool perspective;
#endif //