//--- vertex shader: vertex.glsl 파일에 저장
#version 330 core
//--- in_Position: attribute index 0
//--- in_Color: attribute index 1
layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec3 in_Normal; //--- 위치 변수: attribute position 1
layout (location = 2) in vec4 in_Color; //--- 컬러 변수: attribute position 2
layout (location = 3) in vec2 in_uv; //--- 텍셀 변수: attribute position 3

out vec4 FragPos;		//--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
out vec3 Normal;		//--- 노멀값을 프래그먼트 세이더로 보낸다.
out vec4 out_Color; //--- 프래그먼트 세이더에게 전달
out vec2 TexCoord;

uniform mat4 normalTransform;		//--- 노멀에 적용할 회전 행렬.
//--- 모델링 변환 연습
uniform mat4 modelTransform;		//--- 모델링 변환값
uniform mat4 viewTransform;			//--- 뷰잉 변환값
uniform mat4 projectionTransform;	//--- 투영 변환값


uniform vec4 uColor;		// 고정 색상
uniform bool vColor;		// 정정 색상 true, 고정 색상 false

void main(void) {
	// vertex
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4 (in_Position.x, in_Position.y, in_Position.z, 1.0);
	FragPos = (modelTransform * vec4(in_Position, 1.0));
	// normal
	Normal = vec3(normalTransform * vec4(in_Normal, 1.0));
	// texture
	TexCoord = in_uv;
	// color
	out_Color = vColor? in_Color: uColor;
}