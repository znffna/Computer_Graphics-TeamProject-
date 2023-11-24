//--- fragment shader: fragment.glsl 파일에 저장
#version 330 core
//--- out_Color: 버텍스 세이더에서 입력받는 색상 값
//--- FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달 됨.
in vec3 out_Color; //--- 버텍스 세이더에게서 전달 받음
out vec4 FragColor; //--- 색상 출력

in vec4 FragPos; //--- 정점 위치값
in vec3 Normal; //--- 버텍스 세이더에서 받은 노멀값

uniform bool lightOption;
uniform vec3 lightPos; //--- 조명의 위치
uniform vec3 lightColor; //--- 조명의 색
//uniform vec3 objectColor; //--- 객체의 색

uniform vec3 viewPos;	//--- 카메라 위치

void main(void)
{
	if(lightOption){
		float ambientLight = 0.6; //--- 주변 조명 세기
		vec3 ambient = ambientLight * lightColor; //--- 주변 조명 값

		vec3 normalVector = normalize (Normal);
		vec3 lightDir = normalize (lightPos - vec3(FragPos));//--- 표면과 조명의 위치로 조명의 방향을 결정한다.
		float diffuseLight = max (dot (normalVector, lightDir), 0.0); //--- N과 L의 내적 값으로 강도 조절: 음수 방지
		vec3 diffuse = diffuseLight * lightColor; //--- 산란 반사 조명값: 산란반사값 * 조명색상값

		int shininess = 128; //--- 광택 계수

		vec3 viewDir = normalize (viewPos - vec3(FragPos));//--- 관찰자의 방향
		vec3 reflectDir = reflect (-lightDir, normalVector); //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
		float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V와 R의 내적값으로 강도 조절: 음수 방지
		specularLight = pow(specularLight, shininess); //--- shininess 승을 해주어 하이라이트를 만들어준다.
		vec3 specular = specularLight * lightColor; //--- 거울 반사 조명값: 거울반사값 * 조명색상값

		float len = distance( lightPos, vec3(FragPos)) / 5;
		//vec3 result = (ambient + diffuse + specular) * objectColor; //--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상
		vec3 result = (ambient + diffuse + specular)/ len * out_Color; //--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상
		FragColor = vec4 (result, 1.0); // --- 픽셀 색을 출력
	}
	else{
		FragColor = vec4 (out_Color, 1.0); // --- 조명 없이 픽셀 색을 출력
	}

	//FragColor = vec4 (out_Color, 1.0);
}