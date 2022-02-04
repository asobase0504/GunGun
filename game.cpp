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

//------------------------------------
// �}�N����`
//------------------------------------
#define MESH_FIELD	"data/TEXTURE/bee_tex.jpg"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;			// �|�[�Y�����ǂ���
static int	s_nSizeCnt;			// �傫���̋�؂��

//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_bPause = false;
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
	StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	// �|���S���̐ݒ菈��
	SetPolygon(&D3DXVECTOR3(0.0f, -200.0f, 0.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f),NULL,"floar");
	
	// ���b�V���t�B�[���h�̐ݒ菈��
	SetMesh setMesh;
	setMesh.file = "data/TEXTURE/bg001.jpg";
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
		UpdateGameCamera();		// �J����
		UpdateLight();			// ���C�g
		UpdatePolygon();		// �|���S��
		UpdateShadow();			// �e
		UpdateMeshField();		// ���b�V��
		UpdateWall();			// ��
		UpdateTimer();			// �^�C��
#ifdef _DEBUG
		UpdateLine();	// ���C��
#endif // !_DEBUG
		UpdateGameUI();			// UI

		if (GetKeyboardTrigger(DIK_T))
		{
			SetGameUITex("data/TEXTURE/WORD/mm.png");
		}
		if (GetKeyboardTrigger(DIK_Y))
		{
			SetGameUITex("data/TEXTURE/WORD/cm.png");
		}
		if (GetKeyboardTrigger(DIK_U))
		{
			SetGameUITex("data/TEXTURE/WORD/m.png");
		}
		if (GetKeyboardTrigger(DIK_I))
		{
			SetGameUITex("data/TEXTURE/WORD/km.png");
		}
	}

	if ((int)GetPlayer()->fLength / 14 == s_nSizeCnt)
	{
		GetCamera(0)->fDistance *= 1.5f;
		s_nSizeCnt++;
	}

	// ���Ԃ��؂ꂽ�烊�U���g�Ɉȍ~
	if (TimerUp(0) || GetJoypadTrigger(JOYKEY_X))
	{
		SetFade(MODE_RESULT);
	}
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

		if (s_bPause)
		{
			DrawPause();
		}
#ifdef _DEBUG
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
