/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[
 */


static const int MAX_DIRECTION_LIGHT = 4;
static const int MAX_POINT_LIGHT = 4;
static const int MAX_SPOT_LIGHT = 4;
static const float PI = 3.1415926f;
static const int NUM_SHADOW_MAP = 3;


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
sampler Sampler : register(s0);                             // �T���v���[

// �֐���`
float3 CalcLambertDiffuse(float3 direction, float3 color, float3 normal);
float3 CalcPhongSpecular(float3 direction, float3 color, float3 worldPos, float3 normal, float speclarPow);
float3 CalcDirectionLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth);
float3 CalcPointLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRPointLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos);
float3 CalcSpotLight(float3 normal, float3 worldPos, float specularPow);
float3 CalcPBRSpotLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 worldPos);
float3 CalcLimPower(float3 normal, float3 normalInView);
float3 CalcHemLight(float3 normal);
float CalcBeckmann(float m, float t);
float CalcSpcFresnel(float f0, float u);
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic);
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V);
float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color);
float CalcShadowPow(float isShadowReceiver, float3 worldPos);

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
        
    // --------------------------------------------------------------------
    
    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
    float3 toEye = normalize(eyePos - worldPos);

    
    // �f�B���N�V�������C�g�̋����v�Z
    //float3 dirLig = CalcDirectionLight(normal, worldPos, metallic);
    float3 dirLig = CalcPBRDirectionLight(normal, toEye, albedo, specColor, metallic, smooth);
    
    // �|�C���g���C�g�̋����ݒ�
    //float3 ptLig = CalcPointLight(normal, worldPos, metallic);
    float3 ptLig = CalcPBRPointLight(normal, toEye, albedo, specColor, metallic, smooth, worldPos);
    
    // �X�|�b�g���C�g�̋����ݒ�
    //float3 spLig = CalcSpotLight(normal, worldPos, metallic);
    float3 spLig = CalcPBRSpotLight(normal, toEye, albedo, specColor, metallic, smooth, worldPos);
    
    // �������C�g�̋����ݒ�
    float3 limLig = CalcLimPower(normal, normalInView);
    
    // �������C�g�̋����ݒ�
    float3 hemLig = CalcHemLight(normal);
    
    // �e�𗎂Ƃ������Ƃ��Ȃ����̌v�Z
    float shadowPow = CalcShadowPow(normalTexture.Sample(Sampler, In.uv).w, worldPos);
    
    // �S�Ẵ��C�g�̉e���͂����߂�
    float3 lightPow = dirLig + ptLig + spLig + limLig + hemLig;
    
	    
    // ���C�g�̌����v�Z���ŏI�I�ȃJ���[��ݒ�    
    float4 finalColor = albedo;
    finalColor.xyz *= lightPow;
    finalColor *= shadowPow;
    
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
float3 CalcPBRDirectionLight(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth)
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
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = metallic;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = CalcBeckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = CalcSpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = PI * NdotV * NdotH;

    // �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e����0.5�ŌŒ�B
    float roughness = 0.5f;

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotLH = saturate(dot(L, H));

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
    // �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // �@���ƌ����Ɍ������x�N�g��w�𗘗p���Ċg�U���˗������߂�
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (FL * FV * energyFactor);
}

float3 CalcPBR(float3 normal, float3 toEye, float4 albedo, float3 specColor, float metallic, float smooth, float3 direction, float3 color)
{
    // �t���l�����˂��l�������g�U���˂��v�Z
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -direction, toEye);

    // ���K��Lambert�g�U���˂����߂�
    float NdotL = saturate(dot(normal, -direction));
    float3 lambertDiffuse = color * NdotL / PI;

    // �ŏI�I�Ȋg�U���ˌ����v�Z����
    float3 diffuse = diffuseFromFresnel * lambertDiffuse;

    // Cook-Torrance���f���𗘗p�������ʔ��˗����v�Z����
    float3 spec = CookTorranceSpecular(-direction, toEye, normal, smooth) * color;

    // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
    // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);
    
    // �l��INF�Ȃ�ύX����
    if (isinf(spec.x) || isinf(spec.y) || isinf(spec.z))
    {
        spec = (0.0f, 0.0f, 0.0f);
    }
     
    // �Ō��albedo�Ŋ���̂�0���肪�N���Ȃ��悤�ɏC������
    albedo.x = max(0.000001f, albedo.x);
    albedo.y = max(0.000001f, albedo.y);
    albedo.z = max(0.000001f, albedo.z);    
   
    // ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
    // ���炩����������΁A�g�U���˂͎キ�Ȃ�
    return diffuse * (1.0f - smooth) + spec / albedo;
}

// �e�𗎂Ƃ����ǂ����̌v�Z
float CalcShadowPow(float isShadowReceiver, float3 worldPos)
{
    float shadowPow = 1.0f;
        
    //�V���h�E���V�[�o�[����Ȃ��Ȃ�
    if (isShadowReceiver != 1.0f)
    {
        return shadowPow;
    }
        
    for (int cascadeIndex = 0; cascadeIndex < NUM_SHADOW_MAP; cascadeIndex++)
    {
	    //���C�g�r���[�X�N���[����Ԃ���UV���W��Ԃɕϊ����Ă���
        float4 posInLVP = mul(mLVP[cascadeIndex], float4(worldPos, 1.0f));
	    //���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����
        float zInLVP = posInLVP.z / posInLVP.w;
            
        if (zInLVP >= 0.0f && zInLVP <= 1.0f)
        {
            float2 shadowMapUV = posInLVP.xy / posInLVP.w;
            shadowMapUV *= float2(0.5f, -0.5f);
            shadowMapUV += 0.5f;

	        //UV���W���g���ăV���h�E�}�b�v����e�����T���v�����O
            if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
	                && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
            {
		        //�v�Z����UV���W���g���āA�V���h�E�}�b�v����[�x�l���T���v�����O
                float2 zInshadowMap = shadowMap[cascadeIndex].Sample(Sampler, shadowMapUV).xy;
            
		        //�V���h�E�}�b�v�ɏ������܂�Ă���Z�l�Ɣ�r����
                if (zInLVP >= zInshadowMap.r + 0.001)
                {
			        //�Օ�����Ă���
                    shadowPow = 0.5f;
                }
                break;
            }
        }
    }
    
    return shadowPow;
}


