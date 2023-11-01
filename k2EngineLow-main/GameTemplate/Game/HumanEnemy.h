#pragma once
#include "EnemyBase.h"

class HumanEnemy : public EnemyBase
{
public:
	enum EnHumanEnemyState
	{
		enHumanEnemyState_Idle,		//�ҋ@��
		enHumanEnemyState_Num
	};

public:
	HumanEnemy();
	~HumanEnemy();

	void Update();								//�X�V����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitModel()				override;	//���f���̏�����
	void Render(RenderContext& rc)	override;	//�`�揈��

protected:
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Num
	};

	AnimationClip		animationClips[enAnimClip_Num];				// �A�j���[�V�����N���b�v
	EnHumanEnemyState	m_humanEnemyState = enHumanEnemyState_Idle;		// �v���C���[�̃X�e�[�g(���)��\���ϐ�
};

