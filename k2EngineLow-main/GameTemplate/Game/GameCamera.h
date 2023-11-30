#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }
class GameInformation;
namespace nsBoss { class Boss; }

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	void SetNearCamera(bool isNear);
	Vector3 CalcTargetPosition();
	void BossMovieProcess();

private:
	Vector3	m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g��
	SpringCamera m_springCamera;			//�΂˃J����
	bool m_isNearCamera = false;			//�ߋ����J�������ǂ���
	float m_bossMakeTime = 0.7f;

	nsPlayer::Player* m_player = nullptr;	//�v���C���[
	nsBoss::Boss* m_boss = nullptr;			//�{�X
	GameInformation* m_gameInfo = nullptr;
};

