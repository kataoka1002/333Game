#include "stdafx.h"
#include "BossMagicState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossMagicState::~BossMagicState()
	{

	}

	void BossMagicState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Magic, 0.3f);
	}
	
	IBossState* BossMagicState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossMagicState::Update()
	{

	}
}
