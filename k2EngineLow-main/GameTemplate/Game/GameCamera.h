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

private:
	enOPState m_opState = enOP_Camera1;		//�I�[�v�j���O�X�e�[�g
	Vector3	m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g��
	SpringCamera m_springCamera;			//�΂˃J����
	bool m_isNearCamera = false;			//�ߋ����J�������ǂ���
	float m_bossMakeTime = 0.7f;			//�{�X���o������܂ł̎���
	Vector3 m_opTarget = Vector3::Zero;		//OP�J�����̃^�[�Q�b�g
	float m_opTime = 0.0f;					//�I�[�v�j���O�p�̃^�C�}�[
	bool m_camera1End = false;				//�J�����P���I��������ǂ���

	nsPlayer::Player* m_player = nullptr;	//�v���C���[
	nsBoss::Boss* m_boss = nullptr;			//�{�X
	GameInformation* m_gameInfo = nullptr;
	BlackFade* m_blackFade = nullptr;
};

