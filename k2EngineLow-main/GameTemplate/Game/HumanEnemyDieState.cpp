#include "stdafx.h"
#include "HumanEnemyDieState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyDieState::~HumanEnemyDieState()
	{

	}

	void HumanEnemyDieState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Die, 0.5f);
	}

	IHumanEnemyState* HumanEnemyDieState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyDieState::Update()
	{
		// �A�j���[�V�����̍Đ����I�������
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			// �폜
			m_enemy->OnDestroy();
		}
	}
}
