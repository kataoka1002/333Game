#pragma once
class Teapot;
class GameCamera;

class Player : public IGameObject
{
public:
	enum EnPlayerState
	{
		enPlayerState_Idle,		//�ҋ@��
		enPlayerState_Walk,		//������
		enPlayerState_Jump,		//�W�����v��
		enPlayerState_Attract,	//�����񂹒�
		enPlayerState_Standby,	//�ҋ@��(�I�u�W�F�N�g�������Ă��鎞)
		enPlayerState_Standwalk,//�\���ĕ���
		enPlayerState_Attack,	//�U��
		enPlayerState_WalkRight,//�E�ɕ���
		enPlayerState_WalkLeft,	//���ɕ���
		enPlayerState_WalkBack,	//���ɕ���
		enPlayerState_Num
	};

public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	void Turn();
	void Action();
	void PlayAnimation();
	void ManageState();

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	const EnPlayerState& GetPlayerState()const
	{
		return m_playerState;
	}

	const Vector3& GetPlayerSpeed()const
	{
		return m_moveSpeed;
	}

	const bool& GetIsHoldingObject()const
	{
		return m_isHoldingObject;
	}

private:
	enum EnAnimationClip 
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Jump,
		enAnimClip_Attract,
		enAnimClip_Standby,
		enAnimClip_Standwalk,
		enAnimClip_Attack,	
		enAnimClip_WalkRight,
		enAnimClip_WalkLeft,
		enAnimClip_WalkBack,
		enAnimClip_Num
	};

	AnimationClip		animationClips[enAnimClip_Num];			//�A�j���[�V�����N���b�v
	ModelRender			m_playerModel;							//�v���C���[���f��
	Vector3				m_position = Vector3::Zero;				//���W
	Vector3				m_startPosition = Vector3::Zero;		//�������W
	Vector3				m_moveSpeed = Vector3::Zero;			//�ړ����x
	Vector3             m_rotMove = Vector3::Zero;				//��]���x
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[
	Quaternion			m_rotation;								//�N�H�[�^�j�I��
	EnPlayerState		m_playerState = enPlayerState_Idle;		//�v���C���[�̃X�e�[�g(���)��\���ϐ�
	bool				m_isHoldingObject = false;				//�I�u�W�F�N�g�������Ă��邩�ǂ���
	float				LStick_x = 0.0f;						//L�X�e�B�b�N���͗�(��)
	float				LStick_y = 0.0f;						//L�X�e�B�b�N���͗�(�c)

	Teapot*				m_teapot = nullptr;
	GameCamera*			m_camera = nullptr;
};
