#include "stdafx.h"
#include "BossHitReactionState.h"
#include "BossIdleState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossHitReactionState::~BossHitReactionState()
	{

	}

	void BossHitReactionState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Reaction, 0.3f);
	}

	IBossState* BossHitReactionState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossHitReactionState::Update()
	{
		
	}
}