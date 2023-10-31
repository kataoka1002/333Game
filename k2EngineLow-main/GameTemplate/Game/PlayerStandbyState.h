#pragma once
#include "IPlayerState.h"

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	class Player;

	/// <summary>
	/// �v���C���[�̍U���X�^���o�C���X�e�[�g
	/// </summary>
	class PlayerStandbyState : public IPlayerState
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		/// <param name="player">�v���C���[�̃C���X�^���X�B</param>
		PlayerStandbyState(Player* player) : IPlayerState(player) {}
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~PlayerStandbyState() override;
		/// <summary>
		/// �X�e�[�g�J�n���̏����B
		/// </summary>
		void Enter() override;
		/// <summary>
		/// �X�e�[�g�̑J�ڏ����B
		/// </summary>
		/// <returns>�J�ڂ���X�e�[�g</returns>
		IPlayerState* StateChange() override;
		/// <summary>
		/// �X�e�[�g�ɂ�����X�V�����B
		/// </summary>
		void Update() override;
	};
}
