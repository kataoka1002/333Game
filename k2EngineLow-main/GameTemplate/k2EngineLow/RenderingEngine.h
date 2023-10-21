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
		// �f�B���N�V�������C�g
		struct DirectionalLight
		{
			Vector3 direction = { 1.0f,0.0f,0.0f };		// ���C�g�̕���
			int castShadow = 0;							// �e���L���X�g����?
			Vector4 color = { 1.0f,1.0f,1.0f,1.0f };    // ���C�g�̃J���[
		};

		/// <summary>
		/// ���C�g���p�̃��C�g�f�[�^�B
		/// </summary>
		struct RaytracingLightData 
		{
			DirectionalLight m_directionalLight;  // �f�B���N�V���i�����C�g�B
			Vector3 m_ambientLight;               // �����BIBL�e�N�X�`�����w�肳��Ă��Ȃ��ꍇ�ɗ��p�����B
			float m_iblIntencity;                 // IBL���x�B
			int m_enableIBLTexture;               // IBL�e�N�X�`�����w�肳��Ă���B
		};

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
		void DrawShadowMap(RenderContext& rc);
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
		/// ���C�g�����[���h�Ƀ��f����ǉ��B
		/// </summary>
		/// <param name="model">�ǉ����郂�f��</param>
		void AddModelToRaytracingWorld(Model& model)
		{
			g_graphicsEngine->RegistModelToRaytracingWorld(model);
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

		/// <summary>
		/// �萔�o�b�t�@�ɍs����Z�b�g����
		/// </summary>
		/// <param name="mat">�s��</param>
		/// <param name="num">�V���h�E�}�b�v�̔ԍ�</param>
		void SetLVP(Matrix mat, int num)
		{
			m_sceneLight.SetLVP(mat, num);
		}

		/// <summary>
		/// EyePos�̐ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		void SetEyePosition(Vector3 pos)
		{
			m_sceneLight.SetEyePos(pos);
		}


		/// <summary>
		/// IBL���ď������B
		/// </summary>
		void ReInitIBL(const wchar_t* iblTexFilePath, float luminance);

		/// <summary>
		/// �����̌v�Z�̂��߂�IBL�e�N�X�`����ݒ�B
		/// </summary>
		/// <remark>
		/// ���̊֐��𗘗p���āAIBL�e�N�X�`�����Z�b�g����ƁA
		/// ������IBL�e�N�X�`������T���v�����O���āA����𗘗p����
		/// ���C�e�B���O���s���܂��B
		/// IBL�e�N�X�`���𗘗p���������̌v�Z���I�t�ɂ������ꍇ�́ADisableIBLForAmbinet()���Ăяo���āA
		/// IBL�𖳌��ɂ��Ă��������B
		/// </remark>
		/// <param name="textureFilePath">
		/// IBL�e�N�X�`���̃t�@�C���p�X�B
		/// �L���[�u�}�b�v�ł���K�v������܂��B
		/// </param>
		/// <param name="luminance">
		/// IBL�e�N�X�`���̖��邳�B
		/// <param>
		void SetAmbientByIBLTexture(const wchar_t* textureFilePath, float luminance)
		{
			ReInitIBL(textureFilePath, luminance);
		}
		/// <summary>
		/// ���C�g���[�V���O���L�����ǂ����𔻒肷��B
		/// </summary>
		/// <returns></returns>
		bool IsEnableRaytracing() const
		{
			return m_isEnableRaytracing && g_graphicsEngine->IsPossibleRaytracing();
		}
		/// <summary>
		/// ���C�g���[�V���O��L���ɂ��܂��B
		/// ���̐ݒ�̓n�[�h�E�F�A���C�g���[�V���O�������ȏꍇ�͖�������܂��B
		/// </summary>
		void EnableRaytracing()
		{
			m_isEnableRaytracing = true && IsEnableRaytracing();
		}
		/// <summary>
		/// ���C�g���[�V���O�𖳌��ɂ��܂��B
		/// ���̐ݒ�̓n�[�h�E�F�A���C�g���[�V���O�������ȏꍇ�͖�������܂��B
		/// </summary>
		void DisableRaytracing()
		{
			m_isEnableRaytracing = false && IsEnableRaytracing();
		}
		/// <summary>
		/// ���C�g���p�̃��C�g�f�[�^���擾�B
		/// </summary>
		/// <returns></returns>
		RaytracingLightData& GetRaytracingLightData()
		{
			return m_raytracingLightData;
		}

	private:
			/// <summary>
			/// �C���[�W�x�[�X�h���C�e�B���O(IBL)�̂��߂̃f�[�^������������B
			/// </summary>
			/// <param name="iblTexFilePath">IBL�e�N�X�`���̃t�@�C���p�X�B</param>
			/// <param name="intencity">IBL�̋��x�B</param>
			void InitIBLData(const wchar_t* iblTexFilePath, float intencity);

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

		/// <summary>
		/// GI�e�N�X�`������邽�߂̃u���[�����B
		/// </summary>
		enum EGITextureBlur 
		{
			eGITextureBlur_1024x1024,   // 1024�~1024
			eGITextureBlur_512x512,     // 512�~512
			eGITextureBlur_256x256,     // 256�~256
			eGITextureBlur_128x128,     // 128�~128
			eGITextureBlur_Num,
		};

		/// <summary>
		/// IBL�f�[�^
		/// </summary>
		struct SIBLData 
		{
			Texture m_texture;          // IBL�e�N�X�`��
			float m_intencity = 1.0f;   // ���x�B
		};


		SceneLight m_sceneLight;							// �V�[�����C�g
		RenderTarget m_mainRenderTarget;					// ���C�������_�����O�^�[�Q�b�g
		RenderTarget m_gBuffer[enGBufferNum];				// GBuffer
		RenderTarget m_2DRenderTarget;						// 2D�`��p�̃^�[�Q�b�g
		RenderTarget m_shadowMap;							// �V���h�E�}�b�v
		Sprite m_copyToframeBufferSprite;					// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		Sprite m_diferredLightingSprite;					// �f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g
		Sprite m_2DSprite;									// 2D�`��p�̃X�v���C�g
		Sprite m_mainSprite;								// ���C���X�v���C�g
		PostEffect m_postEffect;							// �|�X�g�G�t�F�N�g
		Camera m_lightCamera;								// ���C�g�J����
		shadow::ShadowMapRender m_shadowMapRender;			// �V���h�E�}�b�v�����_�[
		bool m_isEnableRaytracing = true;					// ���C�g���[�V���O���L���H
		GaussianBlur m_giTextureBlur[eGITextureBlur_Num];   // GI�e�N�X�`���Ƀu���[�������鏈��
		RaytracingLightData m_raytracingLightData;			// ���C�g���p�̃��C�g�f�[�^
		SIBLData m_iblData;                                 // IBL�f�[�^�B

		std::vector<ModelRender*> ModelRenderObject;	// ���f�������_�[
		std::vector<SpriteRender*> SpriteRenderObject;	// �X�v���C�g�����_�[
		std::vector<FontRender*> FontRenderObject;		// �t�H���g�����_�[
	};
}

