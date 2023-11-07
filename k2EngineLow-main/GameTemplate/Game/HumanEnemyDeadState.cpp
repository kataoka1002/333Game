#include "stdafx.h"
#include "HumanEnemyDeadState.h"
#include "HumanEnemyCrawlState.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemyDeadState::~HumanEnemyDeadState()
	{

	}

	void HumanEnemyDeadState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_enemy->SetAnimation(HumanEnemy::enAnimClip_Dead, 0.5f);
	}

	IHumanEnemyState* HumanEnemyDeadState::StateChange()
	{
		// �A�j���[�V�����̍Đ����I�������
		if (m_enemy->GetIsPlayingAnimation() == false)
		{
			//�L�����R���̍Đݒ���s��
			m_enemy->ResetCharaCon();

			return new HumanEnemyCrawlState(m_enemy);
		}

		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void HumanEnemyDeadState::Update()
	{
		
	}
}