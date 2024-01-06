#pragma once

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	class HumanEnemyRender : public IGameObject
	{
	public:
		enum EnAnimationClip
		{
			enAnimClip_Idle,
			enAnimClip_Dead,
			enAnimClip_Crawl,
			enAnimClip_Tremble,
			enAnimClip_Die,
			enAnimClip_Walk,
			enAnimClip_Attack,
			enAnimClip_Reaction,
			enAnimClip_Num
		};

		bool Start();
		void Update();
		void Render(RenderContext& rc);

		/// <summary>
		/// �ő吔��ݒ肷��B
		/// </summary>
		/// <param name="numHumanEnemy"></param>
		void SetMaxHumanEnemy(int maxHumanEnemy)
		{
			m_maxHumanEnemy = maxHumanEnemy;
		}
		/// <summary>
		/// �C���X�^���X�������B
		/// </summary>
		/// <param name="instanceNo"></param>
		void RemoveInstance(int instanceNo)
		{
			m_modelRender.RemoveInstance(instanceNo);
		}
		/// <summary>
		/// �C���X�^���V���O�f�[�^�̍X�V�B
		/// </summary>
		/// <param name="instanceNo">�C���X�^���X�̔ԍ�</param>
		/// <param name="pos"></param>
		/// <param name="rot"></param>
		/// <param name="scale"></param>
		void UpdateInstancingData(
			int instanceNo,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale)
		{
			m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
		}

	private:
		ModelRender			m_modelRender;								// ���f�������_���[
		int					m_maxHumanEnemy = 0;						// ���f���̐�
		AnimationClip		animationClips[enAnimClip_Num];				// �A�j���[�V�����N���b�v
		EnAnimationClip		m_currentAnimationClip = enAnimClip_Idle;	// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
		float				m_complementTime = 0.0f;					// �A�j���[�V�����̕⊮����
	};
}