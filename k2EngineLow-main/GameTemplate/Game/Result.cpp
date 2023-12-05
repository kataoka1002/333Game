#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "BlackFade.h"

bool Result::Start()
{
	//�A�j���[�V�����̏�����
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
	animationClips[enAnimClip_Dwarf].Load("Assets/animData/player/player_dwarf.tka");
	animationClips[enAnimClip_Dwarf].SetLoopFlag(false);

	//�v���C���[���f���̏�����
	m_playerModel.Init("Assets/modelData/player/player.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_playerModel.SetPosition(m_playerPosition);
	m_playerModel.Update();

	//�w�i���f���̏�����
	m_bgModel.Init("Assets/modelData/map/map3.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bgModel.SetPosition(m_playerPosition);
	m_bgModel.Update();

	//������
	InitCamera();

	//UI�̍쐬
	InitUI();

	//�t�F�[�h�N���X�̍쐬
	m_blackFade = NewGO<BlackFade>(1, "blackfade");

	return true;
}

void Result::GoTitle()
{
	//A�{�^���Ń^�C�g���֖߂�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//A�{�^���������ꂽ��t�F�[�h�C��
		m_pressAButton = true;

		m_blackFade->SetAlphaUp(true);
	}

	if (m_pressAButton)
	{
		//�t�F�[�h�C�����I�������
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//�^�C�g���̍쐬
			NewGO<Title>(0, "title");

			//�폜����
			OnDestroy();
		}
	}
}

void Result::SetAnimation()
{
	//�h���[�t�̃A�j���[�V�������Đ�����Ă���Ȃ�
	if (m_didPlayAnim == true)
	{
		return;
	}

	//���Ԃ̌o�߂𑪂�
	m_time += g_gameTime->GetFrameDeltaTime();

	if (m_time >= 1.0f)
	{
		//1�b�o�߂Ń��[�V�����Đ�
		m_currentAnimationClip = enAnimClip_Dwarf;
	}

	//�A�j���[�V�������I�������
	if (m_playerModel.IsPlayingAnimation() == false)
	{
		//�A�C�h���𗬂�
		m_currentAnimationClip = enAnimClip_Idle;

		//�h���[�t�A�j���Đ��ς݂ɂ���
		m_didPlayAnim = true;
	}
}