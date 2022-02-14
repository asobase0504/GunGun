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
#include "game_ui.h"
#include "game.h"
#include "common.h"
#include "player.h"
#include "input.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define SCORE_BG_TEX		"data/TEXTURE/Circle.png"

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
static Object uiLengthScoreBg;
static Object uiLengthUnitBg;
static Object uiGetModelUnitBg[2] = {};
static Object uiOperationDescription[4] = {};
static LPD3DXFONT s_pFont = NULL;		// �t�H���g�ւ̃|�C���^
static char s_aLength[5];
static float s_fLength;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void UninitObject(Object *object);	// �I��

//=========================================
// ������
//=========================================
void InitGameUI(void)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	Object* object;
	s_fLength = 0;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	// �����̃X�R�A�̒����P�ʂ��o���B
	{
		object = &(uiLengthUnitBg);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(170.0f, 175.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 75.0f;								// ����
		object->Width = 100.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/mm.png", &object->tex);	// �e�N�X�`���̓Ǎ�

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

			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
			InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	// �����̃X�R�A��u���ꏊ�̔w�i
	{
		object = &(uiLengthScoreBg);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(110.0f, 120.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f);	// �F
		object->Height = 250.0f;							// ����
		object->Width = 250.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// �e�N�X�`���̓Ǎ�

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

			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
			InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	// �ړ����@
	{
		object = &(uiOperationDescription[0]);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(1200.0f, 120.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 50.0f;								// ����
		object->Width = 150.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		if (IsJoyPadUse(0))
		{
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/LeftStick.png", &object->tex);	// �e�N�X�`���̓Ǎ�
		}
		else
		{
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/WASD.png", &object->tex);	// �e�N�X�`���̓Ǎ�
		}

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

			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
			InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	// �ړ�
	{
		object = &(uiOperationDescription[1]);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(1000.0f, 120.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 50.0f;								// ����
		object->Width = 150.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/move.png", &object->tex);	// �e�N�X�`���̓Ǎ�

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

			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
			InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	// �J�����ړ��̕��@
	{
		object = &(uiOperationDescription[2]);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(1200.0f, 150.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 50.0f;								// ����
		object->Width = 150.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		if (IsJoyPadUse(0))
		{
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/RightStick.png", &object->tex);	// �e�N�X�`���̓Ǎ�
		}
		else
		{
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/QE.png", &object->tex);	// �e�N�X�`���̓Ǎ�
		}

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

			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
			InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	// �J�����ړ�
	{
		object = &(uiOperationDescription[3]);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(1000.0f, 150.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 50.0f;								// ����
		object->Width = 150.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/CameraMove.png", &object->tex);	// �e�N�X�`���̓Ǎ�

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

			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
			InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	// �v���C���[�̑傫���p�t�H���g�̐���
	D3DXCreateFont(GetDevice(), 100, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07����������|�b�v Heavy", &s_pFont);
}

//=========================================
// �I��
//=========================================
void UninitGameUI(void)
{
	UninitObject(&uiLengthScoreBg);
	UninitObject(&uiLengthUnitBg);
	UninitObject(&uiOperationDescription[0]);
	UninitObject(&uiOperationDescription[1]);
	UninitObject(&uiOperationDescription[2]);
	UninitObject(&uiOperationDescription[3]);
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
void UpdateGameUI(void)
{
	Player* player = GetPlayer();
	if (player->fLength < 0.001f)
	{
		SetGameUITex("data/TEXTURE/WORD/mm.png");
	}
	if (player->fLength < 0.001f &&player->fLength < 1.0f)
	{
		SetGameUITex("data/TEXTURE/WORD/cm.png");
	}
	if (player->fLength > 1.0 && player->fLength < 1000.0f)
	{
		SetGameUITex("data/TEXTURE/WORD/m.png");
	}
	if (player->fLength >= 1000.0f)
	{
		SetGameUITex("data/TEXTURE/WORD/km.png");
	}

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

	if (s_fLength >= 100.0f)
	{
		sprintf(s_aLength, "%.0f", s_fLength);
	}
	else
	{
		sprintf(s_aLength, "%.1f", s_fLength);
	}
}

//=========================================
// �`��
//=========================================
void DrawGameUI(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Object* object;

	object = &(uiLengthScoreBg);
	if (object->bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiLengthUnitBg);
	if (object->bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiOperationDescription[0]);
	if (object->bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiOperationDescription[1]);
	if (object->bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiOperationDescription[2]);
	if (object->bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiOperationDescription[3]);
	if (object->bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, object->tex, 0);
	}

	// �\���̈�̍쐬
	RECT rect = { 20,45,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �e�L�X�g�̕`��
	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//=========================================
// �e�N�X�`���̐؂�ւ�
//=========================================
void SetGameUITex(char * file)
{
	Object* object = &(uiLengthUnitBg);
	// �e�N�X�`���̔j��
	if (object->tex != NULL)
	{
		object->tex->Release();
		object->tex = NULL;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	D3DXCreateTextureFromFile(pDevice, file, &object->tex);	// �e�N�X�`���̓Ǎ�
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
