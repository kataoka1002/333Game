#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"
namespace nsK2EngineLow {

	void PostEffect::Init(RenderTarget& renderTarget)
	{
		// �P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬
		luminanceRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),    // �e�N�X�`���̕�
			g_graphicsEngine->GetFrameBufferHeight(),   // �e�N�X�`���̍���
			1,
			1,			
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		// �P�x���o�p�̃X�v���C�g��������
		// �����������쐬����
		SpriteInitData luminanceSpriteInitData;
		// �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		// �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
		luminanceSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		luminanceSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
		luminanceSpriteInitData.m_textures[0] = &renderTarget.GetRenderTargetTexture();
		// �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// �쐬�����������������ƂɃX�v���C�g������������
		m_luminanceSprite.Init(luminanceSpriteInitData);

		// �K�E�V�A���u���[��������
		gaussianBlur[0].Init(&luminanceRenderTarget.GetRenderTargetTexture());
		gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
		gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
		gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());

		// �{�P�摜���������ď������ނ��߂̃X�v���C�g��������
		SpriteInitData finalSpriteInitData;
		// �{�P�e�N�X�`����4���w�肷��
		finalSpriteInitData.m_textures[0] = &gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &gaussianBlur[3].GetBokeTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// �ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
		// 2D�p�̃V�F�[�_�[���g�p����
		finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
		// �������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		// �J���[�o�b�t�@�[�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�[
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// �������������ɉ��Z�����p�̃X�v���C�g������������
		m_finalSprite.Init(finalSpriteInitData);

	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& renderTarget)
	{
		// �P�x���o�p�^�[�Q�b�g�ɕύX���g����悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);
		// �^�[�Q�b�g�Z�b�g
		rc.SetRenderTargetAndViewport(luminanceRenderTarget);
		// �^�[�Q�b�g�̃N���A
		rc.ClearRenderTargetView(luminanceRenderTarget);
		//�`�悵�ďI���܂ő҂�
		m_luminanceSprite.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);

		//�K�E�V�A���u���[�̎��s(�ڂ���)
		gaussianBlur[0].ExecuteOnGPU(rc, 10);
		gaussianBlur[1].ExecuteOnGPU(rc, 10);
		gaussianBlur[2].ExecuteOnGPU(rc, 10);
		gaussianBlur[3].ExecuteOnGPU(rc, 10);

		// MainRenderTarget�Z�b�g
		rc.WaitUntilToPossibleSetRenderTarget(renderTarget);
		rc.SetRenderTargetAndViewport(renderTarget);
		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����(mainrenderTarget�ƃu���[��������)
		m_finalSprite.Draw(rc);
		// �I���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(renderTarget);
	}
}
