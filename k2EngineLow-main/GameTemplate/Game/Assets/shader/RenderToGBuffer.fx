// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};

//�s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; //���W�B
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
};

// �s�N�Z���V�F�[�_�[����̏o��
struct SPSOut
{
    float4 albedo : SV_Target0; // �A���x�h
    float3 normal : SV_Target1; //�@��
    float3 worldPos : SV_Target2;
};


//�V�F�[�_�[���\�[�X
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B

//�T���v���[�X�e�[�g
sampler g_sampler : register(s0);


//�X�L���s����v�Z����B
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

//���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }

    psIn.pos = mul(m, vsIn.pos);        // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);    // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos);    // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.normal = normalize(mul(m, vsIn.normal));

    psIn.uv = vsIn.uv;
    
    return psIn;
}

// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}


//���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
SPSOut PSMain(SPSIn psIn)
{
    //GBuffer�ɏo��
    SPSOut psOut;
    
    //�A���x�h�J���[�̒��o
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    //�@�����̒��o
    psOut.normal = (psIn.normal / 2.0f) + 0.5f;
    
    //���[���h���W�̒��o
    psOut.worldPos = psIn.worldPos;
    
    return psOut;
}