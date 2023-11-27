#pragma once
#include "EnemyBase.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	//�O���錾
	class IBossState;
	class BossHitReactionState;
	class BossUI;

	class Boss : public EnemyBase
	{
	public:
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Dead,
			enAnimClip_Reaction,
			enAnimClip_Jump,
			enAnimClip_Magic,
			enAnimClip_Punch,
			enAnimClip_Swipe,
			enAnimClip_Walk,
			enAnimClip_Num
		};

	public:
		Boss();
		~Boss();

		void Update();								//�X�V����
		void PlayAnimation(EnAnimationClip currentAnimationClip);	//�A�j���[�V�����̍Đ�
		void SetCollision();						//�R���W�����̐ݒ�
		void MoveCollision();						//�R���W�����̓����̐ݒ�	
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);// �A�j���[�V�����C�x���g�p�̊֐�
		void SetReactionState();
		void FollowPlayer()				override;	//�����̏���
		void Turn()						override;	//��]����
		void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
		void InitModel()				override;	//���f���̏�����
		void PlayReaction() override;
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

		/// <summary>
		/// �̗͂̎擾
		/// </summary>
		/// <returns></returns>
		const float GetHP()const
		{
			return m_hp;
		}

		/// <summary>
		/// �v���C���[�܂ł̋����𑪂�
		/// </summary>
		/// <returns></returns>
		float CheckDistanceToPlayer()
		{
			//�v���C���[�܂ł̃x�N�g�������ߋ��������߂�
			Vector3 toPlayer = m_player->GetPosition() - m_position;
			float toPlayerLen = toPlayer.Length();
			return toPlayerLen;
		}

		/// <summary>
		/// HP�̊������擾
		/// </summary>
		/// <returns></returns>
		const float GetHPRatio()const
		{
			return m_hp / 100.0f;
		}

		/// <summary>
		/// �Q�[�����N���A�������ɌĂ΂��
		/// </summary>
		void GameClear();

	protected:

		AnimationClip		animationClips[enAnimClip_Num];			// �A�j���[�V�����N���b�v
		EnAnimationClip		m_currentAnimationClip;					// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
		IBossState*			m_bossState = nullptr;					// �X�e�[�g	
		float				m_complementTime = 0.0f;				// �A�j���[�V�����̕⊮����

		BossUI* m_ui = nullptr;
	};
}
