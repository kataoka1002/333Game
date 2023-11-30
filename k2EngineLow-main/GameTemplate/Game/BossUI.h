#pragma once

/// <summary>
/// �{�X�̖��O���
/// </summary>
namespace nsBoss
{
	class Boss;

	class BossUI : public IGameObject
	{
	public:
		~BossUI();
		bool Start();
		void Update();
		void Render(RenderContext& rc);

		void DrawPlate(bool flag)
		{
			m_drawPlate = flag;
		}

		void DrawHP(bool flag)
		{
			m_drawHP = flag;
		}

	private:
		SpriteRender m_HPFrame;		//HP�̃t���[��
		SpriteRender m_HPGage;		//HP�̃Q�[�W
		SpriteRender m_HPOrange;	//HP�̃I�����W�Q�[�W
		SpriteRender m_HPBack;		//HP�̔w�i
		SpriteRender m_name;		//�{�X�̖��O
		SpriteRender m_plate;		//�v���[�g

		Vector3 m_orangeScale = Vector3::One;	//�I�����W�Q�[�W�̑傫��
		bool m_drawPlate = false;				//�v���[�g��\�����邩�ǂ���
		bool m_drawHP = false;					//HP��\�����邩�ǂ���

		Boss* m_boss = nullptr;
	};
}