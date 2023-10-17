#pragma once
class Teapot;

class Player : public IGameObject
{
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

	void SetStandbyState()
	{
		//�X�^���o�C�X�e�[�g�ɂ���
		m_playerState = enPlayerState_Standby;
	}

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

public:
	enum EnPlayerState
	{
		enPlayerState_Idle,		//�ҋ@��
		enPlayerState_Walk,		//������
		enPlayerState_Jump,		//�W�����v��
		enPlayerState_Attract,	//�����񂹒�
		enPlayerState_Standby,	//�ҋ@��(�I�u�W�F�N�g�������Ă��鎞)
		enPlayerState_Standwalk,//�\���ĕ���
		enPlayerState_Num
	};

private:
	enum EnAnimationClip 
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Jump,
		enAnimClip_Attract,
		enAnimClip_Standby,
		enAnimClip_Standwalk,
		enAnimClip_Num
	};

	AnimationClip		animationClips[enAnimClip_Num];			//�A�j���[�V�����N���b�v
	ModelRender			m_playerModel;							//�v���C���[���f��
	Vector3				m_position = Vector3::Zero;				//���W
	Vector3				m_startPosition = Vector3::Zero;		//�������W
	Vector3				m_moveSpeed = Vector3::Zero;			//�ړ����x
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[
	Quaternion			m_rotation;								//�N�H�[�^�j�I��
	EnPlayerState		m_playerState = enPlayerState_Idle;		//�v���C���[�̃X�e�[�g(���)��\���ϐ�
	bool				m_isHoldingObject = false;				//�I�u�W�F�N�g�������Ă��邩�ǂ���
	Vector3             m_rotMove = Vector3::Zero;

	Teapot* m_teapot = nullptr;
};
