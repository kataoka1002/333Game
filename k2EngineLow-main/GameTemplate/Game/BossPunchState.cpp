#include "stdafx.h"
#include "BossPunchState.h"
#include "BossIdleState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossPunchState::~BossPunchState()
	{

	}

	void BossPunchState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Punch, 0.3f);
	}

	IBossState* BossPunchState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			//�q�b�g�̃N�[���_�E���^�C�����ɂ���
			m_boss->SetHitCoolDown();

			return new BossIdleState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossPunchState::Update()
	{

	}
}
