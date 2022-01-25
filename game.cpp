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

	// �^�C��
	InitTimer();
	StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	// �|�[�Y�̏���������
	InitPause();

	// �|���S���̏���������
	InitPolygon();

	// �v���C���[�̏���������
	InitPlayer();

	// �e�̏���������
	InitShadow();

	// ���f���̏���������
	InitModel();

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �r���{�[�h�̏���������
	InitBillboard();

	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���b�V���̏���������
	InitMeshField();

	// ���b�V��(�~��)�̏���������
	InitMeshCylinder();

	// ���b�V��(��)�̏���������
	InitMeshSphere();

	// ���b�V��(��)�̏���������
	InitMeshSky();

	// �ǂ̏���������
	InitWall();

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
	// �^�C��
	UninitTimer();
	BreakTimer(0);	// �^�C�}�[�̔j��

	// �|�[�Y�̏I������
	UninitPause();

	// �|���S���̏I������
	UninitPolygon();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���f���̏I������
	UninitModel();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �e�̏I������
	UninitShadow();

	// �r���{�[�h�̏I������
	UninitBillboard();

	// ���b�V���̏I������
	UninitMeshField();

	// ���b�V��(�~��)�̏I������
	UninitMeshCylinder();

	// ���b�V��(��)�̏I������
	UninitMeshSphere();

	// ���b�V��(��)�̏I������
	UninitMeshSky();

	// �ǂ̏I������
	UninitWall();

#ifdef _DEBUG
	// ���C���̍X�V����
	UninitLine();
#endif // !_DEBUG
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
		// �|�[�Y
		UpdatePause();
	}
	else
	{
		// ���f���̍X�V
		UpdateModel();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �J�����̍X�V
		UpdateCamera();

		// ���C�g�̍X�V
		UpdateLight();

		// �|���S���̍X�V
		UpdatePolygon();

		// �e�̍X�V
		UpdateShadow();

		// �r���{�[�h�̍X�V
		UpdateBillboard();

		// ���b�V���̍X�V����
		UpdateMeshField();

		// ���b�V��(�~��)�̍X�V����
		UpdateMeshCylinder();

		// ���b�V��(��)�̍X�V����
		UpdateMeshSphere();

		// ���b�V��(��)�̍X�V����
		UpdateMeshSky();

		// �ǂ̍X�V
		UpdateWall();

		// �^�C��
		UpdateTimer();
	}

#ifdef _DEBUG
	// ���C���̍X�V����
	UpdateLine();
#endif // !_DEBUG

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
	// �J�����̐ݒ菈��
	SetCamera();

	// �|���S���̕`�揈��
	//DrawPolygon();

	// �ǂ̕`�揈��
	DrawWall();

	// ���f���̕`�揈��
	DrawModel();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// ���b�V���̕`�揈��
	DrawMeshField();

	//// ���b�V��(�~��)�̕`�揈��
	//DrawMeshCylinder();

	//// ���b�V��(��)�̕`�揈��
	//DrawMeshSphere();

	//// ���b�V��(��)�̕`�揈��
	//DrawMeshSky();

	// �e�̕`�揈��
	DrawShadow();

	// ���f���̕`�揈��
	//DrawModel();

	//// �r���{�[�h�̕`�揈��
	//DrawBillboard();

	// �^�C��
	DrawTimer();

#ifdef _DEBUG
	// ���C���̕`�揈��
	DrawLine();
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
