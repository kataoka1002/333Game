#pragma once

namespace nsK2EngineLow
{
	class ModelRender :public Noncopyable
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// ������
		/// </summary>
		void Init(
			const char* filePath,
			AnimationClip* animationClips = nullptr,
			int animationClipsNum = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReciever = true);

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// ���W�ݒ�
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		void SetPosition(const float x, const float y, const float z)
		{
			SetPosition({ x,y,z });
		}

		/// <summary>
		/// ��]�ݒ�
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// �T�C�Y�ݒ�
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(const float x, const float y, const float z)
		{
			SetScale({ x,y,z });
		}
		void SetScale(const float xyz)
		{
			SetScale({ xyz ,xyz ,xyz });
		}

		/// <summary>
		/// ���W�g���]��ݒ�
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="rotation"></param>
		/// <param name="scale"></param>
		void SetTRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		/// <summary>
		/// ���f�����擾
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			return m_model;
		}

		/// <summary>
		/// ���f���̍��W���擾
		/// </summary>
		/// <returns></returns>
		Vector3& GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// ���f����X���W���擾
		/// </summary>
		/// <returns></returns>
		float& GetPositionX()
		{
			return m_position.x;
		}

		/// <summary>
		/// ���f����Y���W���擾
		/// </summary>
		/// <returns></returns>
		float& GetPositionY()
		{
			return m_position.y;
		}

		/// <summary>
		/// ���f����Z���W���擾
		/// </summary>
		/// <returns></returns>
		float& GetPositionZ()
		{
			return m_position.z;
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ�</param>
		/// <param name="interpolateTime">�⊮����</param>
		void PlayAnimation(int animNum, float interpolateTime = 0.0f)
		{
			m_nowAnimationNumber = animNum;
			m_animation.Play(animNum, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�����i���o�[�̎擾
		/// </summary>
		/// <returns></returns>
		int GetAnimationNum() const
		{
			return m_nowAnimationNumber;
		}


	private:

		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);


	private:
		Model m_model;										//���f��
		Vector3 m_position = Vector3::Zero;					//�|�W�V����
		Vector3 m_scale = Vector3::One;						//�X�P�[��
		Quaternion m_rotation = { 0.0f,0.0f,0.0f,1.0f };	//��]
		Animation m_animation;								//�A�j���[�V����
		AnimationClip* m_animationClips;					//�A�j���[�V�����N���b�v
		Skeleton m_skeleton;								//�X�P���g��
		int m_numAnimationClips = 0;						//�A�j���[�V�����N���b�v�i���o�[
		int m_nowAnimationNumber = -1;						//���̃A�j���[�V�����N���b�v�i���o�[
		float m_animationSpeed = 1.0f;						//�A�j���[�V�����̑���

	};
}


