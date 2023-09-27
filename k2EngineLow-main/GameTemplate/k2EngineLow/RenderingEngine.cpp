#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine::RenderingEngine()
	{
		Init();
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	bool RenderingEngine::Start()
	{
		return true;
	}

	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		Init2DSprite();
		InitCopyToframeBufferSprite();
		InitGBuffer();
		InitShadow();
		InitDefferedLightingSprite();
		m_postEffect.Init(m_mainRenderTarget);
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		//���C�������_�����O�^�[�Q�b�g�̍쐬
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),    
			g_graphicsEngine->GetFrameBufferHeight(),   
			1,
			1,											
			DXGI_FORMAT_R32G32B32A32_FLOAT,				
			DXGI_FORMAT_D32_FLOAT						
		);
	}

	void RenderingEngine::InitShadow()
	{
		m_shadowMapRender.Init();
	}

	void RenderingEngine::Init2DSprite()
	{
		float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };

		//2D�p�̃^�[�Q�b�g�̏�����
		m_2DRenderTarget.Create(
			m_mainRenderTarget.GetWidth(),
			m_mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//�ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//2D�p�̃V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

		//�e�N�X�`����2D�����_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		//�𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		m_2DSprite.Init(spriteInitData);

		//�e�N�X�`���̓��C�������_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//�𑜓x��2D�����_�\�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitCopyToframeBufferSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToframeBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitGBuffer()
	{
		// �A���x�h�J���[�p�̃^�[�Q�b�g���쐬
		float clearColor[] = { 0.5f,0.5f,0.5f,1.0f };
		m_gBuffer[enGBufferAlbedo].Create(
			g_graphicsEngine->GetFrameBufferWidth(),    
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//�@���ƃX�y�L�����p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferNormal].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//���[���h���W�p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferWorldPos].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//�J������Ԃ̖@���p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferNormalInView].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//���^���b�N�X���[�X�}�b�v�p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferMetallicSmooth].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);
	}

	void RenderingEngine::InitDefferedLightingSprite()
	{
		// �|�X�g�G�t�F�N�g�I�Ƀf�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
		SpriteInitData spriteInitData;
		spriteInitData.m_width = FRAME_BUFFER_W;
		spriteInitData.m_height = FRAME_BUFFER_H;

		// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
		spriteInitData.m_textures[enGBufferAlbedo] = &m_gBuffer[enGBufferAlbedo].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferNormal] = &m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferWorldPos] = &m_gBuffer[enGBufferWorldPos].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferNormalInView] = &m_gBuffer[enGBufferNormalInView].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferMetallicSmooth] = &m_gBuffer[enGBufferMetallicSmooth].GetRenderTargetTexture();
		int texNum = 5;
		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			spriteInitData.m_textures[texNum++] = &m_shadowMapRender.GetShadowMap(areaNo);
		}
		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";
		spriteInitData.m_expandConstantBuffer = &GetLightCB();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetLightCB());
		
		// �f�B�t�@�[�h�����_�����O�p�̃X�v���C�g��������
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//GBuffer�ւ̃����_�����O
		RenderToGBuffer(rc);

		//�V���h�E�}�b�v�ɉe��`��
		DrawShadowMap(rc);

		//�f�B�t�@�[�h���C�e�B���O
		DeferredLighting(rc);

		//�|�X�g�G�t�F�N�g�̎��s
		m_postEffect.Render(rc, m_mainRenderTarget);

		//�t�H���g�ƃX�v���C�g�̕`��
		SpriteFontDraw(rc);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);

		//�`�悵���I�u�W�F�N�g���N���A����
		ObjectClear();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");

		// �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX���ď�������
		RenderTarget* rts[] = {
			&m_gBuffer[enGBufferAlbedo],   // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
			&m_gBuffer[enGBufferNormal],
			&m_gBuffer[enGBufferWorldPos],
			&m_gBuffer[enGBufferNormalInView],
			&m_gBuffer[enGBufferMetallicSmooth],
		};

		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		// �܂Ƃ߂ă��f�������_�[��`��
		for (auto& ModelData : ModelRenderObject) 
		{
			ModelData->OnDraw(rc);
		}

		// �����_�����O�^�[�Q�b�g�ւ̏������ݑ҂�
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

		EndGPUEvent();
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		BeginGPUEvent("DeferrdLighting");

		// �����_�����O������C�������_�����O�^�[�Q�b�g�ɂ���
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
		m_diferredLightingSprite.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::DrawShadowMap(RenderContext& rc)
	{
		BeginGPUEvent("CascadeShadowDraw");

		//���C�g�J�����̕�����ݒ�
		Vector3 lightCameraDir = { 1.0f, -1.0f, -1.0f };
		lightCameraDir.Normalize();

		//�V���h�E�}�b�v�̏���
		m_shadowMapRender.Render(rc, lightCameraDir);

		EndGPUEvent();
	}

	void RenderingEngine::SpriteFontDraw(RenderContext& rc)
	{
		BeginGPUEvent("SpriteFontDraw");

		// 2D�p�̃^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// �^�[�Q�b�g�Z�b�g
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// �^�[�Q�b�g�̃N���A
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		// �X�v���C�g�ƕ�����`��
		for(auto SpriteData : SpriteRenderObject)
		{
			SpriteData->OnDraw(rc);
		}
		for(auto FontData : FontRenderObject) 
		{
			FontData->OnDraw(rc);
		}

		// �`�悳���܂ő҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		// �^�[�Q�b�g�����C���ɖ߂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		m_2DSprite.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");

		// �t���[���o�b�t�@�ɃZ�b�g
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		m_copyToframeBufferSprite.Draw(rc);

		EndGPUEvent();
	}

	void RenderingEngine::ObjectClear()
	{
		//�N���A
		ModelRenderObject.clear();
		SpriteRenderObject.clear();
		FontRenderObject.clear();
	}
}
