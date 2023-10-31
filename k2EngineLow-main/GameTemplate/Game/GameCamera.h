#pragma once
#include "SpringCamera.h"
namespace nsPlayer { class Player; }

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

private:
	nsPlayer::Player* m_player = nullptr;				//�v���C���[
	Vector3	m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g��
	SpringCamera m_springCamera;			//�΂˃J����

	bool m_isNearCamera = false;			//�ߋ����J�������ǂ���
};

