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
#include "timer.h"
#include "camera.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define SCORE_BG_TEX		"data/TEXTURE/Circle_01.png"
#define MAX_FAN_VERTEX		(360)		// ��̒��_��

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
static Object uiTimeBg;
static Object uiTimeGauge;
static Object uiLengthUnitBg;
static Object uiGetModelBg;
static Object uiGetModelGauge;
static LPD3DXFONT s_pFont = NULL;		// �t�H���g�ւ̃|�C���^
static char s_aLength[5];
static char s_aLv[5];
static float s_fLength;
static int s_nFanCnt;					// ��̊J���
static int s_nFanSwitch;
static bool s_bFanMax;

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
	s_nFanSwitch = 0;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	// �����̃X�R�A�̒����P�ʂ��o���B
	object = &(uiLengthUnitBg);
	{
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH - 65.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 168.0f * 0.25f;					// ����
		object->Width = 417.0f * 0.25f;						// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/mm.png", &object->tex);	// �e�N�X�`���̓Ǎ�

		SetVtxBuff2D(&object->vtxBuff, 4);	// ���_�o�b�t�@�̐���

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
	}

	// �����̃X�R�A��u���ꏊ�̔w�i
	object = &(uiLengthScoreBg);
	{
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT + 25.0f, 0.0f);	// �ʒu
		object->col = D3DXCOLOR(0.0f, 0.9f, 0.0f, 1.0f);	// �F
		object->Height = 600.0f;							// ����
		object->Width = 600.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// �e�N�X�`���̓Ǎ�

		SetVtxBuff2D(&object->vtxBuff, 4);	// ���_�o�b�t�@�̐���

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
	}

	// �^�C�}�[�w�i
	object = &(uiTimeBg);
	{
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 00.0f, 0.0f);	// �ʒu
		object->rot = ZERO_VECTOR;	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		object->Height = 270.0f;							// ����
		object->Width = 270.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor.png", &object->tex);	// �e�N�X�`���̓Ǎ�

		SetVtxBuff2D(&object->vtxBuff, 4);	// ���_�o�b�t�@�̐���

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
	}

	// �^�C�}�[�̃Q�[�W
	object = &(uiTimeGauge);
	{
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f);	// �ʒu
		object->rot = ZERO_VECTOR;	// �ʒu
		object->rot.z = D3DX_PI * 0.5f;	// �ʒu
		object->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	// �F
		object->Height = 200.0f;							// ����
		object->Width = 200.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor_03.png", &object->tex);	// �e�N�X�`���̓Ǎ�

		SetVtxBuff2D(&object->vtxBuff, 4);	// ���_�o�b�t�@�̐���

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
	}

	// �擾���f���̔w�i
	object = &(uiGetModelBg);
	{
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(120.0f, SCREEN_HEIGHT - 125.0f, 0.0f);	// �ʒu
		object->rot = ZERO_VECTOR;							// �ʒu
		object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �F
		object->Height = 200.0f;							// ����
		object->Width = 200.0f;								// ��
		object->bUse = true;								// �g�p�ɐ؂�ւ�

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Circle.png", &object->tex);	// �e�N�X�`���̓Ǎ�

		SetVtxBuff2D(&object->vtxBuff, 4);	// ���_�o�b�t�@�̐���

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
	}

	//// �擾���f���̃Q�[�W
	//object = &(uiGetModelGauge);
	//{
	//	ZeroMemory(object, sizeof(object));
	//	object->pos = D3DXVECTOR3(120.0f, SCREEN_HEIGHT - 122.5f, 0.0f);	// �ʒu
	//	object->rot = ZERO_VECTOR;
	//	object->col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);	// �F
	//	object->Height = 105.0f;							// ����
	//	object->Width = 200.0f;								// ��
	//	object->bUse = true;								// �g�p�ɐ؂�ւ�
	//	s_bFanMax = false;
	//	s_nFanCnt = 0;

	//	object->tex = NULL;

	//	SetVtxBuff2D(&object->vtxBuff, MAX_FAN_VERTEX);	// ���_�o�b�t�@�̐���

	//	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	pVtx[0].pos = object->pos;					// ���_���W�̐ݒ�
	//	pVtx[0].rhw = 1.0f;							// rhw�̐ݒ�											
	//	pVtx[0].col = object->col;					// ���_�J���[�̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// �e�N�X�`�����W�̐ݒ�

	//	// �f�o�b�N�p
	//	DEBUG_PRINT("%f	%f\n", pVtx[0].pos.x, pVtx[0].pos.y);

	//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//	for (int i = 1; i < MAX_FAN_VERTEX; i++)
	//	{
	//		//float fRot = D3DXToRadian((360.0f / (nVtxNum - 1 - 1)) * (i -1));

	//		float fRot = (D3DX_PI * 2.0f / (MAX_FAN_VERTEX - 2)) * (i - 1);

	//		//// �p�x�̐��K��
	//		//NormalizeRot(&fRot);

	//		// ���_���W�̐ݒ�
	//		pVtx[i].pos.x = object->pos.x + cosf(fRot - D3DX_PI / -2) * object->Height;
	//		pVtx[i].pos.y = object->pos.y + sinf(fRot - D3DX_PI / -2) * object->Height;
	//		pVtx[i].pos.z = object->pos.z;

	//		pVtx[i].rhw = 1.0f;							// rhw�̐ݒ�											
	//		pVtx[i].col = object->col;
	//		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);		// �e�N�X�`�����W�̐ݒ�
	//		DEBUG_PRINT("%f	%f\n", pVtx[i].pos.x, pVtx[i].pos.y);
	//	}
	//	// ���_�o�b�t�@���A�����b�N����
	//	object->vtxBuff->Unlock();
	//}

	// �v���C���[�̑傫���p�t�H���g�̐���
	D3DXCreateFont(GetDevice(), 130, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07����������|�b�v Heavy", &s_pFont);
}

//=========================================
// �I��
//=========================================
void UninitGameUI(void)
{
	UninitObject(&uiLengthScoreBg);
	UninitObject(&uiLengthUnitBg);
	UninitObject(&uiTimeBg);
	UninitObject(&uiTimeGauge);
	UninitObject(&uiGetModelBg);
	//UninitObject(&uiGetModelGauge);
	
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

	sprintf(s_aLv, "Lv.%d", (int)((GetPlayer()->fLength / 14.0f) + 1));

	Object* object;
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	int nSecond = GetTimer(1)->nSecond;

	object = &(uiTimeGauge);
	{
		object->rot.z = -ToRadian(nSecond * 2.0f);	// 

		if (nSecond <= 25)
		{// ���Ԑ���������ƐF�����݂ɐ؂�ւ��
			if (nSecond % 2 == 0)
			{
				object->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				object->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
		}

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		{
			float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
			float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

			SetPosRectCenterRot(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
		}
		// ���_�o�b�t�@���A�����b�N����
		object->vtxBuff->Unlock();
	}

	if (!s_bFanMax)
	{
		float fData = (float)MAX_FAN_VERTEX / 140.0f;
		float fData2 = (int)(player->fLength * 10.0f) % 140 * fData;
		s_nFanCnt = fData2;

		if (s_nFanSwitch != (int)((player->fLength * 10.0f) / 140.0f))
		{
			s_nFanSwitch = (player->fLength * 10.0f) / 140;
			s_bFanMax = true;
		}
	}
	else
	{
		s_nFanCnt--;
		if (s_nFanCnt <= 0)
		{
			s_bFanMax = false;
		}
	}

	//uiGetModelGauge.col = D3DXCOLOR(0.7f, 1.0f - (1.0f / MAX_FAN_VERTEX * s_nFanCnt), 0.0f, 1.0f);
	//// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//uiGetModelGauge.vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//for (int i = 1; i < MAX_FAN_VERTEX; i++)
	//{
	//	SetRectColor(&pVtx[i], &(uiGetModelGauge.col));										// ���_�J���[�̐ݒ�
	//}
	//// ���_�o�b�t�@���A�����b�N����
	//uiGetModelGauge.vtxBuff->Unlock();
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

	object = &(uiTimeBg);
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

	object = &(uiTimeGauge);
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

	//object = &(uiGetModelGauge);
	//if (object->bUse)
	//{
	//	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	//	pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

	//	// ���_�t�H�[�}�b�g�̐ݒ�
	//	pDevice->SetFVF(FVF_VERTEX_2D);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, object->tex);

	//	// �|���S���̕`��
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, s_nFanCnt);
	//}
	
	object = &(uiGetModelBg);
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
	RECT rect = { (LONG)950.0f,(LONG)(SCREEN_HEIGHT - 185.0f),(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	// �e�L�X�g�̕`��
	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));

	//// LV�\��
	//// �\���̈�̍쐬
	//rect = { (LONG)0.0f,(LONG)(SCREEN_HEIGHT - 185.0f),(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	//// �e�L�X�g�̕`��
	//s_pFont->DrawText(NULL, s_aLv, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
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
