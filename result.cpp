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
#include "result_ui.h"
// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "mesh_sky.h"
#include "mesh_field.h"

//------------------------------------
// �}�N����`
//------------------------------------

//=========================================
// �ÓI�ϐ��ϐ�
//=========================================

//=========================================
// ������
//=========================================
void InitResult(void)
{
	// ����������
	InitResultUI();		// ���U���gUI
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
	UninitResultUI();	// ���U���gUI
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
	UpdateResultUI();	// ���U���gUI
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
	DrawResultUI();		// ���U���gUI
	DrawModel();		// ���f��
	DrawMeshSky();		// ���b�V��(��)
	DrawMeshField();	// ���b�V��(�n��)
}
