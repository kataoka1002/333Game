#pragma once
#include "SceneLight.h"
#include "PostEffect.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow
{
	class ModelRender;
	class SpriteRender;
	class FontRender;

	class RenderingEngine : public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		bool Start();
		void Init();
		void InitMainRenderTarget();
		void Init2DSprite();
		void InitCopyToframeBufferSprite();
		void InitGBuffer();
		void InitDefferedLightingSprite();
		void InitShadow();
		void Execute(RenderContext& rc);
		void RenderToGBuffer(RenderContext& rc);
		void DeferredLighting(RenderContext& rc);
		void SpriteFontDraw(RenderContext& rc);
		void DrawShadow(RenderContext& rc);
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		void ObjectClear();

		/// <summary>
		/// ���f�������_�[���R���e�i�̌��ɂ�������
		/// </summary>
		/// <param name="modelRender">���f�������_�[</param>
		void AddModelRenderObject(ModelRender* modelRender)
		{
			ModelRenderObject.push_back(modelRender);
		}

		/// <summary>
		/// �X�v���C�g�����_�[���R���e�i�̌��ɂ�������
		/// </summary>
		/// <param name="spriteRender">�X�v���C�g�����_�[</param>
		void AddSpriteRenderObject(SpriteRender* spriteRender)
		{
			SpriteRenderObject.push_back(spriteRender);
		}

		/// <summary>
		/// �t�H���g�����_�[���R���e�i�̌��ɂ�������
		/// </summary>
		/// <param name="fontRender">�t�H���g�����_�[</param>
		void AddFontRenderObject(FontRender* fontRender)
		{
			FontRenderObject.push_back(fontRender);
		}

		/// <summary>
		/// �V���h�E�}�b�v�ւ̕`��p�X�Ƀ��f����ǉ�
		/// </summary>
		/// <param name="model0">�ߌi�p�̃V���h�E�}�b�v�ɕ`�悷�郂�f��</param>
		/// <param name="model1">���i�p�̃V���h�E�}�b�v1�ɕ`�悷�郂�f��</param>
		/// <param name="model2">���i�p�̃V���h�E�}�b�v2�ɕ`�悷�郂�f��</param>
		void Add3DModelToRenderToShadowMap(Model& model0, Model& model1, Model& model2)
		{
			m_shadowMapRender.Add3DModel(model0, model1, model2);
		}


		/// <summary>
		/// �V�[�����C�g�̍\���̂�Ԃ�
		/// </summary>
		/// <returns></returns>
		Light& GetLightCB()
		{
			return m_sceneLight.GetLight();
		}

		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNum">�i���o�[</param>
		/// <param name="direction">����</param>
		/// <param name="color">�J���[</param>
		void SetDirectionLight(int lightNum, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNum, direction, color);
		}

		/// <summary>
		/// �|�C���g���C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNum">�i���o�[</param>
		/// <param name="position">�|�W�V����</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		void SetPointLight(int lightNum, Vector3 position, Vector3 color, float range)
		{
			m_sceneLight.SetPointLight(lightNum, position, color, range);
		}

		void SetPointLightPosition(int lightNum, Vector3 position)
		{
			m_sceneLight.SetPointLightPosition(lightNum, position);
		}

		/// <summary>
		/// �X�|�b�g���C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNum">�i���o�[</param>
		/// <param name="position">�|�W�V����</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		/// <param name="direction">�ˏo����</param>
		/// <param name="angle">�ˏo�p�x</param>
		void SetSpotLight(int lightNum, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_sceneLight.SetSpotLight(lightNum, position, color, range, direction, angle);
		}

		/// <summary>
		/// �������C�g�̐ݒ�
		/// </summary>
		/// <param name="skyColor">��̐F</param>
		/// <param name="groundColor">�n�ʂ̐F</param>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemLight(Vector3 skyColor, Vector3 groundColor, Vector3 normal)
		{
			m_sceneLight.SetHemLight(skyColor, groundColor, normal);
		}

		void SetLVP(Matrix mat, int num)
		{
			m_sceneLight.SetLVP(mat, num);
		}

		RenderTarget& GetShadowMap()
		{
			return m_shadowMap;
		}

	private:
		// GBuffer�̒�`
		enum EnGBuffer
		{
			enGBufferAlbedo,				// �A���x�h
			enGBufferNormal,				// �@��
			enGBufferWorldPos,				// ���[���h���W
			enGBufferNormalInView,			// �J������Ԃ̖@��
			enGBufferMetallicSmooth,		// ���^���b�N�X���[�X�}�b�v
			enGBufferNum,					// G-Buffer�̐�
		};

		SceneLight m_sceneLight;                        // �V�[�����C�g
		RenderTarget m_mainRenderTarget;				// ���C�������_�����O�^�[�Q�b�g
		RenderTarget m_gBuffer[enGBufferNum];			// GBuffer
		RenderTarget m_2DRenderTarget;					// 2D�`��p�̃^�[�Q�b�g
		RenderTarget m_shadowMap;						// �V���h�E�}�b�v
		Sprite m_copyToframeBufferSprite;				// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		Sprite m_diferredLightingSprite;				// �f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g
		Sprite m_2DSprite;								// 2D�`��p�̃X�v���C�g
		Sprite m_mainSprite;							// ���C���X�v���C�g
		PostEffect m_postEffect;						// �|�X�g�G�t�F�N�g
		Camera m_lightCamera;							// ���C�g�J����
		shadow::ShadowMapRender m_shadowMapRender;		// �V���h�E�}�b�v�����_�[

		std::vector<ModelRender*> ModelRenderObject;	// ���f�������_�[
		std::vector<SpriteRender*> SpriteRenderObject;	// �X�v���C�g�����_�[
		std::vector<FontRender*> FontRenderObject;		// �t�H���g�����_�[
	};
}

