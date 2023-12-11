#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine::RenderingEngine()
	{
		//Init();
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
		InitZPrepassTarget();
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

	void RenderingEngine::InitZPrepassTarget()
	{
		float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
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

		//���f���̖����ʒu(�X�J�C�L���[�u�̈ʒu)�̃��[���h���W��傫�����邱�Ƃŉe�𗎂Ƃ��Ȃ�
		float clearWorld[] = { 10000.0f,10000.0f,10000.0f,1.0f };
		//���[���h���W�p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferWorldPos].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			clearWorld
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
		// GI�e�N�X�`�����쐬���邽�߂̃u���[����������������B
		m_giTextureBlur[eGITextureBlur_1024x1024].Init(&g_graphicsEngine->GetRaytracingOutputTexture(), 1024, 1024);
		m_giTextureBlur[eGITextureBlur_512x512].Init(&m_giTextureBlur[eGITextureBlur_1024x1024].GetBokeTexture(), 512, 512);
		m_giTextureBlur[eGITextureBlur_256x256].Init(&m_giTextureBlur[eGITextureBlur_512x512].GetBokeTexture(), 256, 256);
		m_giTextureBlur[eGITextureBlur_128x128].Init(&m_giTextureBlur[eGITextureBlur_256x256].GetBokeTexture(), 128, 128);

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
			spriteInitData.m_textures[texNum++] = &m_shadowMapRender.GetBokeShadowMap(areaNo);
		}
		//���C�g�����\�Ȃ�
		if (g_graphicsEngine->IsPossibleRaytracing()) 
		{
			spriteInitData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetRaytracingOutputTexture();
			spriteInitData.m_expandShaderResoruceView[1] = &m_giTextureBlur[eGITextureBlur_1024x1024].GetBokeTexture();
			spriteInitData.m_expandShaderResoruceView[2] = &m_giTextureBlur[eGITextureBlur_512x512].GetBokeTexture();
			spriteInitData.m_expandShaderResoruceView[3] = &m_giTextureBlur[eGITextureBlur_256x256].GetBokeTexture();
			spriteInitData.m_expandShaderResoruceView[4] = &m_giTextureBlur[eGITextureBlur_128x128].GetBokeTexture();
		}
		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";
		//spriteInitData.m_expandConstantBuffer = &m_deferredLightingCB;
		spriteInitData.m_expandConstantBuffer = &m_sceneLight.GetLight();
		//spriteInitData.m_expandConstantBufferSize = sizeof(m_deferredLightingCB);
		spriteInitData.m_expandConstantBufferSize = sizeof(m_sceneLight.GetLight());

		if (m_iblData.m_texture.IsValid()) 
		{
			spriteInitData.m_textures[texNum++] = &m_iblData.m_texture;
			m_deferredLightingCB.m_isIBL = 1;
			m_deferredLightingCB.m_iblLuminance = m_iblData.m_intencity;
		}
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		// �f�B�t�@�[�h�����_�����O�p�̃X�v���C�g��������
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::ReInitIBL(const wchar_t* iblTexFilePath, float luminance)
	{
		// IBL�f�[�^���������B
		InitIBLData(iblTexFilePath, luminance);

		InitDefferedLightingSprite();
	}

	void RenderingEngine::InitIBLData(const wchar_t* iblTexFilePath, float intencity)
	{
		m_iblData.m_texture.InitFromDDSFile(iblTexFilePath);
		m_iblData.m_intencity = intencity;
		g_graphicsEngine->SetRaytracingSkyCubeBox(m_iblData.m_texture);
	}

	void RenderingEngine::CalcViewProjectionMatrixForViewCulling()
	{
		Matrix projMatrix;
		projMatrix.MakeProjectionMatrix(
			g_camera3D->GetViewAngle() * 1.5f,
			g_camera3D->GetAspect(),
			g_camera3D->GetNear(),
			g_camera3D->GetFar()
		);
		m_viewProjMatrixForViewCulling.Multiply(g_camera3D->GetViewMatrix(), projMatrix);
	}

	void RenderingEngine::Update()
	{
		// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s��̌v�Z�B
		CalcViewProjectionMatrixForViewCulling();
		// �V�[���̃W�I���g�����̍X�V�B
		m_sceneGeometryData.Update();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// �V�[�����C�g�̃f�[�^���R�s�[�B
		m_deferredLightingCB.m_light = m_sceneLight.GetLight();
		m_deferredLightingCB.m_isEnableRaytracing = IsEnableRaytracing() ? 1 : 0;

		// ZPrepass
		ZPrepass(rc);

		//GBuffer�ւ̃����_�����O
		RenderToGBuffer(rc);

		//�G�t�F�N�g�̕`��
		DrawEffect(rc);

		//�V���h�E�}�b�v�ɉe��`��
		DrawShadowMap(rc);

		//EyePos�̓J�����̈ʒu
		SetEyePosition(g_camera3D->GetPosition());
		
		// ���C�g���p�̃��C�g�f�[�^���R�s�[�B
		//m_raytracingLightData.m_directionalLight = m_sceneLight.GetSceneLight().directionalLight[0];
		m_raytracingLightData.m_iblIntencity = m_iblData.m_intencity;
		m_raytracingLightData.m_ambientLight = Vector3{ 1.1f,1.1f,1.1f };
		m_raytracingLightData.m_enableIBLTexture = m_iblData.m_texture.IsValid() ? 1 : 0;

		// ���C�g���ŉf�荞�݉摜���쐬����B
		/*if (IsEnableRaytracing()) 
		{
			g_graphicsEngine->DispatchRaytracing(rc);

			for (auto& blur : m_giTextureBlur) 
			{
				blur.ExecuteOnGPU(rc, 5);
			}
		}*/

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

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		BeginGPUEvent("ZPrepass");
		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : ModelRenderObject) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
		EndGPUEvent();
	}

	void RenderingEngine::DrawEffect(RenderContext& rc)
	{
		BeginGPUEvent("DrawEffect");

		// �A���x�h��g�o�b�t�@�[���Z�b�g
		rc.WaitUntilToPossibleSetRenderTarget(m_gBuffer[enGBufferAlbedo]);
		rc.SetRenderTargetAndViewport(m_gBuffer[enGBufferAlbedo]);
		
		//�����ŃG�t�F�N�g�h���[�B
		EffectEngine::GetInstance()->Draw();

		// �I���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_gBuffer[enGBufferAlbedo]);
	
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
		BeginGPUEvent("CascadeShadow");

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
