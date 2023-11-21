#pragma once
class ResultCamera;

class Result : public IGameObject, Noncopyable
{
public:
	//�A�j���[�V�����N���b�v
	enum EnAnimationClip
	{
		enAnimClip_Idle,
		enAnimClip_Num
	};

public:
	Result() {}
	virtual ~Result() {}

	bool Start();
	void GoTitle();
	virtual void InitCamera() = 0;	//���f���̏�����
	virtual void OnDestroy() = 0;	//������Ƃ��ɌĂ΂�鏈��

	/// <summary>
	/// �v���C���[���f���̃|�W�V������Ԃ�
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPlayerPosition() const
	{
		return m_playerPosition;
	}

protected:
	AnimationClip		animationClips[enAnimClip_Num];				// �A�j���[�V�����N���b�v
	EnAnimationClip		m_currentAnimationClip = enAnimClip_Idle;	// ���ݐݒ肳��Ă���A�j���[�V�����N���b�v
	ModelRender			m_playerModel;								// �v���C���[���f��

	Vector3				m_playerPosition = Vector3::Zero;
	ResultCamera*		m_resultCamera = nullptr;
};

