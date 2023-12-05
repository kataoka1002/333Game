#include "stdafx.h"
#include "EnemyUI.h"
#include "Player.h"
#include "Game.h"

namespace
{
	//�X�v���C�g�̃T�C�Y(���F�摜�̃T�C�Y�A�E�F�傫���̔{��)
	const Vector2 HP_BLACK_SIZE = { 657.0f * 0.3f,29.0f * 0.5f };
	const Vector2 HP_ORANGE_SIZE = { 657.0f * 0.3f,29.0f * 0.5f };
	const Vector2 HP_RED_SIZE = { 657.0f * 0.3f,29.0f * 0.5f };
	const Vector2 HP_FRAME_SIZE = { 681.0f * 0.3f,53.0f * 0.5f };

	//HP�o�[�̃��[�J���|�W�V����
	const float HP_BER_POS_Y = 220.0f;
	const float HP_BER_POS_Y_CRAWL = 120.0f;

	//�I�����W����������
	const float ORANGE_MOVE_SPEED = 2.0f;
}

EnemyUI::EnemyUI()
{
	//HP�o�[�̏�����
	m_HPBlackSprite.Init("Assets/spriteData/UI/enemy/EnemyHPBlack.dds", HP_BLACK_SIZE.x, HP_BLACK_SIZE.y);
	m_HPBlackSprite.SetScale(m_redScale);
	m_HPOrangeSprite.Init("Assets/spriteData/UI/enemy/EnemyHPOrange.dds", HP_ORANGE_SIZE.x, HP_ORANGE_SIZE.y);
	m_HPOrangeSprite.SetScale(m_orangeScale);
	m_HPRedSprite.Init("Assets/spriteData/UI/enemy/EnemyHPRed.dds", HP_RED_SIZE.x, HP_RED_SIZE.y);
	m_HPRedSprite.SetScale(m_redScale);

	//HP�t���[���̏�����
	m_HPFrameSprite.Init("Assets/spriteData/UI/enemy/enemyHPFrame.dds", HP_FRAME_SIZE.x, HP_FRAME_SIZE.y);
	m_HPFrameSprite.SetScale(m_redScale);
}

EnemyUI::~EnemyUI()
{

}

bool EnemyUI::Start()
{
	//�N���X��������
	m_game = FindGO<Game>("game");
	m_player = FindGO<nsPlayer::Player>("player");

	return true;
}

void EnemyUI::Update()
{
	//�t���[���̏���
	CalcFrame();

	//�Ԃ̏���
	CalcRed();

	//�I�����W�̏���
	CalcOrange();
}

void EnemyUI::CalcScale(Vector3& scale)
{
	//�e�̗̑͂̊�������UI�̃T�C�Y���v�Z���Ă���B
	scale.x = m_enemy->GetHP() * (1.0f / m_enemy->GetMaxHP());

	//�T�C�Y��0��菬�����Ȃ�Ȃ�
	if (scale.x <= 0.0f)
	{
		scale.x = 0.0f;
	}
}

void EnemyUI::CalcPosition(Vector2& position, Vector2 spriteSize, bool useLerp, bool isFrame)
{
	//�|�W�V�����̎擾
	Vector3 pos = m_enemy->GetPosition();

	//�G�l�~�[�̏�̕��ɉ摜��\���������̂�,y���W�������傫������B
	//�N���[�����Ȃ�
	if (m_enemy->IsCrawling())
	{
		pos.y += HP_BER_POS_Y_CRAWL;
	}
	//�񑫕��s���Ȃ�
	else
	{
		pos.y += HP_BER_POS_Y;
	}

	//���[���h���W����X�N���[�����W���v�Z(�v�Z���ʂ�position�ɑ�������)
	g_camera3D->CalcScreenPositionFromWorldPosition(position, pos);

	//�摜���t���[���Ȃ�Ԃ�
	if (isFrame == true)
	{
		return;
	}

	//�摜�����Ɋ񂹂��
	Vector3 BerSizeSubtraction = SendHPBer(spriteSize, m_redScale);

	if (useLerp == true)
	{
		//���߂��ړ��ʂ܂ŕ�Ԃŋ߂Â���
		m_orangeOffset = Math::Lerp(g_gameTime->GetFrameDeltaTime() * ORANGE_MOVE_SPEED, m_orangeOffset, BerSizeSubtraction);
		position.x -= m_orangeOffset.x;
	}
	else
	{
		//�ړ��ʂ̕��񂹂�
		position.x -= BerSizeSubtraction.x;
	}
}

void EnemyUI::CalcFrame()
{
	//�|�W�V�����̌v�Z
	CalcPosition(m_framePosition, Vector2::Zero, false, true);

	//�t���[���Ɣw�i���Z�b�g����
	m_HPFrameSprite.SetPosition(Vector3(m_framePosition.x, m_framePosition.y, 0.0f));
	m_HPFrameSprite.Update();
	m_HPBlackSprite.SetPosition(Vector3(m_framePosition.x, m_framePosition.y, 0.0f));
	m_HPBlackSprite.Update();
}

void EnemyUI::CalcRed()
{
	//�X�P�[���̏���
	CalcScale(m_redScale);

	//�Z�b�g����
	m_HPRedSprite.SetScale(m_redScale);
	m_HPRedSprite.Update();


	//�|�W�V�����̏���
	CalcPosition(m_redPosition, HP_RED_SIZE);

	//HP�o�[���Z�b�g����
	m_HPRedSprite.SetPosition(Vector3(m_redPosition.x, m_redPosition.y, 0.0f));
	m_HPRedSprite.Update();
}

void EnemyUI::CalcOrange()
{
	//�X�P�[���̏���
	CalcScale(m_orangeScale);

	//�I�����W�̃o�[���Ԃ̌��ǂ�������
	m_orangeScaleNew = Math::Lerp(g_gameTime->GetFrameDeltaTime() * ORANGE_MOVE_SPEED, m_orangeScaleNew, m_orangeScale);

	//�Z�b�g����
	m_HPOrangeSprite.SetScale(m_orangeScaleNew);
	m_HPOrangeSprite.Update();


	//�|�W�V�����̏���
	CalcPosition(m_orangePosition, HP_ORANGE_SIZE, true);

	//HP�o�[���Z�b�g����
	m_HPOrangeSprite.SetPosition({ m_orangePosition.x,m_orangePosition.y ,0.0f });
	m_HPOrangeSprite.Update();
}

Vector3 EnemyUI::SendHPBer(Vector2 size, Vector3 scale)
{
	Vector3 BerSize = { size.x,size.y ,1.0f };				//�摜�̌��̑傫��
	Vector3 changedBerSize = Vector3::Zero;					//�摜���X�P�[���ϊ��������Ƃ̑傫��
	Vector3 BerSizeSubtraction = Vector3::Zero;				//���摜�ƕϊ���摜�̃X�P�[����

	changedBerSize.x = BerSize.x * scale.x;
	BerSizeSubtraction.x = BerSize.x - changedBerSize.x;
	BerSizeSubtraction.x *= 0.5f;	//�T�C�Y�����ʂ�2�Ŋ��邱�Ƃ�,�ړ������鋗�������߂�

	return BerSizeSubtraction;
}

bool EnemyUI::CalcAngle(EnemyBase* enemy)
{
	//�J��������G�l�~�[�̈ʒu�ւ̃x�N�g�������߂�
	Vector3 toEnemy = enemy->GetPosition() - g_camera3D->GetPosition();
	toEnemy.Normalize();

	//�J�����̑O�����ƃJ��������G�l�~�[�ւ̃x�N�g���̓��ς����߂�
	float angle = g_camera3D->GetForward().Dot(toEnemy);

	//���ς̌��ʂ���p�x�����߂�
	angle = acos(angle);

	//�J�������猩�ăG�l�~�[�����p�x�ȓ��̂Ƃ�
	if (fabsf(angle) <= Math::DegToRad(50.0f))
	{
		//�v���C���[�ƃG�l�~�[�̋��������߂�
		Vector3 diff = enemy->GetPosition() - m_player->GetPosition();

		//��苗���ȓ��Ȃ�
		if (diff.Length() <= 1000.0f)
		{
			//�`�悷��
			return true;
		}
	}

	//�`�悵�Ȃ�
	return false;
}

void EnemyUI::Render(RenderContext& rc)
{
	//�`�悷�邩���Ȃ����̏���
	if (CalcAngle(m_enemy) == false)
	{
		return;
	}

	//�`��
	m_HPBlackSprite.Draw(rc);
	m_HPOrangeSprite.Draw(rc);
	m_HPRedSprite.Draw(rc);
	m_HPFrameSprite.Draw(rc);
}