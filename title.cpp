//=========================================
// 
// �^�C�g���̏���
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "title.h"
#include "input.h"
#include "key_config.h"
#include "fade.h"
#include "common.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "polygon.h"
#include "mesh_field.h"
#include "player.h"
#include "model.h"

#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)

//------------------------------------
// �v���X�G���^�[�̏�Ԃ̎��
//------------------------------------
typedef enum
{
	TYPE_NOME,	// �ω��Ȃ�
	TYPE_BLINK,	// �_�ŏ��
	TYPE_MAX
}PRESS_TYPE;

//------------------------------------
// �z�u����I�u�W�F�N�g�̎��
//------------------------------------
typedef enum
{
	OBJ_BG,				// �X�N���[���w�i
	OBJ_SELECTBG,		// �I����ʔw�i
	OBJ_TITLE,			// �^�C�g������
	OBJ_GAMESTART,		// GAMESTART����
	OBJ_TUTORIAL,		// TUTORIAL����
	OBJ_EXIT,			// EXIT����
	OBJ_SERECTCURSOR,	// �Z���N�g�J�[�\��
	OBJ_MAX				// 
}OBJ_TYPE;

//------------------------------------
// �I�����ꂽ�I�u�W�F�N�g
//------------------------------------
typedef enum
{
	SELECT_GAMESTART = OBJ_GAMESTART,
	SELECT_TUTORIAL,
	SELECT_EXIT,
}SELECT_MODE;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 Tex = NULL;				// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;							// �ʒu
	D3DXVECTOR3 rot;							// �p�x
	D3DXCOLOR col;								// �F
	bool bUse;									// �g�p���Ă邩�ǂ���
	float Height;								// ����
	float Width;								// ��
}OBJECT;

//------------------------------------
// �ÓI�ϐ��ϐ�
//------------------------------------
static SELECT_MODE s_Select;
static OBJECT s_Object[OBJ_MAX] = {};
static bool	s_bFadeCheek;	// �t�F�[�h���u�Ɉڍs���邩�̏���
static int	s_nFadeCnt;		// �t�F�[�h�����ɍs���܂ł̊Ԋu
static bool s_bExit;		// Exit�̃t���O

//=========================================
// ����������
//=========================================
void InitTitle(void)
{
	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitPolygon();		// �|���S��
	InitMeshField();	// ���b�V���t�B�[���h
	InitModel();		// ���f��
	InitPlayer();		// �v���C���[

	// �|���S���̐ݒ菈��
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, -25.0f), &ZERO_VECTOR, D3DXVECTOR3(25.0f, 0.0f, 12.5f), "data/TEXTURE/WORD/Exit.jpg");
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, -25.0f), &ZERO_VECTOR, D3DXVECTOR3(25.0f, 0.0f, 12.5f), "data/TEXTURE/WORD/Start.jpg");
}

//=========================================
// �I������
//=========================================
void UninitTitle(void)
{
	UninitCamera();		// �J����
	UninitLight();		// ���C�g
	UninitPolygon();	// �|���S��
	UninitMeshField();	// ���b�V���t�B�[���h
	UninitModel();		// ���f��
	UninitPlayer();		// �v���C���[
}

//=========================================
// �X�V����
//=========================================
void UpdateTitle(void)
{
	UpdateCamera();		// �J����
	UpdateLight();		// ���C�g
	UpdatePolygon();	// �|���S��
	UpdateMeshField();	// ���b�V���t�B�[���h
	UpdateModel();		// ���f��
	UpdatePlayer();		// �v���C���[
}

//=========================================
// �I������
//=========================================
void SelectTitle(void)
{
	// �W���C�p�b�h�̎g�p���̎擾
	bool bUseJoyPad = IsJoyPadUse(0);

	switch (s_Select)
	{
	case SELECT_GAMESTART:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_EXIT;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_TUTORIAL;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_EXIT;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_TUTORIAL;
			}
		}
		break;
	case SELECT_TUTORIAL:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_GAMESTART;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_EXIT;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_GAMESTART;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_EXIT;
			}
		}
		break;
	case SELECT_EXIT:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_TUTORIAL;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_GAMESTART;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_TUTORIAL;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_GAMESTART;
			}
		}
		break;
	default:
		break;
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawTitle(int cameraData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	if (cameraData == 0)
	{
		// �r���[�{�[�h�̃N���A
		pDevice->SetViewport(&GetCamera(cameraData)->viewport);

		// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		SetCamera(cameraData);	// �J����
		DrawPolygon();			// �|���S��
		DrawMeshField();		// ���b�V���t�B�[���h
		DrawModel();		// ���f��
		DrawPlayer();			// �v���C���[
	}
}

//=========================================
// �I�������擾
//=========================================
bool GetExit(void)
{
	return s_bExit;
}
