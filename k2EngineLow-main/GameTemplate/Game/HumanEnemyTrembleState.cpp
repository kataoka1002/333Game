#include "stdafx.h"
#include "HumanEnemyTrembleState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyTrembleState::~HumanEnemyTrembleState()
	{

	}

	void HumanEnemyTrembleState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Dead, 0.5f);
	}

	IHumanEnemyState* HumanEnemyTrembleState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyTrembleState::Update()
	{
		
	}
}
