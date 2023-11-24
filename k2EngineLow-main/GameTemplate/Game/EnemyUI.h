#pragma once
namespace nsPlayer { class Player; }
class Game;
class EnemyBase;

class EnemyUI : public IGameObject, Noncopyable
{
public:
	EnemyUI();
	~EnemyUI();
	bool Start();
	void Update();
	void CalcFrame();
	void CalcRed();
	void CalcOrange();
	void CalcScale(Vector3& scale);
	void CalcPosition(Vector2& position, Vector2 spriteSize = Vector2::Zero, bool useLerp = false, bool isFrame = false);
	Vector3 SendHPBer(Vector2 size, Vector3 scale);	//�摜�̈ړ��ʂ����߂�
	void Render(RenderContext& rc);

	/// <summary>
	/// ��苗���ȓ�&�J�����̎���ɓ����Ă���Ȃ�̗͕\��
	/// </summary>
	/// <param name="enemy">�G�l�~�[�̃N���X</param>
	/// <returns>true : �`�悷��Afalse : �`�悵�Ȃ�</returns>
	bool CalcAngle(EnemyBase* enemy);

	/// <summary>
	/// �G�l�~�[���Z�b�g
	/// </summary>
	/// <param name="pointa">�|�C���^</param>
	void SetEnemy(EnemyBase* pointa)
	{
		m_enemy = pointa;
	}

private:
	EnemyBase* m_enemy = nullptr;
	nsPlayer::Player* m_player = nullptr;
	Game* m_game = nullptr;

	SpriteRender m_HPBlackSprite;
	SpriteRender m_HPOrangeSprite;
	SpriteRender m_HPRedSprite;
	SpriteRender m_HPFrameSprite;

	Vector2 m_redPosition = Vector2::Zero;
	Vector2 m_orangePosition = Vector2::Zero;
	Vector3 m_redScale = { 1.0f,1.0f,0.0f };
	Vector3 m_orangeScale = { 1.0f,1.0f,0.0f };
	Vector3 m_orangeScaleNew = { 1.0f,1.0f,0.0f };
	Vector3 m_orangeOffset = Vector3::Zero;
	Vector2 m_framePosition = Vector2::Zero;
};