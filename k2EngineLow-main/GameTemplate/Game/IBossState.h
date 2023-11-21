#pragma once
#include "Boss.h"
#include "BossConstants.h"

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	//�O���錾
	class Boss;

	/// <summary>
	/// �{�X�X�e�[�g���N���X
	/// </summary>
	class IBossState
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="enemy">�{�X</param>
		IBossState(Boss* boss) : m_boss(boss) {}

		/// <summary>
		/// �f�X�X�g���N�^
		/// </summary>
		virtual ~IBossState() {}

		/// <summary>
		/// �X�e�[�g�J�n���̏����B
		/// </summary>
		virtual void Enter() = 0;

		/// <summary>
		/// �X�e�[�g�̑J�ڏ����B
		/// </summary>
		/// <returns>�J�ڂ���X�e�[�g</returns>
		virtual IBossState* StateChange() = 0;

		/// <summary>
		/// �e�X�e�[�g�ɂ�����X�V�����B
		/// </summary>
		virtual void Update() = 0;

	protected:
		Boss* m_boss = nullptr;		// �{�X
	};

}