//=========================================
// 
// ���C�g�̍쐬
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "light.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define LIGTH_MAX	(5)

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static D3DLIGHT9 s_light[LIGTH_MAX];	//���C�g���

//=========================================
// ������
//=========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;	// ���C�g�̕����x�N�g��

	// ���C�g���N���A����
	ZeroMemory(&s_light[0], sizeof(D3DLIGHT9));	// �\���̕ϐ����[���N���A�ł���֐�

	// ���C�g�̎�ނ�ݒ�
	s_light[0].Type = D3DLIGHT_DIRECTIONAL;		// ���s����

	// ���C�g�̊g�U����ݒ�
	s_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���C�g�̐F

	// ���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������(�傫���P�̃x�N�g���ɂ���)
	s_light[0].Direction = vecDir;

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &s_light[0]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, true);

	// ���C�g�̎�ނ�ݒ�
	s_light[1].Type = D3DLIGHT_DIRECTIONAL;		// ���s����

	// ���C�g�̊g�U����ݒ�
	s_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// ���C�g�̐F

	// ���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// ���K������(�傫���P�̃x�N�g���ɂ���)
	s_light[0].Direction = vecDir;

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &s_light[0]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, true);
}

//=========================================
// �I��
//=========================================
void UninitLight(void)
{
}

//=========================================
// �X�V
//=========================================
void UpdateLight(void)
{
}
