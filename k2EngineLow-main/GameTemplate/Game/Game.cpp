#include "stdafx.h"
#include "Game.h"
#include "GameClear.h"
#include "GameOver.h"
#include "Fade.h"

Game::Game()
{
	
}

Game::~Game()
{
	//エネミーをすべて削除
	for (auto enemy : m_enemyList)
	{
		//削除
		DeleteGO(enemy);
	}
	//リストを空にする
	m_enemyList.clear();

	//オブジェクトを全て削除
	for (auto object : m_objectList)
	{
		//削除
		DeleteGO(object);
	}
	//リストを空にする
	m_objectList.clear();


	//削除
	//DeleteGO(m_skyCube);	//ResultUIでデリートしている
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_camera);
	DeleteGO(m_light);
	DeleteGO(m_boss);
	DeleteGO(m_reticle);
	DeleteGO(m_playerUI);
	DeleteGO(m_wall);
	//DeleteGO(m_gameInfo); //ResultUIでデリートしている
	DeleteGO(m_blackFade);
	DeleteGO(m_missionUI);
	DeleteGO(m_makeEfe);
}

bool Game::Start()
{
	Fade* m_fade = FindGO<Fade>("fade");
	m_fade->SetAlphaDown();

	//エフェクトの初期化
	m_makeEfe = NewGO<MakeEffect>(0, "makeeffect");

	//ゲーム情報クラスの作成
	m_gameInfo = NewGO<GameInformation>(0, "gameinformation");

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_levelRender.Init("Assets/modelData/level/newLevel.tkl",
		[&](LevelObjectData& objData)
		{
			//名前がhumanだったら。
			if (objData.EqualObjectName(L"player") == true)
			{
				//プレイヤーのオブジェクトを作成する。
				m_player = NewGO<nsPlayer::Player>(0, "player");
				//座標を設定する。
				m_player->SetPosition(objData.position);
				//大きさを設定する。
				m_player->SetScale(objData.scale);
				//回転を設定する。
				m_player->SetRotation(objData.rotation);
				//trueにすると、レベルの方でモデルが読み込まれます。
				return true;
			}
			//名前がbackgroundだったら。
			else if (objData.EqualObjectName(L"background") == true)
			{
				//背景オブジェクトを作成する。
				m_bg = NewGO<BackGround>(0, "background");
				//座標を設定する。
				m_bg->SetPosition(objData.position);
				//大きさを設定する。
				m_bg->SetScale(objData.scale);
				//回転を設定する。
				m_bg->SetRotation(objData.rotation);
				return true;
			}
			//名前がbossだったら。
			else if (objData.EqualObjectName(L"boss") == true)
			{
				//ボスオブジェクトを作成する。
				m_boss = NewGO<nsBoss::Boss>(0, "boss");
				//座標を設定する。
				m_boss->SetPosition(objData.position);
				//大きさを設定する。
				m_boss->SetScale(objData.scale);
				//回転を設定する。
				m_boss->SetRotation(objData.rotation);
				return true;
			}
			//名前がteapotだったら。
			else if (objData.EqualObjectName(L"teapot") == true)
			{
				//ティーポットオブジェクトを作成する。
				m_teapot = NewGO<Teapot>(0, "teapot");

				SetLevel(m_teapot, objData);

				//リストに追加
				m_objectList.emplace_back(m_teapot);
				return true;
			}
			else if (objData.EqualObjectName(L"air") == true)
			{
				//換気扇オブジェクトを作成する。
				m_air = NewGO<Air>(0, "air");

				SetLevel(m_air, objData);

				//リストに追加
				m_objectList.emplace_back(m_air);
				return true;
			}
			else if (objData.EqualObjectName(L"barrierFence") == true)
			{
				//工事の柵オブジェクトを作成する。
				m_barrierFence = NewGO<BarrierFence>(0, "barrierfence");

				SetLevel(m_barrierFence, objData);

				//リストに追加
				m_objectList.emplace_back(m_barrierFence);
				return true;
			}
			else if (objData.EqualObjectName(L"bench_big") == true)
			{
				//でかいベンチオブジェクトを作成する。
				m_benchBig = NewGO<BenchBig>(0, "benchbig");

				SetLevel(m_benchBig, objData);

				//リストに追加
				m_objectList.emplace_back(m_benchBig);
				return true;
			}
			else if (objData.EqualObjectName(L"bench_small") == true)
			{
				//小さいベンチオブジェクトを作成する。
				m_benchSmall = NewGO<BenchSmall>(0, "benchsmall");

				SetLevel(m_benchSmall, objData);

				//リストに追加
				m_objectList.emplace_back(m_benchSmall);
				return true;
			}
			else if (objData.EqualObjectName(L"billboard_small") == true)
			{
				//看板オブジェクトを作成する。
				m_billboardSmall = NewGO<BillboardSmall>(0, "billboardsmall");

				SetLevel(m_billboardSmall, objData);

				//リストに追加
				m_objectList.emplace_back(m_billboardSmall);
				return true;
			}
			else if (objData.EqualObjectName(L"bush_big") == true)
			{
				//草オブジェクトを作成する。
				m_bushBig = NewGO<BushBig>(0, "bushbig");

				SetLevel(m_bushBig, objData);

				//リストに追加
				m_objectList.emplace_back(m_bushBig);
				return true;
			}
			else if (objData.EqualObjectName(L"cone") == true)
			{
				//三角コーンオブジェクトを作成する。
				m_cone = NewGO<Cone>(0, "cone");

				SetLevel(m_cone, objData);

				//リストに追加
				m_objectList.emplace_back(m_cone);
				return true;
			}
			else if (objData.EqualObjectName(L"dustbin") == true)
			{
				//ゴミ箱オブジェクトを作成する。
				m_dustbin = NewGO<Dustbin>(0, "dustbin");

				SetLevel(m_dustbin, objData);

				//リストに追加
				m_objectList.emplace_back(m_dustbin);
				return true;
			}
			else if (objData.EqualObjectName(L"fence") == true)
			{
				//フェンスオブジェクトを作成する。
				m_fence = NewGO<Fence>(0, "fence");

				SetLevel(m_fence, objData);

				//リストに追加
				m_objectList.emplace_back(m_fence);
				return true;
			}
			else if (objData.EqualObjectName(L"hydrant") == true)
			{
				//消火栓オブジェクトを作成する。
				m_hydrant = NewGO<Hydrant>(0, "hydrant");

				SetLevel(m_hydrant, objData);

				//リストに追加
				m_objectList.emplace_back(m_hydrant);
				return true;
			}
			else if (objData.EqualObjectName(L"plant_long") == true)
			{
				//大花瓶オブジェクトを作成する。
				m_plantLong = NewGO<PlantLong>(0, "plantlong");

				SetLevel(m_plantLong, objData);

				//リストに追加
				m_objectList.emplace_back(m_plantLong);
				return true;
			}
			else if (objData.EqualObjectName(L"plant_low") == true)
			{
				//小花瓶オブジェクトを作成する。
				m_plantLow = NewGO<PlantLow>(0, "plantlow");

				SetLevel(m_plantLow, objData);

				//リストに追加
				m_objectList.emplace_back(m_plantLow);
				return true;
			}
			else if (objData.EqualObjectName(L"rock_big") == true)
			{
				//岩オブジェクトを作成する。
				m_rockBig = NewGO<RockBig>(0, "rockbig");

				SetLevel(m_rockBig, objData);

				//リストに追加
				m_objectList.emplace_back(m_rockBig);
				return true;
			}
			else if (objData.EqualObjectName(L"solarPanel") == true)
			{
				//ソーラーパネルオブジェクトを作成する。
				m_solarPanel = NewGO<SolarPanel>(0, "solarpanel");

				SetLevel(m_solarPanel, objData);

				//リストに追加
				m_objectList.emplace_back(m_solarPanel);
				return true;
			}
			//名前がhumanEnemyだったら。
			else if (objData.EqualObjectName(L"humanEnemy") == true)
			{
				//エネミーオブジェクトを作成する。
				m_humanEnemy = NewGO<nsHumanEnemy::HumanEnemy>(0, "humanenemy");
				//座標を設定する。
				m_humanEnemy->SetPosition(objData.position);
				//大きさを設定する。
				m_humanEnemy->SetScale(objData.scale);
				//回転を設定する。
				m_humanEnemy->SetRotation(objData.rotation);
				//リストに追加
				m_enemyList.emplace_back(m_humanEnemy);
				return true;
			}
			//名前がwallだったら。
			else if (objData.EqualObjectName(L"wall") == true)
			{
				//当たり判定壁オブジェクトを作成する。
				m_wall = NewGO<Wall>(0, "wall");
				//座標を設定する。
				m_wall->SetPosition(objData.position);
				//大きさを設定する。
				m_wall->SetScale(objData.scale);
				//回転を設定する。
				m_wall->SetRotation(objData.rotation);
				return true;
			}

			return true;
		});
	
	//スカイキューブの作成
	SetSkyCube();

	//レティクルの作成
	m_reticle = NewGO<nsPlayer::Reticle>(0, "reticle");

	//UIの作成
	m_playerUI = NewGO<nsPlayer::PlayerUI>(0, "playerui");

	//ゲームカメラの作成
	m_camera = NewGO<GameCamera>(0, "gamecamera");

	//ライトの作成
	m_light = NewGO<Lighting>(0, "lighting");

	//黒フェードの作成
	m_blackFade = NewGO<BlackFade>(1, "blackfade");

	//ミッションUIの作成
	m_missionUI = NewGO<MissionUI>(0, "missionui");

	return true;
}

template <typename T>
void Game::SetLevel(T* objct, LevelObjectData& objData)
{
	//座標を設定する。
	objct->SetPosition(objData.position);
	//大きさを設定する。
	objct->SetScale(objData.scale);
	//回転を設定する。
	objct->SetRotation(objData.rotation);
}

void Game::SetGameClear()
{
	if (m_gameClear == false && m_gameOver == false)
	{
		//フェードインの開始
		m_blackFade->SetAlphaUp(true);

		m_gameClear = true;
	}
}

void Game::SetGameOver()
{
	if (m_gameClear == false && m_gameOver == false)
	{
		//フェードインの開始
		m_blackFade->SetAlphaUp(true);

		m_gameOver = true;
	}
}

void Game::Update()
{
	if (m_gameClear == true)
	{
		//フェードインが終わったら
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//ゲームクリアを生成
			NewGO<GameClear>(0, "gameclear");

			//自分自身の削除
			DeleteGO(this);
		}
	}
	else if (m_gameOver == true)
	{
		//フェードインが終わったら
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//ゲームオーバーを生成
			NewGO<GameOver>(0, "gameover");

			//自分自身の削除
			DeleteGO(this);
		}
	}
}

void Game::SetSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(2000.0f);
	m_skyCube->SetPosition({-5000.0f,500.0f,6000.0f});
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}

void Game::Render(RenderContext& rc)
{

}