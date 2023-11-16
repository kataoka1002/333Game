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
	DeleteGO(m_playerUI);
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
				////���W��ݒ肷��B
				//m_teapot->SetPosition(objData.position);
				////�傫����ݒ肷��B
				//m_teapot->SetScale(objData.scale);
				////��]��ݒ肷��B
				//m_teapot->SetRotation(objData.rotation);
				
				SetLevel(m_teapot, objData);
				
				//���X�g�ɒǉ�
				m_objectList.push_back(m_teapot);
				return true;
			}
			else if (objData.EqualObjectName(L"air") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_air = NewGO<Air>(0, "air");

				//SetLevel(m_air, objData);
				//���W��ݒ肷��B
				m_air->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_air->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_air->SetRotation(objData.rotation);

				//���X�g�ɒǉ�
				m_objectList.push_back(m_air);
				return true;
			}
			else if (objData.EqualObjectName(L"barrierFence") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_barrierFence = NewGO<BarrierFence>(0, "barrierfence");

				SetLevel(m_barrierFence, objData);

				//���X�g�ɒǉ�
				m_objectList.push_back(m_barrierFence);
				return true;
			}
			else if (objData.EqualObjectName(L"bench_big") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_benchBig = NewGO<BenchBig>(0, "benchbig");

				SetLevel(m_benchBig, objData);

				//���X�g�ɒǉ�
				m_objectList.push_back(m_benchBig);
				return true;
			}
			else if (objData.EqualObjectName(L"bench_small") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_benchSmall = NewGO<BenchSmall>(0, "benchsmall");

				SetLevel(m_benchSmall, objData);

				//���X�g�ɒǉ�
				m_objectList.push_back(m_benchSmall);
				return true;
			}
			else if (objData.EqualObjectName(L"billboard_small") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_billboardSmall = NewGO<BillboardSmall>(0, "billboardsmall");

			SetLevel(m_billboardSmall, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_billboardSmall);
			return true;
			}
			else if (objData.EqualObjectName(L"bush_big") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_bushBig = NewGO<BushBig>(0, "bushbig");

			SetLevel(m_bushBig, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_bushBig);
			return true;
			}
			else if (objData.EqualObjectName(L"cone") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_cone = NewGO<Cone>(0, "cone");

			SetLevel(m_cone, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_cone);
			return true;
			}
			else if (objData.EqualObjectName(L"dustbin") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_dustbin = NewGO<Dustbin>(0, "dustbin");

			SetLevel(m_dustbin, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_dustbin);
			return true;
			}
			else if (objData.EqualObjectName(L"fence") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_fence = NewGO<Fence>(0, "fence");

			SetLevel(m_fence, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_fence);
			return true;
			}
			else if (objData.EqualObjectName(L"hydrant") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_hydrant = NewGO<Hydrant>(0, "hydrant");

			SetLevel(m_hydrant, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_hydrant);
			return true;
			}
			else if (objData.EqualObjectName(L"plant_long") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_plantLong = NewGO<PlantLong>(0, "plantlong");

			SetLevel(m_plantLong, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_plantLong);
			return true;
			}
			else if (objData.EqualObjectName(L"plant_low") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_plantLow = NewGO<PlantLow>(0, "plantlow");

			SetLevel(m_plantLow, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_plantLow);
			return true;
			}
			else if (objData.EqualObjectName(L"rock_big") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_rockBig = NewGO<RockBig>(0, "rockbig");

			SetLevel(m_rockBig, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_rockBig);
			return true;
			}
			else if (objData.EqualObjectName(L"solarPanel") == true)
			{
			//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
			m_solarPanel = NewGO<SolarPanel>(0, "solarpanel");

			SetLevel(m_solarPanel, objData);

			//���X�g�ɒǉ�
			m_objectList.push_back(m_solarPanel);
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

	//UI�̍쐬
	m_playerUI = NewGO<nsPlayer::PlayerUI>(0, "playerui");

	//�Q�[���J�����̍쐬
	m_camera = NewGO<GameCamera>(0, "gamecamera");

	//���C�g�̍쐬
	m_light = NewGO<Lighting>(0, "lighting");

	return true;
}

template <typename T>
void Game::SetLevel(T* objct, LevelObjectData& objData)
{
	//���W��ݒ肷��B
	objct->SetPosition(objData.position);
	//�傫����ݒ肷��B
	objct->SetScale(objData.scale);
	//��]��ݒ肷��B
	objct->SetRotation(objData.rotation);
}

void Game::Update()
{
	
}

void Game::SetSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(2000.0f);
	m_skyCube->SetPosition({-5000.0f,0.0f,6000.0f});
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	// �����̌v�Z�̂��߂�IBL�e�N�X�`�����Z�b�g����B
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}

void Game::Render(RenderContext& rc)
{

}