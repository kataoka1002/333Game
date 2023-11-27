#include "stdafx.h"
#include "BossWalkState.h"
#include "BossSwipeState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossWalkState::~BossWalkState()
	{

	}

	void BossWalkState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Walk, 0.3f);
	}

	IBossState* BossWalkState::StateChange()
	{
		if (m_boss->CheckDistanceToPlayer() < FOLLOW_RANGE)
		{
			return new BossSwipeState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossWalkState::Update()
	{
		//�v���C���[��ǂ�������
		m_boss->FollowPlayer();

		//��]����
		m_boss->Turn();
	}
}
