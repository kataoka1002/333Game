#pragma once
#include "SpringCamera.h"
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();

private:
	Player* m_player = nullptr;				//�v���C���[
	Vector3	m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g��
	SpringCamera m_springCamera;			//�΂˃J����
};

