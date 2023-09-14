//////�R�R����֐�/////////////////////////////////////////////////////////////////////////////////////////////////////

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

    // ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
    // ���炩����������΁A�g�U���˂͎キ�Ȃ�
    return diffuse * (1.0f - smooth) + spec / albedo;
}
