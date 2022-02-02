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
static int	s_nCnt;
static bool	s_bFadeCheek;	// �t�F�[�h���u�Ɉڍs���邩�̏���
static int	s_nFadeCnt;		// �t�F�[�h�����ɍs���܂ł̊Ԋu
static bool s_bExit;		// Exit�̃t���O

//=========================================
// ����������
//=========================================
void InitTitle(void)
{
	s_nCnt = 0;

	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitPolygon();		// �|���S��
	InitMeshField();	// ���b�V���t�B�[���h
	InitModel();		// ���f��
	InitPlayer();		// �v���C���[
	DeleteModel();		// �v���C���[�ȊO�̃��f���̏���

	// �|���S���̐ݒ菈��
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, -25.0f), &ZERO_VECTOR, D3DXVECTOR3(25.0f, 0.0f, 12.5f), "data/TEXTURE/WORD/Exit.jpg","exit");
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, -25.0f), &ZERO_VECTOR, D3DXVECTOR3(25.0f, 0.0f, 12.5f), "data/TEXTURE/WORD/Start.jpg", "start");

	// �v���C���[�̐ݒ菈��
	Player* player = GetPlayer();
	player->pos = D3DXVECTOR3(0.0f, 0.0f, -25.0f);

	// ���b�V���t�B�[���h�̐ݒ菈��
	SetMesh setMesh;
	setMesh.file = NULL;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 30;
	setMesh.nSurfaceWidth = 30;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);

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
	// �v���C���[���w�肳�ꂽ�|���S���ɏ���Ă�����J�E���g��i�߂�
	if (CollisionPolygon(&GetPlayer()->pos, "start") && s_nCnt < 50)
	{
		s_Select = SELECT_GAMESTART;
		s_nCnt++;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "exit") && s_nCnt < 50)
	{
		s_Select = SELECT_EXIT;
		s_nCnt++;
	}
	else if(s_nCnt < 50)
	{
		s_nCnt = 0;
	}

	// ���ȏ�̒l�ɂȂ��������Ă������ڂɈȍ~����
	if (s_nCnt >= 50)
	{
		switch (s_Select)
		{
		case SELECT_GAMESTART:
			SetFade(MODE_GAME);	// �Q�[�����[�h�Ɉڍs
			break;
		case SELECT_TUTORIAL:
			SetFade(MODE_TUTORIAL);	// �`���[�g���A����ʂɈڍs
			break;
		case SELECT_EXIT:
			s_bExit = true;
			break;
		default:
			break;
		}
	}
	else
	{
		UpdateCamera();		// �J����
		UpdateLight();		// ���C�g
		UpdatePolygon();	// �|���S��
		UpdateMeshField();	// ���b�V���t�B�[���h
		UpdateModel();		// ���f��
		UpdatePlayer();		// �v���C���[

		// �v���C���[�̈ړ�����
		Player* player = GetPlayer();
		if (player->pos.x < -130.0f)
		{
			player->pos.x = 120.0f;
		}
		else if (player->pos.x > 130.0f)
		{
			player->pos.x = -120.0f;
		}
		if (player->pos.z < -80.0f)
		{
			player->pos.z = 70.0f;
		}
		else if (player->pos.z > 80.0f)
		{
			player->pos.z = -70.0f;
		}
	}
}

//=========================================
// �I������
//=========================================
void SelectTitle(void)
{

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
