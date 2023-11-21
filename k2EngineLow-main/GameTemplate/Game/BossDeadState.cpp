#include "stdafx.h"
#include "BossDeadState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	BossDeadState::~BossDeadState()
	{

	}

	void BossDeadState::Enter()
	{
		// �Đ�����A�j���[�V������ݒ�B
		m_boss->SetAnimation(Boss::enAnimClip_Dead, 0.3f);
	}

	IBossState* BossDeadState::StateChange()
	{
		// �����܂ŗ�����X�e�[�g��J�ڂ��Ȃ��B
		return nullptr;
	}

	void BossDeadState::Update()
	{
		//�A�j���[�V�����̍Đ����I�������
		if (m_boss->GetIsPlayingAnimation() == false)
		{
			//�Q�[���N���A��ʂ�
			m_boss->GameClear();
		}
	}
}
