#include "stdafx.h"
#include "Result.h"
#include "Title.h"

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

	//������
	InitCamera();

	return true;
}

void Result::GoTitle()
{
	//A�{�^���Ń^�C�g���֖߂�
	if (g_pad[0]->IsPress(enButtonA))
	{
		//�^�C�g���̍쐬
		NewGO<Title>(0, "title");

		//�폜����
		OnDestroy();
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