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

	private:
		SpriteRender m_HPFrame;		//HP�̃t���[��
		SpriteRender m_HPGage;		//HP�̃Q�[�W
		SpriteRender m_HPOrange;	//HP�̃I�����W�Q�[�W
		SpriteRender m_HPBack;		//HP�̔w�i
		SpriteRender m_name;		//�{�X�̖��O

		Vector3 m_orangeScale = Vector3::One;	//�I�����W�Q�[�W�̑傫��

		Boss* m_boss = nullptr;
	};
}