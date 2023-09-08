/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[
 */
static const int MAX_DIRECTION_LIGHT = 4;

// �\����
struct DirectionLight
{
    float3 dirDirection;    //�f�B���N�V�������C�g�̕���
    float3 dirColor;        //�f�B���N�V�������C�g�̃J���[
};


cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer LightCb : register(b1)
{
    DirectionLight directionLight[MAX_DIRECTION_LIGHT];
    float3 eyePos;
}

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// �ϐ���`
Texture2D<float4> albedoTexture : register(t0);     // �A���x�h
Texture2D<float4> normalTexture : register(t1);     // �@��
Texture2D<float4> worldPosTexture : register(t2);   // ���[���h���W
sampler Sampler : register(s0);                     // �T���v���[

// �֐���`
float3 CalcDirectionLight(float3 normal,float3 worldPos);
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal);

// ���_�V�F�[�_�[
PSInput VSMain(VSInput vsIn)
{
    PSInput psIn;
    psIn.pos = mul(mvp, vsIn.pos);
    psIn.uv = vsIn.uv;
    return psIn;
}

// �s�N�Z���V�F�[�_�[
float4 PSMain(PSInput In) : SV_Target0
{
	// GBuffer�̓��e���g���ă��C�e�B���O-----------------------------------
    
    // �A���x�h
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
    
    // �@��
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;
    
    // ���[���h���W
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;

    // --------------------------------------------------------------------
    
    
    // �f�B���N�V�������C�g�̋����v�Z
    float3 dirLig = CalcDirectionLight(normal, worldPos);
    

    // ���C�g�̌����v�Z���ŏI�I�ȃJ���[��ݒ�    
    float4 finalColor = albedo;
    finalColor.xyz *= dirLig;
	
    return finalColor;
}

//////�R�R����֐�/////////////////////////////////////////////////////////////////////////////////////////////////////

//�f�B���N�V�������C�g�̌v�Z
float3 CalcDirectionLight(float3 normal, float3 worldPos)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT;i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal);
    }
    
    return lig;
}

//�g�U���˂̌v�Z
float3 CalcLambertDiffuse(float3 direction,float3 color,float3 normal)
{
    float power = max(0.0f, dot(normal, direction) * -1.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//���ʔ��˂̌v�Z
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal)
{
    float3 toEye = normalize(eyePos - worldPos);
    
    float3 refVec = reflect(direction, normal);
    
    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);
    
    float3 lig = color * power;
    
    return lig;
}