#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow 
{
	SceneLight::SceneLight()
	{
		//�J�����̈ʒu�̐ݒ�
		//m_light.eyePos = g_camera3D->GetPosition();
	}

	bool SceneLight::Start()
	{
		//�J�����̈ʒu�̐ݒ�
		m_light.eyePos = g_camera3D->GetPosition();
		return true;
	}
}