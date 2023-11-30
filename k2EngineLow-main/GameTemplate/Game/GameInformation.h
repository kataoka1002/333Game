#pragma once
class GameInformation : public IGameObject
{
public:
	~GameInformation();
	bool Start();
	void Update();

	/// <summary>
	/// �|�������𑝂₷
	/// </summary>
	void AddDefeatEnemyNum()
	{
		m_defeatEnemyNum++;
	}

	/// <summary>
	/// �{�X��ɍs�����Ƃ��o����
	/// </summary>
	void EnableBossBattle()
	{
		m_canGoBossBattle = true;
	}

	/// <summary>
	/// �{�X�풆�ɂ���
	/// </summary>
	void StartBossBattle()
	{
		m_isInBossBattle = true;
	}

	/// <summary>
	/// ���[�r�[�����ǂ������Z�b�g����
	/// </summary>
	/// <param name="now"></param>
	void SetInMovie(bool now)
	{
		m_isInMovie = now;
	}

	/// <summary>
	/// �{�X��ɍs�����Ƃ��ł��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetCanGoBossBattle() const
	{
		return m_canGoBossBattle;
	}

	/// <summary>
	/// �{�X�풆���ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsInBossBattle() const
	{
		return m_isInBossBattle;
	}

	/// <summary>
	/// ���[�r�[�����ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsInMovie() const
	{
		return m_isInMovie;
	}

private:
	int m_defeatEnemyNum = 0;			//�|�����G�̐�
	bool m_canGoBossBattle = false;		//�{�X��ɍs���邩�ǂ���
	bool m_isInBossBattle = false;		//�{�X�풆���ǂ���
	bool m_isInMovie = false;			//���[�r�[�����ǂ���
};