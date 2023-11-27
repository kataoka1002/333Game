#include "stdafx.h"
#include "BossSwipeState.h"
#include "BossIdleState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossSwipeState::~BossSwipeState()
	{

	}

	void BossSwipeState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Swipe, 0.3f);
	}

	IBossState* BossSwipeState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossSwipeState::Update()
	{

	}
}
