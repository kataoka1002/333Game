#include "stdafx.h"
#include "BossJumpAttackState.h"
#include "BossIdleState.h"

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

		//�ړI�n�̐ݒ�
		m_boss->SetJumpTarget();
	}

	IBossState* BossJumpAttackState::StateChange()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			return new BossIdleState(m_boss);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossJumpAttackState::Update()
	{
		//�W�����v�U���̏���
		m_boss->JumpAttack();
	}
}
