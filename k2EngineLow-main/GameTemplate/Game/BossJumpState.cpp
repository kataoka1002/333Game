#include "stdafx.h"
#include "BossJumpState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossJumpState::~BossJumpState()
	{

	}

	void BossJumpState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Jump, 0.3f);
	}

	IBossState* BossJumpState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossJumpState::Update()
	{

	}
}
