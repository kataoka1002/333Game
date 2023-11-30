#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }
class GameInformation;
namespace nsBoss { class Boss; }
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

private:
	enOPState m_opState = enOP_Camera1;
	Vector3	m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g��
	SpringCamera m_springCamera;			//�΂˃J����
	bool m_isNearCamera = false;			//�ߋ����J�������ǂ���
	float m_bossMakeTime = 0.7f;
	Vector3 m_opTarget = Vector3::Zero;
	float m_opTime = 0.0f;
	bool m_camera1End = false;

	nsPlayer::Player* m_player = nullptr;	//�v���C���[
	nsBoss::Boss* m_boss = nullptr;			//�{�X
	GameInformation* m_gameInfo = nullptr;
	BlackFade* m_blackFade = nullptr;
};

