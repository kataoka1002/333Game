#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }
namespace nsBoss { class Boss; }
class GameInformation;
class BlackFade;

class GameCamera : public IGameObject
{
public:
	enum enOPState
	{
		enOP_Camera1,
		enOP_Camera2,
		enOP_Player,
		enOP_Num
	};

public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	void SetNearCamera(bool isNear);
	Vector3 CalcTargetPosition();
	void BossMovieProcess();
	void OpeningProcess();
	void CalcOpeningFade(const enOPState& nextState, const Vector3& nextPos);
	void PlayVibration(int decline, int maxVib, bool& flag, int& vibration);
	void ManageVibration();
	void CalcChangeCamera();

	/// <summary>
	/// �h��Ă��邩�ǂ����̃t���O���Z�b�g����
	/// </summary>
	/// <param name="flag">true�F�h��Ă��� , false�F�h��Ă��Ȃ�</param>
	void SetVibFlag(bool flag)
	{
		m_vibFlag = flag;
	}

	/// <summary>
	/// �ߋ����J�������ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const bool GetIsNearCamera() const
	{
		return m_isNearCamera;
	}

private:
	enOPState m_opState = enOP_Camera1;		//�I�[�v�j���O�X�e�[�g
	Vector3	m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g��
	SpringCamera m_springCamera;			//�΂˃J����
	bool m_isNearCamera = false;			//�ߋ����J�������ǂ���
	float m_bossMakeTime = 0.0f;			//�{�X���o������܂ł̎���
	Vector3 m_opTarget = Vector3::Zero;		//OP�J�����̃^�[�Q�b�g
	float m_opTime = 0.0f;					//�I�[�v�j���O�p�̃^�C�}�[
	bool m_camera1End = false;				//�J�����P���I��������ǂ���
	int BGX = 0, BGY = 0;					//�ŏI�I�ȗh��̑傫��
	bool m_vibFlag = false;					//��ʂ��h��Ă��邩�ǂ���
	float m_cameraChangeTime = 0.5f;		//�ߋ������牓�����ɐ؂�ւ�鎞��
	bool m_isChangingCamera = false;		//�ߋ������牓�����ɐ؂�ւ�蒆���ǂ���

	nsPlayer::Player* m_player = nullptr;	//�v���C���[
	nsBoss::Boss* m_boss = nullptr;			//�{�X
	GameInformation* m_gameInfo = nullptr;
	BlackFade* m_blackFade = nullptr;
};

