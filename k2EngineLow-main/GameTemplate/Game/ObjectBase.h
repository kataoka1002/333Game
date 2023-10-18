#pragma once
class Player;

class ObjectBase : public IGameObject, Noncopyable
{
public:

	//�R���X�g���N�^
	ObjectBase() {}
	//�f�X�g���N�^�̓o�[�`�����ɂ��Ă���
	virtual ~ObjectBase() {}

	bool Start();
	void AttractedToPlayer();						//�v���C���[�܂ň����񂹂��鏈��
	void CalcVelocity(const float speed, const float curvatureRadius, const float damping);
	void InitAttract();
	void Move();
	void IdleMove();
	void FollowPlayer();
	void UpDown();
	void Turn(Vector3 speed);
	void CalcTargetPosition();
	void InitBlowAway();
	void BlowAway();

	virtual void Render(RenderContext& rc)	= 0;	//�`�揈��
	virtual void InitModel()				= 0;	//���f���̏�����

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
	Vector3					m_position = Vector3::Zero;					//�|�W�V����
	Vector3					m_targetPosition = Vector3::Zero;			//�ړI�n
	Vector3					m_velocity = Vector3::Zero;					//���x
	Vector3					m_rotMove = Vector3::Zero;
	Vector3					m_flightSpeed = Vector3::Zero;
	Quaternion				m_rotation;									//�N�H�[�^�j�I��
	EnObjectState			m_objectState = enObjectState_Quiescence;	//�I�u�W�F�N�g�̃X�e�[�g(���)��\���ϐ�
	float degree = 0.0f;

	Vector3 m_followSpeed = Vector3::Zero;

	Player* m_player = nullptr;
};

