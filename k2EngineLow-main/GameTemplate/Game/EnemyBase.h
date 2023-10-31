#pragma once
class EnemyBase : public IGameObject, Noncopyable
{
public:
	//�R���X�g���N�^
	EnemyBase() {}
	//�f�X�g���N�^
	virtual ~EnemyBase() {}

	bool Start();
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

protected:
	ModelRender				m_model;
	Vector3					m_position = Vector3::Zero;					//�|�W�V����
	Vector3					m_scale = Vector3::One;						//�傫��
	Quaternion				m_rotation;									//�N�H�[�^�j�I��
};

