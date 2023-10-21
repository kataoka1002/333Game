#pragma once
namespace nsK2EngineLow
{
	const int MAX_DIRECTION_LIGHT = 4;
	const int MAX_POINT_LIGHT = 4;
	const int MAX_SPOT_LIGHT = 4;

	//�f�B���N�V�������C�g�\����
	struct DirectionLight
	{
		Vector3 direction = Vector3::Zero;
		float pad0 = 0.0f;
		Vector3 color = Vector3::Zero;
		float pad1 = 0.0f;
	};

	//�|�C���g���C�g�\����
	struct PointLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
	};

	//�X�|�b�g���C�g�\����
	struct SpotLight
	{
		Vector3 position = Vector3::Zero;
		float pad = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
		Vector3 direction = Vector3::Zero;
		float angle = 0.0f;
	};

	//�������C�g�\����
	struct HemLight
	{
		Vector3 skyColor = Vector3::Zero;
		float pad = 0.0f;
		Vector3 groundColor = Vector3::Zero;
		float pad2 = 0.0f;
		Vector3 groundNormal = Vector3::Zero;
	};


	//���C�g�\����
	struct Light
	{
		DirectionLight directionLight[MAX_DIRECTION_LIGHT];	//�f�B���N�V�������C�g�̔z��
		Vector3 eyePos = Vector3::Zero;						//�J�����̈ʒu
		float pad = 0.0f;
		PointLight pointLight[MAX_POINT_LIGHT];				//�|�C���g���C�g�̔z��
		SpotLight spotLight[MAX_SPOT_LIGHT];				//�X�|�b�g���C�g�̔z��
		HemLight hemLight;									//�������C�g
		float pad1 = 0.0f;
		Matrix mLVP[NUM_SHADOW_MAP];						//���C�g�r���[�v���W�F�N�V�����s��
	};


	class SceneLight : public Noncopyable
	{
	public:
		//�R���X�g���N�^
		SceneLight();
		bool Start();

		//�f�B���N�V�������C�g�̐ݒ�
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_light.directionLight[lightNum].direction = direction;
			m_light.directionLight[lightNum].color = color;
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

		//�X�|�b�g���C�g�̐ݒ�
		void SetSpotLight(int lightNum, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_light.spotLight[lightNum].position = position;
			m_light.spotLight[lightNum].color = color;
			m_light.spotLight[lightNum].range = range;
			m_light.spotLight[lightNum].direction = direction;
			m_light.spotLight[lightNum].angle = angle;
		}

		//�������C�g�̐ݒ�
		void SetHemLight(Vector3 skyColor, Vector3 groundColor, Vector3 normal)
		{
			m_light.hemLight.skyColor = skyColor;
			m_light.hemLight.groundColor = groundColor;
			m_light.hemLight.groundNormal = normal;
		}

		void SetLVP(Matrix mat, int num)
		{
			m_light.mLVP[num] = mat;
		}

		void SetEyePos(Vector3 pos)
		{
			m_light.eyePos = pos;
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

