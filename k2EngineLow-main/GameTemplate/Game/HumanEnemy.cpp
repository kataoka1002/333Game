#include "stdafx.h"
#include "HumanEnemy.h"

HumanEnemy::HumanEnemy()
{
	//�A�j���[�V�����̏�����
	animationClips[enAnimClip_Idle].Load("Assets/animData/enemy/humanEnemy_Idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
}

HumanEnemy::~HumanEnemy()
{

}

void HumanEnemy::InitModel()
{
	m_model.Init("Assets/modelData/enemy/humanEnemy.tkm", animationClips, enAnimClip_Num, enModelUpAxisZ);
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.Update();
}

void HumanEnemy::Update()
{
	switch (m_humanEnemyState)
	{
		//�v���C���[�X�e�[�g���ҋ@��������
	case enHumanEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ�
		m_model.PlayAnimation(enAnimClip_Idle, 0.5f);
		break;

	default:
		break;
	}
	m_model.Update();
}

void HumanEnemy::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
