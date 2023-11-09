#pragma once
#include "HumanEnemy.h"
#include "HumanEnemyConstants.h"

/// <summary>
/// �q���[�}���G�l�~�[�̖��O���
/// </summary>
namespace nsHumanEnemy
{
	//�O���錾
	class HumanEnemy;

	/// <summary>
	/// �v���C���[�X�e�[�g���N���X
	/// </summary>
	class IHumanEnemyState
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="enemy">�G�l�~�[</param>
		IHumanEnemyState(HumanEnemy* enemy) : m_enemy(enemy) {}

		/// <summary>
		/// �f�X�X�g���N�^
		/// </summary>
		virtual ~IHumanEnemyState() {}

		/// <summary>
		/// �X�e�[�g�J�n���̏����B
		/// </summary>
		virtual void Enter() = 0;

		/// <summary>
		/// �X�e�[�g�̑J�ڏ����B
		/// </summary>
		/// <returns>�J�ڂ���X�e�[�g</returns>
		virtual IHumanEnemyState* StateChange() = 0;

		/// <summary>
		/// �e�X�e�[�g�ɂ�����X�V�����B
		/// </summary>
		virtual void Update() = 0;

	protected:
		HumanEnemy* m_enemy = nullptr;		// �G�l�~�[
	};

}