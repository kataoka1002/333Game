#pragma once

/// <summary>
/// �v���C���[�̖��O���
/// </summary>
namespace nsPlayer
{
	//�O���錾
	class Player;

	class PlayerUI : public IGameObject
	{
	public:
		~PlayerUI();
		bool Start();
		void Update();
		void Render(RenderContext& rc);

	private:
		SpriteRender m_HPFrame;		//HP�̃t���[��
		SpriteRender m_HPGage;		//HP�̃Q�[�W
		SpriteRender m_HPBack;		//HP�̔w�i
		SpriteRender m_plus;		//HP�́{

		Player* m_player = nullptr;
	};
}