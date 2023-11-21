#pragma once
// �O���錾�B
namespace nsPlayer { class Player; }
class Game;
class Boss;

class ObjectBase : public IGameObject, Noncopyable
{
public:

	//�R���X�g���N�^
	ObjectBase() {}
	//�f�X�g���N�^�̓o�[�`�����ɂ��Ă���
	virtual ~ObjectBase() {}
	bool Start();
	/// <summary>
	/// �����񂹏����̏�����
	/// </summary>
	void InitAttract();

	/// <summary>
	/// �Ԃ���я����̏�����
	/// </summary>
	void InitBlowAway();

	virtual void Render(RenderContext& rc)	= 0;	//�`�揈��
	virtual void InitModel()				= 0;	//���f���̏�����
	virtual void OnDestroy()				= 0;	//������Ƃ��ɌĂ΂�鏈��
	virtual void InitCollision()			= 0;	//�R���W�����̏�����

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
	/// �ړI�n�̐ݒ�
	/// </summary>
	/// <param name="pos"></param>
	void SetTargetPos(const Vector3 pos)
	{
		 m_targetPosition = pos;
	}

	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	/// <param name="vel"></param>
	void SetVelocity(Vector3 vel)
	{
		m_velocity = vel;
	}

	/// <summary>
	/// �����񂹂�邩�ǂ����̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetCanAttract() const
	{
		return m_canAttract;
	}
	
protected:

	/// <summary>
	/// �v���C���[�܂ň����񂹂��鏈��
	/// </summary>
	void AttractedToPlayer();

	/// <summary>
	/// �����񂹒��̑��x�v�Z
	/// </summary>
	/// <param name="speed"></param>
	/// <param name="curvatureRadius"></param>
	/// <param name="damping"></param>
	void CalcVelocity(const float speed, const float curvatureRadius, const float damping);

	/// <summary>
	/// �����̏���
	/// </summary>
	void Move();

	/// <summary>
	/// �A�C�h�����̏���
	/// </summary>
	void IdleMove();

	/// <summary>
	/// �ǔ����̏���
	/// </summary>
	void FollowPlayer();

	/// <summary>
	/// �t���t������
	/// </summary>
	void UpDown();

	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="speed"></param>
	void Turn(Vector3 speed);

	/// <summary>
	/// �A�C�h�����̖ړI�n�̌v�Z
	/// </summary>
	void CalcTargetPosition();

	/// <summary>
	/// ������ԏ���
	/// </summary>
	void BlowAway();

	/// <summary>
	/// ���ɓ������Ă��邩�̏���
	/// </summary>
	void CalcCollision();

	/// <summary>
	/// ���e�B�N���ɍ��킳���Ă��邩�̏���
	/// </summary>
	void CalcAimingDirection();

protected:
	enum EnObjectState
	{
		enObjectState_Quiescence,	//�������
		enObjectState_Idle,			//�ҋ@��
		enObjectState_Attract,		//�����񂹒�
		enObjectState_Blow,
		enObjectState_Num
	};

	ModelRender				m_model;									//���f��
	CharacterController		m_charaCon;									//�L�����N�^�[�R���g���[���[
	SphereCollider			m_sphereCollider;							//�Օ����m�F�p�̃R���C�_�[
	Vector3					m_position = Vector3::Zero;					//�|�W�V����
	Vector3					m_targetPosition = Vector3::Zero;			//�ړI�n
	Vector3					m_velocity = Vector3::Zero;					//���x
	Vector3					m_rotMove = Vector3::Zero;					//��]���x
	Vector3					m_flightSpeed = Vector3::Zero;				//���ł�����
	Vector3					m_followSpeed = Vector3::Zero;				//�ǔ��̑���
	Vector3					m_scale = Vector3::Zero;					//�傫��
	Vector3					m_collisionPosition = Vector3::Zero;		//�R���W�����I�u�W�F�N�g�̍��W
	Quaternion				m_rotation;									//�N�H�[�^�j�I��
	EnObjectState			m_objectState = enObjectState_Quiescence;	//�I�u�W�F�N�g�̃X�e�[�g(���)��\���ϐ�
	float					m_degree = 0.0f;							//�t���t���p�̊p�x
	float					m_damage = 0.0f;							//�^����_���[�W
	float					m_collisionAddPos = 0.0f;					//�R���W�����I�u�W�F�N�g��Y�l���Z��
	bool					m_canAttract = false;						//�����񂹂�邩�ǂ���

	nsPlayer::Player*		m_player = nullptr;
	Game*					m_game = nullptr;
	CollisionObject*		m_collisionObject = nullptr;
};

