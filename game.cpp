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
#include "particle.h"
#include "sound.h"
#include "shadow.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define MESH_FIELD	"data/TEXTURE/kusa.jpg"
#define WORLD_EDGE	(1300.0f)

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;				// �|�[�Y�����ǂ���
static bool s_bDebug;				// �f�o�b�O���[�h���ǂ���
static bool s_bCountDownTime;		// �J�E���g�_�E�������ۂ� 
static int nDelayCnt;				// �x�����̃J�E���g
static bool s_bGame;				// �Q�[�������Q�[�����n�܂�O��
static bool s_bEndGame;				// �Q�[�����I��
static int s_nEndCnt;				// �Q�[�����I������܂ł̎���
static bool s_bAssat;
static LPD3DXFONT pFont;

//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_bPause = false;
	s_bDebug = false;
	s_bCountDownTime = false;
	s_bGame = false;
	s_bEndGame = false;
	s_bAssat = false;
	s_nEndCnt = 0;
	nDelayCnt = 0;

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
	InitShadow();		// �e
	InitPlayer();		// �v���C���[
	InitMeshField();	// ���b�V��
	InitGameUI();		// UI
	InitMeshSky();		// ���b�V���X�J�C
	InitParticle();		// �p�[�e�B�N��

	// ���b�V���X�J�C�̐ݒ菈��
	SetMeshSky();

	// �^�C���̐ݒ菈��
	StartTimer(3, 0, 40.0f, 180.0f, D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2.0f - 90.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	// �|���S���̐ݒ菈��
	SetPolygon(&D3DXVECTOR3(0.0f, -200.0f, 0.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), &D3DXVECTOR3(100.0f, 0.0f, 100.0f),&D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), NULL, "floar");
	SetPolygonUI(&D3DXVECTOR3(-25.5f, -10.5f, 30.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 0.0f, 5.0f), "data/TEXTURE/Circle.png");

	// ���b�V���t�B�[���h�̐ݒ菈��
	SetMesh setMesh;
	setMesh.file = MESH_FIELD;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 75;
	setMesh.nSurfaceWidth = 75;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);

	// BGM�̍Đ�
	PlaySound(SOUND_LABEL_BGM_GAME);

	// �t�H���g�̏�����
	D3DXCreateFont(GetDevice(), 140, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07����������|�b�v Heavy", &pFont);
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	StopSound();	// ���̍Đ���S�Ă��~

	// �I��
	UninitTimer();			// �^�C��
	UninitPause();			// �|�[�Y
	UninitPolygon();		// �|���S��
	UninitCamera();			// �J����
	UninitLight();			// ���C�g
	UninitShadow();			// �e
	UninitMeshField();		// ���b�V��
	UninitMeshSky();		// ���b�V��
	UninitGameUI();			// UI
	UninitParticle();		// �p�[�e�B�N��

#ifdef _DEBUG
	// ���C��
	UninitLine();
#endif // !_DEBUG

	// �^�C�}�[�̔j��
	BreakTimer(0);
	BreakTimer(1);

	// �f�o�b�O�\���p�t�H���g�̔j��
	if (pFont != NULL)
	{
		pFont->Release();
		pFont = NULL;
	}
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	// �|�[�Y�̋@�\
	if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardTrigger(DIK_P))
	{
		s_bPause = !s_bPause;
	}

	// �|�[�Y���Ȃ�|�[�Y�ȊO���X�V���Ȃ�
	if (s_bPause)
	{
		UpdatePause();		// �|�[�Y
	}
	else
	{
		UpdateTimer();			// �^�C��
		UpdateGameCamera();		// �J����

		// �J�E���g�_�E������
		if (!TimerUp(0) && GetTimer(0)->bUse && !GetTimer(1)->bUse)
		{
			return;
		}

		// �Q�[���X�^�[�g���̒x��
		if (nDelayCnt < 30)
		{
			nDelayCnt++;
			return;
		}

		if (!s_bGame)
		{
			StartTimer(10, 1, 25.0f, 100.0f, D3DXVECTOR3(0.0f, 00.0f, 0.0f), 0);
			// �^�C�}�[�̔j��
			BreakTimer(0);
			s_bCountDownTime = true;
			s_bGame = true;
		}

		if (GetTimer(1)->nSecond == 25 && !s_bAssat)
		{
			PlaySound(SOUND_LABEL_SE_TIME);
			s_bAssat = true;
		}

		if (GetTimer(1)->nSecond == 5 && !GetTimer(0)->bUse)
		{
			StartTimer(5, 0, 40.0f, 180.0f, D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2.0f - 90.0f, 0.0f), 0);
		}

		// ���Ԃ��؂ꂽ�烊�U���g�Ɉȍ~
		if (TimerUp(1))
		{
			BreakTimer(1);
			BreakTimer(0);
			PlaySound(SOUND_LABEL_SE_ENDGAME);
			s_bEndGame = true;
		}

		if (s_bEndGame)
		{
			s_nEndCnt++;

			if (s_nEndCnt >= 40)
			{
				GetPlayer()->pos.y += 1.0f;
			}
			if (s_nEndCnt >= 120)
			{
				SetFade(MODE_RESULT);
			}
		}
		else
		{
			// �X�V
			UpdateModel();			// ���f��
			UpdatePlayer();			// �v���C���[
			UpdateLight();			// ���C�g
			UpdatePolygon();		// �|���S��
			UpdateShadow();			// �e
			UpdateMeshField();		// ���b�V��
			UpdateMeshSky();		// ���b�V���X�J�C
			UpdateGameUI();			// UI
			UpdateParticle();		// �p�[�e�B�N��

#ifndef _GETMODEL_POP
			SetModelUI(SetJustModel());
#endif // !_GETMODEL_POP

		}

#ifdef _DEBUG
		UpdateLine();	// ���C��

						// ���U���g��ʂɈړ�
		if (GetJoypadTrigger(JOYKEY_X))
		{
			SetFade(MODE_RESULT);
		}
#endif // !_DEBUG

		// ���E�̒[
		Player* player = GetPlayer();
		if (player->pos.x + player->fLength >= WORLD_EDGE)
		{
			player->pos.x = WORLD_EDGE - player->fLength;
		}
		if (player->pos.x - player->fLength <= -WORLD_EDGE)
		{
			player->pos.x = -WORLD_EDGE + player->fLength;
		}
		if (player->pos.z + player->fLength >= WORLD_EDGE)
		{
			player->pos.z = WORLD_EDGE - player->fLength;
		}
		if (player->pos.z - player->fLength <= -WORLD_EDGE)
		{
			player->pos.z = -WORLD_EDGE + player->fLength;
		}
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

		DrawModel();		// ���f��
		DrawPlayer();		// �v���C���[
		DrawMeshField();	// ���b�V��
		DrawMeshSky();		// ���b�V���X�J�C
		DrawShadow();		// �e

		if (s_bGame)
		{
			DrawModelUI();		// ���f��UI
			DrawParticle();		// �p�[�e�B�N��
			DrawGameUI();		// UI
			DrawPolygonUI();	// �|���S��UI
		}

		//DrawTimer();		// �^�C��

		if (s_bPause)
		{
			DrawPause();
		}

		if (s_bEndGame)
		{
			RECT rect = { (LONG)0.0f,(LONG)(SCREEN_HEIGHT / 2.0f - 90.0f),(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };

			pFont->DrawText(NULL, "���イ��傤�`�`�`�I�I", -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

#ifdef _DEBUG
		DrawLine();		// ���C��
		//DrawFPS();	// FPS�̕\��
#endif // !_DEBUG
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
