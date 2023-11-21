#include "stdafx.h"
#include "Result.h"
#include "Title.h"

bool Result::Start()
{
	//�A�j���[�V�����̏�����
	animationClips[enAnimClip_Idle].Load("Assets/animData/player/player_idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);

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