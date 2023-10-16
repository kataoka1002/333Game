#pragma once

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayAnimation();
	void Move();
	void Turn();
	void ManageState();

	const Vector3& GetPosition() const
	{
		return m_position;
	}

public:
	enum EnPlayerState
	{
		enPlayerState_Idle,		//�ҋ@��
		enPlayerState_Walk,		//������
		enPlayerState_Jump,		//�W�����v��
		enPlayerState_Num
	};

private:
	enum EnAnimationClip 
	{
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Num,
	};

	AnimationClip		animationClips[enAnimClip_Num];			//�A�j���[�V�����N���b�v
	ModelRender			m_playerModel;							//�v���C���[���f��
	Vector3				m_position = Vector3::Zero;				//���W
	Vector3				m_startPosition = Vector3::Zero;		//�������W
	Vector3				m_moveSpeed = Vector3::Zero;			//�ړ����x
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[
	Quaternion			m_rotation;								//�N�H�[�^�j�I��
	EnPlayerState		m_playerState = enPlayerState_Idle;		//�v���C���[�̃X�e�[�g(���)��\���ϐ�
};
