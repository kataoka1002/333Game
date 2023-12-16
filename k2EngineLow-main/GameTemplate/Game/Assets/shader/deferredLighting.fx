/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[
 */
#include "IBL.h"


static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 4;
static const int MAX_SPOT_LIGHT = 4;
static const float PI = 3.1415926f;
static const int NUM_SHADOW_MAP = 3;
static const int INFINITY = 40.0f;
static const int NUM_REFLECTION_TEXTURE = 5;


// �\����
struct DirectionLight
{
    float3 dirDirection;    //����
    float3 dirColor;        //�J���[
};

struct PointLight
{
    float3 ptPosition;   //�|�W�V����
    float3 ptColor;      //�J���[
    float ptRange;       //�e���͈�
};

struct SpotLight
{
    float3 spPosition;  //�|�W�V����
    float3 spColor;     //�J���[
    float spRange;      //�e���͈�
    float3 spDirection; //�ˏo����
    float spAngle;      //�ˏo�p�x
};

struct HemLight
{
    float3 skyColor;        //��̐F
    float3 groundColor;     //�n�ʂ̐F
    float3 groundNormal;    //�n�ʂ̖@��
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
    PointLight pointLight[MAX_POINT_LIGHT];
    SpotLight spotLight[MAX_SPOT_LIGHT];
    HemLight hemLight;
    float4x4 mLVP[NUM_SHADOW_MAP];				//���C�g�r���[�v���v���W�F�N�V�����s��
    float iblIntencity;                         // IBL�̋��x
    int isIBL;                                  // IBL���s��
    int isEnableRaytracing;                     // ���C�g�����L��
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
Texture2D<float4> albedoTexture : register(t0);             // �A���x�h
Texture2D<float4> normalTexture : register(t1);             // �@��
Texture2D<float4> worldPosTexture : register(t2);           // ���[���h���W
Texture2D<float4> normalInViewTexture : register(t3);       // �J������Ԃ̖@��
Texture2D<float4> metallicSmoothTexture : register(t4);     // ���^���b�N�X���[�X(�X�y�L����)
Texture2D<float4> shadowMap[NUM_SHADOW_MAP] : register(t5); // �V���h�E�}�b�v(GBuffer�ł͂Ȃ�)
TextureCube<float4> g_skyCubeMap : register(t8);           // �X�J�C�L���[�u

Texture2D<float4> g_raytracingTexture[NUM_REFLECTION_TEXTURE] : register(t20); //���C�g���̉摜

//sampler Sampler : register(s0);                           // �T���v���[

// �֐���`
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow);
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float isShadowReceiver, float3 worldPos);
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRPointLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos);
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRSpotLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos);
float3 CalcLimPower(float3 normal, float3 normalInView);
float3 CalcHemLight(float3 normal);
float CalcBeckmann(float m, float t);
float CalcSpcFresnel(float f0, float u);
float CookTorranceSpecular(float3 L, float3 V, float3 N, float smooth);
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float smooth);
float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color);
float CalcShadowPow(float isShadowReceiver, float3 worldPos);
float Chebyshev(float2 moments, float depth);
float4 SampleReflectionColor(float2 uv, float level);

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
    normal = (normal - 0.5f) * 2.0f;
    
    // ���[���h���W
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;
    
    // �J������Ԃ̖@��
    float3 normalInView = normalInViewTexture.Sample(Sampler, In.uv).xyz;
    
    // �X�y�L�����J���[�̓A���x�h�J���[�Ɠ���
    float3 specColor = albedo.xyz;

    // �����x(�X�y�L����)
    float metallic = metallicSmoothTexture.Sample(Sampler, In.uv).r;

    // ���炩��
    float smooth = metallicSmoothTexture.Sample(Sampler, In.uv).a;

    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
    float3 toEye = normalize(eyePos - worldPos);
        
    // --------------------------------------------------------------------
    
    
    // �f�B���N�V�������C�g�̌v�Z
    //float3 lig = CalcDirectionLight(normal, worldPos, metallic);
    float3 lig = CalcPBRDirectionLight(normal, toEye, albedo, specColor, metallic, smooth, normalTexture.Sample(Sampler, In.uv).w, worldPos);
    
    // �|�C���g���C�g�̋����ݒ�
    //lig += CalcPointLight(normal, worldPos, metallic);
    lig += CalcPBRPointLight(normal, toEye, albedo, specColor, metallic, smooth, worldPos);
    
    // �X�|�b�g���C�g�̋����ݒ�
    //lig += CalcSpotLight(normal, worldPos, metallic);
    lig += CalcPBRSpotLight(normal, toEye, albedo, specColor, metallic, smooth, worldPos);
    
    // �������C�g�̋����ݒ�(��������)
    //lig += CalcLimPower(normal, normalInView);
    
    // �������C�g�̋����ݒ�
    lig += CalcHemLight(normal);
    
    //if (isEnableRaytracing)
    //{
    //    // ���C�g�����s���ꍇ�̓��C�g���ō�������˃e�N�X�`����IBL�e�N�X�`������������B
    //    // GL�e�N�X�`��
    //    float reflectionRate = 1.0f - ((smooth - 0.5f) * 2.0f);
    //    float level = lerp(0.0f, (float) (NUM_REFLECTION_TEXTURE - 1), pow(reflectionRate, 3.0f));
    //    if (level < NUM_REFLECTION_TEXTURE - 1)
    //    {
    //        lig += albedo * SampleReflectionColor(In.uv, level);
    //    }
    //    else if (isIBL == 1)
    //    {
    //        // IBL������Ȃ�B
    //        lig += albedo * SampleIBLColorFromSkyCube(
    //            g_skyCubeMap,
    //            toEye,
    //            normal,
    //            smooth,
    //            iblIntencity
    //        );
    //    }
    //    else
    //    {
    //        // �����ɂ���グ
    //        float ambientLight = 1.1f;
    //        lig += albedo * ambientLight;
    //    }
    //}
    //else if (isIBL == 1)
    //{
    //    // ��������̔��˃x�N�g�������߂�B
    //    lig += albedo * SampleIBLColorFromSkyCube(
    //        g_skyCubeMap,
    //        toEye,
    //        normal,
    //        smooth,
    //        iblIntencity
    //    );
    //}
    //else
    {
        // �����ɂ���グ
        float ambientLight = 1.1f;
        lig += albedo * ambientLight;
    }
    
    // �e�̗�������v�Z����B
    float shadow = CalcShadowPow(normalTexture.Sample(Sampler, In.uv).w, worldPos) * normalTexture.Sample(Sampler, In.uv).w;
    lig *= max(0.5f, 1.0f - shadow);
    
    float4 finalColor = 1.0f;
    finalColor.xyz = lig;    
        
    //�F����F���߂ŕ␳����
    finalColor.r *= 0.9f;
    finalColor.g *= 0.9f;
    finalColor.b *= 1.2f;
    
    return finalColor;
}

////////�R�R����֐�/////////////////////////////////////////////////////////////////////////////////////////////////////

//�g�U���˂̌v�Z
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal)
{
    float power = max(0.0f, dot(normal, direction) * -1.0f);
    
    float3 lig = color * power;
    
    return lig;
}

//���ʔ��˂̌v�Z
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow)
{
    float3 toEye = normalize(eyePos - worldPos);
    
    float3 refVec = reflect(direction, normal);
    
    float power = max(0.0f, dot(toEye, refVec));
    power = pow(power, 5.0f);
    
    float3 lig = color * power * speclarPow;
    
    return lig;
}

//�f�B���N�V�������C�g�̌v�Z
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        lig += CalcLambertDiffuse(directionLight[i].dirDirection, directionLight[i].dirColor, normal);
        lig += CalcPhongSpecular(directionLight[i].dirDirection, directionLight[i].dirColor, worldPos, normal, specularPow);
    }
    
    return lig;
}

//PBR�̃f�B���N�V�������C�g�̌v�Z
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float isShadowReceiver, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {        
        // PBR�ɂ�郉�C�g�̋��������߂�
        lig += CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, directionLight[i].dirDirection, directionLight[i].dirColor);
    }
    
    return lig;
}

//�|�C���g���C�g�̌v�Z
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig;
    
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
     	// ���̃T�[�t�F�C�X�ɓ��˂��Ă���|�C���g���C�g�̌��̌������v�Z����
        float3 ligDir = worldPos - pointLight[i].ptPosition;
        
        // ���K�����đ傫��1�̃x�N�g���ɂ���
        ligDir = normalize(ligDir);

        // �����Ȃ���Lambert�g�U���ˌ����v�Z����
        float3 diffPoint = CalcLambertDiffuse(ligDir, pointLight[i].ptColor, normal);

        // �����Ȃ���Phong���ʔ��ˌ����v�Z����
        float3 specPoint = CalcPhongSpecular(ligDir, pointLight[i].ptColor, worldPos, normal, specularPow);

        // �����ɂ��e�������v�Z����
        // �|�C���g���C�g�Ƃ̋������v�Z����
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�3�悵�Ă���
        affect = pow(affect, 3.0f);

        // �g�U���ˌ��Ƌ��ʔ��ˌ��Ɍ���������Z���ĉe������߂�
        diffPoint *= affect;
        specPoint *= affect;
        
        // �ŏI�I�ɕԂ������ɑ����Ă���
        lig += diffPoint + specPoint;
    }

    return lig;
}

//PBR�̃|�C���g���C�g���v�Z
float3 CalcPBRPointLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
     	// ���̃T�[�t�F�C�X�ɓ��˂��Ă���|�C���g���C�g�̌��̌������v�Z����
        float3 ligDir = worldPos - pointLight[i].ptPosition;

        // ���K�����đ傫��1�̃x�N�g���ɂ���
        ligDir = normalize(ligDir);

        // PBR�ɂ����̋������v�Z����        
        float3 PBRLig = CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, ligDir, pointLight[i].ptColor);

        // �����ɂ��e�������v�Z����
        // �|�C���g���C�g�Ƃ̋������v�Z����
        float3 distance = length(worldPos - pointLight[i].ptPosition);

        // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / pointLight[i].ptRange * distance;
        // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

        // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�3�悵�Ă���
        affect = pow(affect, 3.0f);

        // ����������Z���ĉe������߂�
        lig += PBRLig * affect;
    }

    return lig;
}

//�X�|�b�g���C�g�̌v�Z
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //�T�[�t�F�C�X�ɓ��˂���X�|�b�g���C�g�̌��̌������v�Z����
        float3 LigDir = worldPos - spotLight[i].spPosition;
	    //���K��
        LigDir = normalize(LigDir);
    
	    //�����Ȃ���Lambert�g�U���ˌ����v�Z����
        float3 diffSpot = CalcLambertDiffuse(LigDir, spotLight[i].spColor, normal);
    
	    //�����Ȃ���Phong���ʔ��˂̌v�Z
        float3 specSpot = CalcPhongSpecular(LigDir, spotLight[i].spColor, worldPos, normal, specularPow);
    
	    //�X�|�b�g���C�g�Ƃ̋������v�Z����
        float distance = length(worldPos - spotLight[i].spPosition);
    
	    //�e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
	    //�e���͂��}�C�i�X�ɂȂ�Ȃ��悤��
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //�e�����w���֐��I�ɂ���
        affect = pow(affect, 3.0f);
    
	    //�g�U���ˌ��Ƌ��ʔ��ˌ��Ɍ���������Z���ĉe������߂�
        diffSpot *= affect;
        specSpot *= affect;
    
        // ��������͊p�x�ɂ��e���������߂�
        
	    //���ˌ��Ǝˏo�����̊p�x�����߂�
        float angle = dot(LigDir, spotLight[i].spDirection);
    
	    //dot()�ŋ��߂��l��acos()�ɓn���Ċp�x�����߂�
        angle = abs(acos(angle));
    
	    //�p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //�e�����w���֐��I�ɂ���
        affect = pow(affect, 0.5f);
    
	    //�p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
        diffSpot *= affect;
        specSpot *= affect;

        //�ŏI�I�ɕԂ������ɑ����Ă���
        lig += diffSpot;
        lig += specSpot;

    }
	
    return lig;
}

//PBR�̃X�|�b�g���C�g���v�Z
float3 CalcPBRSpotLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        //�T�[�t�F�C�X�ɓ��˂���X�|�b�g���C�g�̌��̌������v�Z����
        float3 ligDir = worldPos - spotLight[i].spPosition;
        
	    //���K��
        ligDir = normalize(ligDir);
    
        // PBR�ɂ����̋������v�Z����        
        float3 PBRLig = CalcPBR(normal, toEye, albedo, specColor, metallic, smooth, ligDir, spotLight[i].spColor);

	    //�X�|�b�g���C�g�Ƃ̋������v�Z����
        float distance = length(worldPos - spotLight[i].spPosition);
    
	    //�e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / spotLight[i].spRange * distance;
	    //�e���͂��}�C�i�X�ɂȂ�Ȃ��悤��
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //�e�����w���֐��I�ɂ���
        affect = pow(affect, 3.0f);
    
	    //����������Z���ĉe������߂�
        PBRLig *= affect;
    
        // ��������͊p�x�ɂ��e���������߂�
        
	    //���ˌ��Ǝˏo�����̊p�x�����߂�
        float angle = dot(ligDir, spotLight[i].spDirection);
    
	    //dot()�ŋ��߂��l��acos()�ɓn���Ċp�x�����߂�
        angle = abs(acos(angle));
    
	    //�p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
        affect = 1.0f - 1.0f / spotLight[i].spAngle * angle;
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }
    
	    //�e�����w���֐��I�ɂ���
        affect = pow(affect, 0.5f);
    
	    //�p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
        PBRLig *= affect;

        //�ŏI�I�ɕԂ������ɑ����Ă���
        lig += PBRLig;
    }
	
    return lig;
}

//�������C�g�̌v�Z
float3 CalcLimPower(float3 normal, float3 normalInView)
{
    float3 lig = { 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < MAX_DIRECTION_LIGHT; i++)
    {
        //�T�[�t�F�C�X�̖@���ƌ��̓��˕����Ɉˑ����郊���̋��������߂�
        float power1 = 1.0f - max(0.0f, dot(directionLight[i].dirDirection, normal));
        
	    //�T�[�t�F�C�X�̖@���Ǝ����̕����Ɉˑ����郊���̋��������߂�
        float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);
        
	    //�ŏI�I�ȃ����̋��������߂�
        float limPower = power1 * power2;
        
        limPower = max(0.0f, limPower);
        
	    //pow()���g�p�������̕ω����w���֐��I�ɂ���
        limPower = pow(limPower, 1.3f);
        
        //�������C�g�̃J���[���v�Z�������Ă���
        lig += directionLight[i].dirColor * limPower;
    }

    return lig;
}

//�������C�g�̌v�Z
float3 CalcHemLight(float3 normal)
{
	//�T�[�t�F�C�X�̖@���ƒn�ʂ̖@���Ƃ̓��ς��v�Z����
    float t = dot(normal, hemLight.groundNormal);
    
	//���ς̌��ʂ�0�`1�͈̔͂ɕϊ�
    t = (t + 1.0f) / 2.0f;
    
	//�n�ʂƓV���F��⊮��t�Ő��`�⊮���A�Ԃ�
    return lerp(hemLight.groundColor, hemLight.skyColor, t);
}

//�x�b�N�}�����z���v�Z����
float CalcBeckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

//�t���l�����v�Z
float CalcSpcFresnel(float f0, float u)
{
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torrance���f���̋��ʔ��˂��v�Z
/// </summary>
/// <param name="L">�����Ɍ������x�N�g��</param>
/// <param name="V">���_�Ɍ������x�N�g��</param>
/// <param name="N">�@���x�N�g��</param>
/// <param name="metallic">�����x</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float smooth)
{
    // �}�C�N���t�@�Z�b�g���������Ȃ肷����ƁA���ʔ��˂������Ȃ肷���邱�Ƃ�����̂ŁA������0.5�ɂ���
    float microfacet = max(0.5f, 1.0f - smooth);

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = 0.5;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = max(saturate(dot(N, H)), 0.001f);
    float VdotH = max(saturate(dot(V, H)), 0.001f);
    float NdotL = max(saturate(dot(N, L)), 0.001f);
    float NdotV = max(saturate(dot(N, V)), 0.001f);

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = CalcBeckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = CalcSpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = max(0.01f,PI * NdotV * NdotH);  

    // �����܂ŋ��߂��A�l�𗘗p���āA�N�b�N�g�����X���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float smooth)
{
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);
    
    //�e����0.5�ŌŒ�B
    float roughness = 1.0f - smooth;
    
    //����̓G�l���M�[�̕ۑ�
    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotLH = saturate(dot(L, H));
    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A�������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���B
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;
    
    // �@���ƌ����Ɍ������x�N�g�����𗘗p���Ċg�U���˗������߂Ă��܂�
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    
    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂Ă��܂�
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    //�@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (FL * FV * energyFactor);
}

float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color)
{
    // �t���l�����˂��l�������g�U���˂��v�Z
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -direction, toEye, smooth);

    // ���K��Lambert�g�U���˂����߂�
    float NdotL = saturate(dot(normal, -direction));
    float3 lambertDiffuse = color * NdotL / PI;

    // �ŏI�I�Ȋg�U���ˌ����v�Z����
    float3 diffuse = albedo * diffuseFromFresnel * lambertDiffuse;

    // �N�b�N�g�����X���f���̋��ʔ��˗����v�Z����
    float3 spec = CookTorranceSpecular(-direction, toEye, normal, smooth)* color;

    // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
    // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);

    // ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
    return max(float3(0.0f, 0.0f, 0.0f), diffuse * (1.0f - smooth) + spec * smooth);
}

// �`�F�r�V�F�t�̕s�����𗘗p���āA�e�ɂȂ�\�����v�Z����B
float Chebyshev(float2 moments, float depth)
{
    if (depth <= moments.x)
    {
        return 0.0f;
    }
    // �Օ�����Ă���Ȃ�A�`�F�r�V�F�t�̕s�����𗘗p���Č���������m�������߂�
    float depth_sq = moments.x * moments.x;
    // ���̃O���[�v�̕��U������߂�
    // ���U���傫���قǁAvariance�̐��l�͑傫���Ȃ�
    float variance = moments.y - depth_sq;
    // ���̃s�N�Z���̃��C�g���猩���[�x�l�ƃV���h�E�}�b�v�̕��ς̐[�x�l�̍������߂�
    float md = depth - moments.x;
    // �����͂��m�������߂�
    float lit_factor = variance / (variance + md * md);
    float lig_factor_min = 0.3f;
    // �����͂��m���̉����ȉ��͉e�ɂȂ�悤�ɂ���B
    lit_factor = saturate((lit_factor - lig_factor_min) / (1.0f - lig_factor_min));
    // �����͂��m������e�ɂȂ�m�������߂�B
    return 1.0f - lit_factor;
}

// �e�𗎂Ƃ����ǂ����̌v�Z
float CalcShadowPow(float isShadowReceiver, float3 worldPos)
{
    float shadow = 0.0f;
    
    //�e���󂯂Ȃ��Ȃ�
    if(isShadowReceiver == false)
    {
        return shadow;
    }
    //�N���A�J���[�Ȃ�
    else if (worldPos.x == 10000.0f, worldPos.y == 10000.0f, worldPos.z == 10000.0f)
    {
        return shadow;
    }
    
    for (int cascadeIndex = 0; cascadeIndex < NUM_SHADOW_MAP; cascadeIndex++)
    {
        float4 posInLVP = mul(mLVP[cascadeIndex], float4(worldPos, 1.0f));
        float2 shadowMapUV = posInLVP.xy / posInLVP.w;
        float zInLVP = posInLVP.z / posInLVP.w;
        shadowMapUV *= float2(0.5f, -0.5f);
        shadowMapUV += 0.5f;
        
        // �V���h�E�}�b�vUV���͈͓�������
        if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
            && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f
            && zInLVP < 0.98f && zInLVP > 0.02f)
        {
            // �V���h�E�}�b�v����l���T���v�����O
            float4 shadowValue = shadowMap[cascadeIndex].Sample(Sampler, shadowMapUV);
            zInLVP -= 0.001f;
            float pos = exp(INFINITY * zInLVP);
            
            // �\�t�g�V���h�E�B
            shadow = Chebyshev(shadowValue.xy, pos);
                       
            break;
        }
    }
    
    return shadow;
}

/*!
 *@brief	GI���C�g���T���v�����O
 *@param[in]	uv				uv���W
 *@param[in]	level           ���˃��x��
 */
float4 SampleReflectionColor(float2 uv, float level)
{
    int iLevel = (int) level;
    float4 col_0;
    float4 col_1;
    if (iLevel == 0)
    {
        col_0 = g_raytracingTexture[0].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[1].Sample(Sampler, uv);
    }
    else if (iLevel == 1)
    {
        col_0 = g_raytracingTexture[1].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[2].Sample(Sampler, uv);
    }
    else if (iLevel == 2)
    {
        col_0 = g_raytracingTexture[2].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[3].Sample(Sampler, uv);
    }
    else if (iLevel == 3)
    {
        col_0 = g_raytracingTexture[3].Sample(Sampler, uv);
        col_1 = g_raytracingTexture[4].Sample(Sampler, uv);
    }

    return lerp(col_0, col_1, frac(level));
}


