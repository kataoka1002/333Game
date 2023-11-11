#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	
}

Game::~Game()
{
	//�폜
	DeleteGO(m_skyCube);
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_camera);
	DeleteGO(m_light);
	DeleteGO(m_teapot);
	DeleteGO(m_reticle);
}

bool Game::Start()
{
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_levelRender.Init("Assets/modelData/level/newLevel.tkl",
		[&](LevelObjectData& objData)
		{
			//���O��human��������B
			if (objData.EqualObjectName(L"player") == true)
			{
				//�v���C���[�̃I�u�W�F�N�g���쐬����B
				m_player = NewGO<nsPlayer::Player>(0, "player");
				//���W��ݒ肷��B
				m_player->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_player->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_player->SetRotation(objData.rotation);
				//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�܂��B
				return true;
			}
			//���O��background��������B
			else if (objData.EqualObjectName(L"background") == true)
			{
				//�w�i�I�u�W�F�N�g���쐬����B
				m_bg = NewGO<BackGround>(0, "background");
				//���W��ݒ肷��B
				m_bg->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_bg->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_bg->SetRotation(objData.rotation);
				return true;
			}
			//���O��background��������B
			else if (objData.EqualObjectName(L"house") == true)
			{
				//�w�i�I�u�W�F�N�g���쐬����B
				m_house = NewGO<House>(0, "house");
				//���W��ݒ肷��B
				m_house->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_house->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_house->SetRotation(objData.rotation);
				return true;
			}
			//���O��teapot��������B
			else if (objData.EqualObjectName(L"teapot") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_teapot = NewGO<Teapot>(0, "teapot");
				//���W��ݒ肷��B
				m_teapot->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_teapot->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_teapot->SetRotation(objData.rotation);
				//���X�g�ɒǉ�
				m_objectList.push_back(m_teapot);
				return true;
			}
			//���O��humanEnemy��������B
			else if (objData.EqualObjectName(L"humanEnemy") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_humanEnemy = NewGO<nsHumanEnemy::HumanEnemy>(0, "humanenemy");
				//���W��ݒ肷��B
				m_humanEnemy->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_humanEnemy->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_humanEnemy->SetRotation(objData.rotation);
				//���X�g�ɒǉ�
				m_enemyList.push_back(m_humanEnemy);
				return true;
			}
			return true;
		});
	
	//�X�J�C�L���[�u�̍쐬
	SetSkyCube();

	//���e�B�N���̍쐬
	m_reticle = NewGO<nsPlayer::Reticle>(0, "reticle");

	//�Q�[���J�����̍쐬
	m_camera = NewGO<GameCamera>(0, "gamecamera");

	//���C�g�̍쐬
	m_light = NewGO<Lighting>(0, "lighting");

	return true;
}

void Game::Update()
{
	
}

void Game::SetSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(1000.0f);
	m_skyCube->SetPosition(m_player->GetPosition());
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	// �����̌v�Z�̂��߂�IBL�e�N�X�`�����Z�b�g����B
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}

void Game::Render(RenderContext& rc)
{

}