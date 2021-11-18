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
// �ÓI�ϐ�
//------------------------------------
static D3DLIGHT9 s_light;	//���C�g���

void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;	// ���C�g�̕����x�N�g��

	// ���C�g���N���A����
	ZeroMemory(&s_light, sizeof(D3DLIGHT9));	// �\���̕ϐ����[���N���A�ł���֐�

	// ���C�g�̎�ނ�ݒ�
	s_light.Type = D3DLIGHT_DIRECTIONAL;					// ���s����
	
	// ���C�g�̊g�U����ݒ�
	s_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���C�g�̐F

	// ���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	// ���K������(�傫���P�̃x�N�g���ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);
	s_light.Direction = vecDir;

	// ���C�g��ݒ肷��
	pDevice->SetLight(0, &s_light);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(0, true);
}

void UninitLight(void)
{
}

void UpdateLight(void)
{
}
