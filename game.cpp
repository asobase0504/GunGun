//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
#include "game.h"
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "player.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "mesh_field.h"
#include "mesh_cylinder.h"
#include "mesh_sphere.h"
#include "mesh_sky.h"
#include "line.h"
#include "pause.h"
#include "timer.h"
#include "fade.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;			//�|�[�Y�����ǂ���

//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_bPause = false;
#ifdef _DEBUG
	// ���C���̏���������
	InitLine();
#endif // !_DEBUG

	// ������
	InitTimer();		// �^�C��
	InitPause();		// �|�[�Y
	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitPolygon();		// �|���S��
	InitModel();		// ���f��
	InitPlayer();		// �v���C���[
	InitShadow();		// �e
	InitBillboard();	// �r���{�[�h
	InitMeshField();	// ���b�V��
	InitMeshCylinder();	// ���b�V��(�~��)
	InitMeshSphere();	// ���b�V��(��)
	InitMeshSky();		// ���b�V��(��)
	InitWall();			// ��

	// �^�C���̐ݒ菈��
	StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	// �r���{�[�h�̐ݒ菈��
	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//// �ǂ̐ݒ菈��
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	// �I��
	UninitTimer();			// �^�C��
	UninitModel();			// ���f��
	UninitPlayer();			// �v���C���[
	UninitPause();			// �|�[�Y
	UninitPolygon();		// �|���S��
	UninitCamera();			// �J����
	UninitLight();			// ���C�g
	UninitShadow();			// �e
	UninitBillboard();		// �r���{�[�h
	UninitMeshField();		// ���b�V��
	UninitMeshCylinder();	// ���b�V��(�~��)
	UninitMeshSphere();		// ���b�V��(��)
	UninitMeshSky();		// ���b�V��(��)
	UninitWall();			// ��

#ifdef _DEBUG
	// ���C��
	UninitLine();
#endif // !_DEBUG

	// �^�C�}�[�̔j��
	BreakTimer(0);
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	if (GetJoypadTrigger(JOYKEY_START))
	{
		s_bPause = !s_bPause;
	}

	if (s_bPause)
	{
		UpdatePause();	// �|�[�Y
	}
	else
	{
		// �X�V
		UpdateModel();			// ���f��
		UpdatePlayer();			// �v���C���[
		UpdateCamera();			// �J����
		UpdateLight();			// ���C�g
		UpdatePolygon();		// �|���S��
		UpdateShadow();			// �e
		UpdateBillboard();		// �r���{�[�h
		UpdateMeshField();		// ���b�V��
		UpdateMeshCylinder();	// ���b�V��(�~��)
		UpdateMeshSphere();		// ���b�V��(��)
		UpdateMeshSky();		// ���b�V��(��)
		UpdateWall();			// ��
		UpdateTimer();			// �^�C��
#ifdef _DEBUG
		UpdateLine();	// ���C��
#endif // !_DEBUG
	}

	// ���Ԃ��؂ꂽ�烊�U���g�Ɉȍ~
	if (TimerUp(0) == 0 || GetJoypadTrigger(JOYKEY_X))
	{
		SetFade(MODE_RESULT);
	}
}

//=========================================
// �`��
//=========================================
void DrawGame(void)
{
	SetCamera();			// �J����
	//DrawPolygon();		// �|���S��
	DrawWall();				// ��
	DrawModel();			// ���f��
	DrawPlayer();			// �v���C���[
	DrawMeshField();		// ���b�V��
	//DrawMeshCylinder();	// ���b�V��(�~��)
	//DrawMeshSphere();		// ���b�V��(��)
	//DrawMeshSky();		// ���b�V��(��)
	DrawShadow();			// �e
	//DrawModel();			// ���f��
	//DrawBillboard();		// �r���{�[�h

	// �^�C��
	DrawTimer();

#ifdef _DEBUG
	DrawLine();	// ���C��
#endif // !_DEBUG

	if (s_bPause)
	{
		DrawPause();
	}
}

//=========================================
// �|�[�Y�̐ݒ菈��
//=========================================
void SetEnablePause(bool bUse)
{
	s_bPause = bUse;
}

//=========================================
// �|�[�Y�̏�Ԃ��擾
//=========================================
bool GetPause(void)
{
	return s_bPause;
}
