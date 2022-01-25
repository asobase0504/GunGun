//=========================================
//
// ���U���g����
// Author:Kishimoto Eiji
// Author:Yuda Kaito
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "result.h"
#include "input.h"
#include "common.h"
#include "fade.h"
// 2D

// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "mesh_sky.h"
#include "mesh_field.h"


//------------------------------------
// �}�N����`
//------------------------------------
#define FILENAME	("data/TEXTURE/Result_Hunter.png")

//=========================================
// �ÓI�ϐ��ϐ�
//=========================================
static LPDIRECT3DTEXTURE9 s_pTexture[GAME_MAX] = { NULL,NULL };		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTextureWinner = NULL;					// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
static bool bWin;

//=========================================
// ������
//=========================================
void InitResult(void)
{
	// ����������
	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitModel();		// ���f��
	InitMeshSky();		// ���b�V��(��)
	InitMeshField();	// ���b�V��(�n��)
}

//=========================================
// ���U���g�̏I������
//=========================================
void UninitResult(void)
{
	// �I������
	UninitCamera();		// �J����
	UninitLight();		// ���C�g
	UninitModel();		// ���f��
	UninitMeshSky();	// ���b�V��(��)
	UninitMeshField();	// ���b�V��(�n��)
}

//=========================================
// ���U���g�̍X�V����
//=========================================
void UpdateResult(void)
{
	// �X�V����
	UpdateCamera();		// �J����
	UpdateLight();		// ���C�g
	UpdateModel();		// ���f��
	UpdateMeshSky();	// ���b�V��(��)
	UpdateMeshField();	// ���b�V��(�n��)

	if (GetFade() == FADE_NONE && (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN)))
	{
		SetFade(MODE_TITLE);	 // �^�C�g����ʂɈڍs
	}
}

//=========================================
// ���U���g�̕`�揈��
//=========================================
void DrawResult(void)
{
	// �`�揈��
	DrawModel();		// ���f��
	DrawMeshSky();		// ���b�V��(��)
	DrawMeshField();	// ���b�V��(�n��)
}
