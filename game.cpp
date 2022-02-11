//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
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
#include "debug.h"
#include "game_ui.h"
#include "item.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define MESH_FIELD	"data/TEXTURE/bg001.jpg"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;			// �|�[�Y�����ǂ���
static int	s_nSizeCnt;			// �傫���̋�؂��
static bool s_bDebug;			// �f�o�b�O���[�h���ǂ���
static bool s_bCountDownTime;	// �J�E���g�_�E�������ۂ� 
static int	nDelayCnt;			// �x�����̃J�E���g

//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_bPause = false;
	s_bDebug = false;
	s_bCountDownTime = false;

	s_nSizeCnt = 1;
#ifdef _DEBUG
	// ���C���̏���������
	InitLine();
#endif // !_DEBUG

	// ������
	InitTimer();		// �^�C��
	InitPause();		// �|�[�Y
	InitPolygon();		// �|���S��
	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitModel();		// ���f��
	InitPlayer();		// �v���C���[
	InitShadow();		// �e
	InitMeshField();	// ���b�V��
	InitWall();			// ��
	InitGameUI();		// UI

	// �^�C���̐ݒ菈��
	StartTimer(3, 0, 40.0f, 80.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 0);
	CountRestartStop(true, 0);
	CountRestartStop(true, 1);

	// �|���S���̐ݒ菈��
	SetPolygon(&D3DXVECTOR3(0.0f, -200.0f, 0.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), NULL, "floar");
	SetPolygonUI(&D3DXVECTOR3(-25.5f, -10.5f, 30.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 0.0f, 5.0f), "data/TEXTURE/Circle.png");

	// ���b�V���t�B�[���h�̐ݒ菈��
	SetMesh setMesh;
	setMesh.file = MESH_FIELD;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 30;
	setMesh.nSurfaceWidth = 30;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	// �I��
	UninitTimer();			// �^�C��
	UninitPause();			// �|�[�Y
	UninitPolygon();		// �|���S��
	UninitCamera();			// �J����
	UninitLight();			// ���C�g
	UninitShadow();			// �e
	UninitMeshField();		// ���b�V��
	UninitWall();			// ��
	UninitGameUI();			// UI

#ifdef _DEBUG
	// ���C��
	UninitLine();
	
#endif // !_DEBUG

	// �^�C�}�[�̔j��
	BreakTimer(1);
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	if (!TimerUp(0) && GetTimer(0)->bUse)
	{
		UpdateTimer();			// �v���C���[
		return;
	}

	if (!s_bCountDownTime)
	{
		StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
		// �^�C�}�[�̔j��
		BreakTimer(0);
		s_bCountDownTime  = true;
	}

	if (nDelayCnt < 30)
	{
		nDelayCnt++;
		return;
	}

	if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardTrigger(DIK_P))
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

		// �f�o�b�O���[�h���ǂ���
		if (!s_bDebug)
		{
			UpdatePlayer();			// �v���C���[
		}
		else
		{
			UpdateItem();
		}

		UpdateGameCamera();		// �J����
		UpdateLight();			// ���C�g
		UpdatePolygon();		// �|���S��
		UpdateShadow();			// �e
		UpdateMeshField();		// ���b�V��
		UpdateWall();			// ��
		UpdateTimer();			// �^�C��

		UpdateGameUI();			// UI

		Player* player = GetPlayer();
		// �v���C���[����ʈ�t�ɂȂ������ʂ̊g��
		if ((int)player->fLength / 24 == s_nSizeCnt)
		{
			GetCamera(0)->fDistance *= 1.5f;
			s_nSizeCnt++;
		}

	}

	// ���Ԃ��؂ꂽ�烊�U���g�Ɉȍ~
	if (TimerUp(1))
	{
		SetFade(MODE_RESULT);
	}

#ifdef _DEBUG
	UpdateLine();	// ���C��

	// �G�f�B�^���[�h�̐؂�ւ�
	if (GetJoypadTrigger(JOYKEY_BACK))
	{
		s_bDebug = !s_bDebug;

		if (s_bDebug)
		{
			InitItem();
		}
		else
		{
			UninitItem();
		}
	}
	// ���U���g��ʂɈړ�
	if (GetJoypadTrigger(JOYKEY_X))
	{
		SetFade(MODE_RESULT);
	}
	// �}�b�v�̍X�V
	if (GetJoypadTrigger(JOYKEY_Y))
	{
		OutputMap("data/map02.txt");
	}
#endif // !_DEBUG
}

//=========================================
// �`��
//=========================================
void DrawGame(int cameraData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
												
	switch (cameraData)
	{
	case 0:
		// �r���[�{�[�h�̃N���A
		pDevice->SetViewport(&GetCamera(cameraData)->viewport);

		// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		SetCamera(cameraData);			// �J����

		DrawWall();			// ��
		DrawModel();		// ���f��
		DrawPlayer();		// �v���C���[
		DrawMeshField();	// ���b�V��
		DrawShadow();		// �e
		DrawModelUI();		// ���f��UI
		DrawGameUI();		// UI
		DrawPolygonUI();	// �|���S��UI

		if (s_bPause)
		{
			DrawPause();
		}

#ifdef _DEBUG
		if (s_bDebug)
		{
			DrawItem();
		}

		DrawLine();		// ���C��
		DrawFPS();		// FPS�̕\��
#endif // !_DEBUG

		DrawTimer();		// �^�C��
		break;
	//case 1:
	//	DrawPolygon();
	//	break;
	default:
		break;
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
