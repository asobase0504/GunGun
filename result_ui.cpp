//=========================================
// 
// ���U���g��UI����
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "result_ui.h"
#include "result.h"
#include "common.h"
#include <stdio.h>
#include <time.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define SELECTBG			"data/TEXTURE/number000.png"
#define MODELCOUNT_MAX		(3)	// ���f�����̃X�R�A�̍ő包��
#define SCORELENGTH_MAX		(3)	// �����X�R�A�̍ő包��

//------------------------------------
// UI�̍\����
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 vtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 tex;				// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// �p�x
	D3DXCOLOR col;						// �F
	float Height;						// ����
	float Width;						// ��
	bool bUse;							// �g�p���Ă邩�ǂ���
}Object;

//------------------------------------
// �ÓI�ϐ��錾
//------------------------------------
static Object nextButton;
static Object scoreLengthWord;
static LPD3DXFONT s_pFont = NULL;		// �t�H���g�ւ̃|�C���^
static char s_aLength[5];
static char s_aModelCnt[5];
static char s_aModelName[50];
static float s_fLength;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void UninitObject(Object *object);	// �I��

//=========================================
// ������
//=========================================
void InitResultUI(void)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	Object* object;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	srand((unsigned int)time(NULL));

	//
	// ���f���̌�
	//
	object = &(nextButton);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(270.0f, 250.0f, 0.0f);	// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	object->Height = 75.0f;								// ����
	object->Width = 475.0f;								// ��
	object->bUse = true;								// �g�p�ɐ؂�ւ�

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/ResultModel.png", &object->tex);	// �e�N�X�`���̓Ǎ�

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
		float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

		SetPosRectCenterRot(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
		SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
		InitRectRhw(pVtx);														// rhw�̐ݒ�
		InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
	}
	// ���_�o�b�t�@���A�����b�N����
	object->vtxBuff->Unlock();

	//
	// ���f���̒����̃X�R�A����
	//
	object = &(scoreLengthWord);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(1000.0f, 250.0f, 0.0f);	// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	object->Height = 75.0f;								// ����
	object->Width = 475.0f;								// ��
	object->bUse = true;								// �g�p�ɐ؂�ւ�

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/ResultLength.png", &object->tex);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
		float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

		SetPosRectCenterRot(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
		SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
		InitRectRhw(pVtx);														// rhw�̐ݒ�
		InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
	}
	// ���_�o�b�t�@���A�����b�N����
	object->vtxBuff->Unlock();

	Player* player = GetPlayer();

	// ���f���̑傫����\��
	s_fLength = player->fLength;

	if (s_fLength < 0.001f)
	{
		s_fLength *= 10000.0f;
	}
	else if (s_fLength > 0.001f && s_fLength < 1.0f)
	{
		s_fLength *= 100.0f;
	}
	else if (s_fLength > 1.0f && s_fLength < 1000.0f)
	{
	}
	else if (s_fLength >= 1000.0f)
	{
		s_fLength /= 1000.0f;
	}

	if (player->fLength < 0.001f)
	{
		sprintf(s_aLength, "%.1fmm", s_fLength);
	}
	if (player->fLength > 0.01f &&player->fLength < 1.0f)
	{
		sprintf(s_aLength, "%.1fcm", s_fLength);
	}
	if (player->fLength > 1.0f && player->fLength < 1000.0f)
	{
		if (s_fLength >= 100.0f)
		{
			sprintf(s_aLength, "%.0fm", s_fLength);
		}
		else
		{
			sprintf(s_aLength, "%.1fm", s_fLength);
		}
	}
	if (player->fLength >= (100.0f * 1000.0f))
	{
		sprintf(s_aLength, "%.1fkm", s_fLength);
	}
	sprintf(s_aModelCnt, "%03d �R", GetPlayer()->nModelCnt);

	int nRand;

	if (GetPlayer()->nModelCnt != 0)
	{

		nRand = rand() % GetPlayer()->nModelCnt;
	}
	else
	{
		nRand = GetPlayer()->nModelCnt;
	}

	for (int nModelCnt = 0; nModelCnt < MODEL_MAX; nModelCnt++)
	{
		Model* model = GetPlayerModel()[nModelCnt];
		if ((model == NULL) || (model->nIdxModelParent != 0))
		{
			continue;
		}	

		nRand--;
		if (nRand <= 0)
		{
			sprintf(s_aModelName, "%s��", model->name);
			break;
		}
	}

	// �v���C���[�̑傫���p�t�H���g�̐���
	D3DXCreateFont(GetDevice(), 100, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07����������|�b�v Heavy", &s_pFont);
}

//=========================================
// �I��
//=========================================
void UninitResultUI(void)
{
	UninitObject(&nextButton);
	UninitObject(&scoreLengthWord);

	// �f�o�b�O�\���p�t�H���g�̔j��
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}
}

//=========================================
// �X�V
//=========================================
void UpdateResultUI(void)
{
}

//=========================================
// �`��
//=========================================
void DrawResultUI(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (scoreLengthWord.bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, scoreLengthWord.vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, scoreLengthWord.tex, 0);
	}

	if (nextButton.bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, nextButton.vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, nextButton.tex, 0);
	}

	// �\���̈�̍쐬
	RECT rect = { 160,300,SCREEN_WIDTH,SCREEN_HEIGHT };
	// �e�L�X�g�̕`��
	s_pFont->DrawText(NULL, s_aModelCnt, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// �\���̈�̍쐬
	rect = { 900,300,SCREEN_WIDTH,SCREEN_HEIGHT };
	// �e�L�X�g�̕`��
	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// �\���̈�̍쐬
	rect = { 0,550,SCREEN_WIDTH,SCREEN_HEIGHT };
	// �e�L�X�g�̕`��
	s_pFont->DrawText(NULL, s_aModelName, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//=========================================
// �I�u�W�F�N�g�̏I��
//=========================================
void UninitObject(Object * object)
{
	// �e�N�X�`���̔j��
	if (object->tex != NULL)
	{
		object->tex->Release();
		object->tex = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (object->vtxBuff != NULL)
	{
		object->vtxBuff->Release();
		object->vtxBuff = NULL;
	}
}