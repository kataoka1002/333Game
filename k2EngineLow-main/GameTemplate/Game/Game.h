#pragma once
#include "SkyCube.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Lighting.h"
#include "Teapot.h"
#include "Reticle.h"
#include "level3D/Level.h"
#include "ObjectBase.h"
#include "HumanEnemy.h"
#include "House.h"

class Game : public IGameObject, Noncopyable
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetSkyCube();

	/// <summary>
	/// �I�u�W�F�N�g�����X�g����폜����
	/// </summary>
	/// <param name="teapot"></param>
	void RemoveObjectFromList(ObjectBase* object)
	{
		std::vector<ObjectBase*>::iterator it = std::find(
			m_objectList.begin(), 	//�I�u�W�F�N�g�̃��X�g�̍ŏ�
			m_objectList.end(), 	//�I�u�W�F�N�g�̃��X�g�̍Ō�
			object);				//�T���Ă���I�u�W�F�N�g

		if (it != m_objectList.end())
		{
			m_objectList.erase(it);
		}
	}

	/// <summary>
	/// �G�l�~�[�����X�g����폜����
	/// </summary>
	/// <param name="teapot"></param>
	void RemoveEnemyFromList(EnemyBase* enemy)
	{
		std::vector<EnemyBase*>::iterator it = std::find(
			m_enemyList.begin(), 	//�I�u�W�F�N�g�̃��X�g�̍ŏ�
			m_enemyList.end(), 		//�I�u�W�F�N�g�̃��X�g�̍Ō�
			enemy);					//�T���Ă���I�u�W�F�N�g

		if (it != m_enemyList.end())
		{
			m_enemyList.erase(it);
		}
	}

	/// <summary>
	/// �I�u�W�F�N�g�̃��X�g��Ԃ�
	/// </summary>
	/// <returns></returns>
	const std::vector<ObjectBase*>& GetObjectList() const
	{
		return m_objectList;
	}

	/// <summary>
	/// �G�l�~�[�̃��X�g��Ԃ�
	/// </summary>
	/// <returns></returns>
	const std::vector<EnemyBase*>& GetEnemyList() const
	{
		return m_enemyList;
	}


private:
	Level m_levelRender;
	SkyCube* m_skyCube = nullptr;
	nsPlayer::Player* m_player = nullptr;
	nsPlayer::Reticle* m_reticle = nullptr;
	BackGround* m_bg = nullptr;
	GameCamera* m_camera = nullptr;
	Lighting* m_light = nullptr;
	Teapot* m_teapot = nullptr;
	nsHumanEnemy::HumanEnemy* m_humanEnemy = nullptr;
	House* m_house = nullptr;

	std::vector<EnemyBase*> m_enemyList;			//�G�l�~�[�̃��X�g
	std::vector<ObjectBase*> m_objectList;			//�e�B�[�|�b�g�̃��X�g
};

