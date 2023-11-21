#pragma once
#include "Player.h"
class Game;

class EnemyBase : public IGameObject, Noncopyable
{
public:
	//�R���X�g���N�^
	EnemyBase() {}
	//�f�X�g���N�^
	virtual ~EnemyBase() {}

	bool Start();
	void HandleDamageEvent(float damage);			//�_���[�W���󂯂����̏���
	bool DidAttackHit();
	virtual void FollowPlayer()				= 0;	//�ړ�����
	virtual void Turn()						= 0;	//��]����
	virtual void OnDestroy()				= 0;	//������Ƃ��ɌĂ΂�鏈��
	virtual void InitModel()				= 0;	//���f���̏�����
	virtual void Render(RenderContext& rc)	= 0;	//�`�揈��

	/// <summary>
	/// �|�W�V�����̐ݒ�
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// �L�����R����Ԃ�
	/// </summary>
	/// <returns></returns>
	CharacterController& GetCharaCon()
	{
		return m_charaCon;
	}

	/// <summary>
	/// �R���W������Ԃ�
	/// </summary>
	/// <returns></returns>
	CollisionObject* GetCollision()
	{
		return m_collisionObject;
	}

protected:
	ModelRender				m_model;
	Vector3					m_position = Vector3::Zero;				// �|�W�V����
	Vector3					m_scale = Vector3::One;					// �傫��
	Quaternion				m_rotation;								// �N�H�[�^�j�I��
	CharacterController		m_charaCon;								// �L�����N�^�[�R���g���[���[
	float					m_hp = 0.0f;							// HP
	Vector3					m_forward = {0.0f,0.0f,1.0f};			// �O����
	Vector3					m_moveSpeed = Vector3::Zero;			// �ړ����x
	Vector3					m_rotMove = Vector3::Zero;				// ��]���x

	Game*					m_game = nullptr;
	nsPlayer::Player*		m_player = nullptr;
	CollisionObject*		m_collisionObject = nullptr;			// �R���W����
};

