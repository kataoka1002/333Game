#pragma once
#include "EnemyBase.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	//�O���錾
	class IHumanEnemyState;

	class HumanEnemy : public EnemyBase
	{
	public:
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Dead,
			enAnimClip_Crawl,
			enAnimClip_Tremble,
			enAnimClip_Num
		};

	public:
		HumanEnemy();
		~HumanEnemy();

		void Update();								//�X�V����
		void PlayAnimation(EnAnimationClip currentAnimationClip);
		void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
		void InitModel()				override;	//���f���̏�����
		void Render(RenderContext& rc)	override;	//�`�揈��

		/// <summary>
		/// �Đ�����A�j���[�V�����N���b�v��ݒ肷��B
		/// </summary>
		/// <param name="enAnimationClip">�A�j���[�V�����N���b�v</param>
		/// <param name="complementTime">��Ԏ���</param>
		void SetAnimation(const EnAnimationClip enAnimationClip, const float complementTime)
		{
			m_currentAnimationClip = enAnimationClip;
			m_complementTime = complementTime;
		}

		/// <summary>
		/// �L�����R���̍Đݒ�
		/// </summary>
		void ResetCharaCon()
		{
			m_charaCon.RemoveRigidBoby();
			m_charaCon.Init(
				30.0f,			//���a
				10.0f,			//����
				m_position		//���W
			);
		}

		/// <summary>
		/// �A�j���[�V�����Đ������ǂ����̎擾
		/// </summary>
		/// <returns></returns>
		const bool& GetIsPlayingAnimation() const
		{
			return m_model.IsPlayingAnimation();
		}

		const float GetHP()const
		{
			return m_hp;
		}

	protected:

		AnimationClip		animationClips[enAnimClip_Num];			// �A�j���[�V�����N���b�v
		EnAnimationClip		m_currentAnimationClip;					// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
		IHumanEnemyState*	m_humanEnemyState = nullptr;			// �X�e�[�g	
		float				m_complementTime = 0.0f;				// �A�j���[�V�����̕⊮����

	};
}
