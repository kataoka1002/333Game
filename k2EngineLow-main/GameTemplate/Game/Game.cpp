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

	m_levelRender.Init("Assets/modelData/level/level.tkl",
		[&](LevelObjectData& objData)
		{
			//���O��human��������B
			if (objData.EqualObjectName(L"player") == true)
			{
				//�v���C���[�̃I�u�W�F�N�g���쐬����B
				m_player = NewGO<Player>(0, "player");
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
			return true;
		});
	
	//�X�J�C�L���[�u�̍쐬
	SetSkyCube();

	//�v���C���[�̍쐬
	//m_player = NewGO<Player>(0, "player");

	//���e�B�N���̍쐬
	m_reticle = NewGO<Reticle>(0, "reticle");

	//�w�i�X�e�[�W�̍쐬
	//m_bg = NewGO<BackGround>(0, "background");

	//�e�B�[�|�b�g�̍쐬
	m_teapot = NewGO<Teapot>(0, "teapot");

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
	m_skyCube->SetPosition({ 0.0f,100.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	// �����̌v�Z�̂��߂�IBL�e�N�X�`�����Z�b�g����B
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}

void Game::Render(RenderContext& rc)
{

}