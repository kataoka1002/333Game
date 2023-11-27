#pragma once
#include "IBossState.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	//�O���錾
	class Boss;

	/// <summary>
	/// �{�X�̃X���C�v�X�e�[�g�N���X
	/// </summary>
	class BossSwipeState : public IBossState
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		/// <param name="player">�G�l�~�[�̃C���X�^���X�B</param>
		BossSwipeState(Boss* boss) : IBossState(boss) {}
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~BossSwipeState() override;
		/// <summary>
		/// �X�e�[�g�J�n���̏����B
		/// </summary>
		void Enter() override;
		/// <summary>
		/// �X�e�[�g�̑J�ڏ����B
		/// </summary>
		/// <returns>�J�ڂ���X�e�[�g</returns>
		IBossState* StateChange() override;
		/// <summary>
		/// �X�e�[�g�ɂ�����X�V�����B
		/// </summary>
		void Update() override;
	};
}