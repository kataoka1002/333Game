#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;
	const int MAX_POINT_LIGHT = 128;

	//�f�B���N�V�������C�g�\����
	struct DirectionLight
	{
		Vector3 ligDirection = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 ligColor = Vector3::Zero;
		float pad1 = 0.0f;
	};

	struct PointLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.01f;
	};

	//���C�g�\����
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//�f�B���N�V�������C�g�̔z��
		Vector3 eyePos = Vector3::Zero;						//�J�����̈ʒu
		float pad = 0.0f;
		PointLight pointLight[MAX_POINT_LIGHT];				//�|�C���g���C�g�̔z��
	};


	class SceneLight : public Noncopyable
	{
	public:
		//�R���X�g���N�^
		SceneLight();

		//�f�B���N�V�������C�g�̐ݒ�
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].ligDirection = direction;
			m_light.directionLight[lightNum].ligColor = color;
		}

		//�|�C���g���C�g�̐ݒ�
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_light.pointLight[lightNum].position = position;
			m_light.pointLight[lightNum].color = color;
			m_light.pointLight[lightNum].range = range;
		}

		//�|�C���g���C�g�̃|�W�V�����ݒ�
		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_light.pointLight[lightNum].position = position;
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

