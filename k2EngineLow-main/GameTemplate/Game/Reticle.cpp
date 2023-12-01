#include "stdafx.h"
#include "Reticle.h"
#include "Player.h"
#include "GameInformation.h"

namespace
{
	//�h�b�g���e�B�N���̑傫��
	const Vector3 DOT_RETICLE_SCALE = { 0.8f, 0.8f, 1.0f };

	//�T�C�h���e�B�N���̑傫��(�ő�l)
	const Vector3 MAX_SIDE_RETICLE_SCALE = { 1.4f, 1.4f, 1.0f };

	//�T�C�h���e�B�N���̑傫��(�ŏ��l)
	const Vector3 MIN_SIDE_RETICLE_SCALE = { 0.0f, 0.0f, 1.0f };

	//�T�C�h���e�B�N���̑傫��������
	const Vector3 ADD_SIDE_RETICLE_SCALE = { 0.15f, 0.15f, 0.0f };
}

namespace nsPlayer
{

	Reticle::Reticle()
	{

	}

	Reticle::~Reticle()
	{

	}

	bool Reticle::Start()
	{
		m_player = FindGO<Player>("player");

		m_gameInfo = FindGO<GameInformation>("gameinformation");

		m_dotReticle.Init("Assets/spriteData/dotReticle.dds", 1600.0f, 900.0f);
		m_dotReticle.SetScale(DOT_RETICLE_SCALE);
		m_dotReticle.Update();

		m_sideReticle.Init("Assets/spriteData/sideReticle.dds", 1600.0f, 900.0f);
		m_sideReticle.SetScale(MIN_SIDE_RETICLE_SCALE);
		m_sideReticle.Update();

		return true;
	}

	void Reticle::Update()
	{
		//�傫���̌v�Z
		CalcScale();

		//�X�V
		m_dotReticle.Update();
		m_sideReticle.Update();
	}

	void Reticle::CalcScale()
	{
		//�v���C���[���I�u�W�F�N�g��ێ����Ă���Ȃ�
		if (m_player->GetIsHoldingObject())
		{
			//�傫������
			m_sideReticleScale += ADD_SIDE_RETICLE_SCALE;

			//���̑傫���ŌŒ�
			if (m_sideReticleScale.x >= 1.5f)
			{
				m_sideReticleScale = MAX_SIDE_RETICLE_SCALE;
			}
		}
		else
		{
			//����������
			m_sideReticleScale -= ADD_SIDE_RETICLE_SCALE;

			//���̑傫���ŌŒ�
			if (m_sideReticleScale.x <= 0.0f)
			{
				m_sideReticleScale = MIN_SIDE_RETICLE_SCALE;
			}
		}

		//�Z�b�g
		m_sideReticle.SetScale(m_sideReticleScale);
	}

	void Reticle::Render(RenderContext& rc)
	{
		if (m_gameInfo->GetIsInGame() == false)
		{
			return;
		}

		m_dotReticle.Draw(rc);
		m_sideReticle.Draw(rc);
	}
}