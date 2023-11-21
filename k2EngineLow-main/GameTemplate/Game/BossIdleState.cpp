#include "stdafx.h"
#include "BossIdleState.h"
#include "BossDeadState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossIdleState::~BossIdleState()
	{

	}

	void BossIdleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Idle, 0.3f);
	}

	IBossState* BossIdleState::StateChange()
	{
		//�U�����󂯂���
		if (m_boss->DidAttackHit())
		{
			// �̗͂�0�ȉ��Ŏ��S
			if (m_boss->GetHP() <= 0.0f)
			{
				return new BossDeadState(m_boss);
			}
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossIdleState::Update()
	{

	}
}
