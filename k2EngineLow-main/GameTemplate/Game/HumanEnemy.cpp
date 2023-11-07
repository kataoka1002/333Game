#include "stdafx.h"
#include "HumanEnemy.h"
#include "Game.h"
#include "IHumanEnemyState.h"
#include "HumanEnemyIdleState.h"

namespace
{
	//�̗͂̍ő�l
	const float MAX_HP = 100.0f;
}

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	HumanEnemy::HumanEnemy()
	{
		//�A�j���[�V�����̏�����
		animationClips[enAnimClip_Idle].Load("Assets/animData/enemy/humanEnemy_Idle.tka");
		animationClips[enAnimClip_Idle].SetLoopFlag(true);
		animationClips[enAnimClip_Dead].Load("Assets/animData/enemy/humanEnemy_Dead.tka");
		animationClips[enAnimClip_Dead].SetLoopFlag(false);
		animationClips[enAnimClip_Crawl].Load("Assets/animData/enemy/humanEnemy_Crawl.tka");
		animationClips[enAnimClip_Crawl].SetLoopFlag(true);
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

		// �L�����N�^�[�R���g���[���[��������
		m_charaCon.Init(
			30.0f,			//���a
			150.0f,			//����
			m_position		//���W
		);

		// HP�̐ݒ�
		m_hp = MAX_HP;

		// �����X�e�[�g��ݒ�
		m_humanEnemyState = new HumanEnemyIdleState(this);
		m_humanEnemyState->Enter();
	}

	void HumanEnemy::Update()
	{
		// �X�e�[�g��ύX���邩
		IHumanEnemyState* enemyState = m_humanEnemyState->StateChange();

		// �ύX��̃X�e�[�g���ݒ肳��Ă���Ȃ��
		if (enemyState != nullptr)
		{
			// �X�e�[�g��ύX����B
			delete m_humanEnemyState;
			m_humanEnemyState = enemyState;
			m_humanEnemyState->Enter();
		}

		// �e�X�e�[�g�̍X�V���������s�B
		m_humanEnemyState->Update();

		// �A�j���[�V�������Đ�����B
		PlayAnimation(m_currentAnimationClip);

		// ���f�����X�V����B
		m_model.Update();
	}

	void HumanEnemy::OnDestroy()
	{
		//���X�g����폜
		m_game->RemoveEnemyFromList(this);

		//�폜
		DeleteGO(this);
	}

	void HumanEnemy::PlayAnimation(EnAnimationClip currentAnimationClip)
	{
		// �A�j���[�V�������Đ�
		m_model.PlayAnimation(currentAnimationClip, m_complementTime);
	}

	void HumanEnemy::Render(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}