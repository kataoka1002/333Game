#include "stdafx.h"
#include "BossJumpAttackState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossJumpAttackState::~BossJumpAttackState()
	{

	}

	void BossJumpAttackState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_JumpAttack, 0.3f);
	}

	IBossState* BossJumpAttackState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossJumpAttackState::Update()
	{

	}
}
