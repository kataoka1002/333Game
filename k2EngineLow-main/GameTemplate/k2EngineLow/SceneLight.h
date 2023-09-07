#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;

	//�f�B���N�V�������C�g�\����
	struct DirectionLight
	{
		Vector3 ligDirection = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 ligColor = Vector3::Zero;
		float pad1 = 0.0f;
	};

	//���C�g�\����
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//�f�B���N�V�������C�g�̔z��
	};


	class SceneLight : public Noncopyable
	{
	public:
		//�f�B���N�V�������C�g�̐ݒ�
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].ligDirection = direction;
			m_light.directionLight[lightNum].ligColor = color;
		}

		//�\���̂�Ԃ�
		Light& GetLight()
		{
			return m_light;
		}


	private:
		Light m_light;
	};

}

