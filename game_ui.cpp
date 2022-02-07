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

//------------------------------------
// �}�N����`
//------------------------------------
#define SCORE_BG_TEX		"data/TEXTURE/Circle.png"
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
static Object uiLengthScoreBg;
static Object uiLengthUnitBg;
static Object uiGetModelUnitBg[2] = {};

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	// �����̃X�R�A��u���ꏊ�̔w�i
	object = &(uiLengthScoreBg);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 120.0f, 0.0f);	// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
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

	// �����̃X�R�A�̒����P�ʂ��o���B
	object = &(uiLengthUnitBg);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(170.0f, 170.0f, 0.0f);	// �ʒu
	object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �F
	object->Height = 150.0f;							// ����
	object->Width = 150.0f;								// ��
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

	// �����̃X�R�A��u���ꏊ�̔w�i
	object = &(uiLengthScoreBg);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 120.0f, 0.0f);	// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
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

	//// ��ɓ��ꂽ���f����u���ꏊ�̔w�i
	//object = &(uiGetModelUnitBg[0]);
	//ZeroMemory(object, sizeof(object));
	//object->pos = D3DXVECTOR3(110.0f, 500.0f, 0.0f);	// �ʒu
	//object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);	// �F
	//object->Height = 200.0f;							// ����
	//object->Width = 200.0f;								// ��
	//object->bUse = true;								// �g�p�ɐ؂�ւ�

	//D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// �e�N�X�`���̓Ǎ�

	//// ���_�o�b�t�@�̐���
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&object->vtxBuff,
	//	NULL);

	//// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//{
	//	float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
	//	float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

	//	SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
	//	SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
	//	InitRectRhw(pVtx);														// rhw�̐ݒ�
	//	InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
	//}
	//// ���_�o�b�t�@���A�����b�N����
	//object->vtxBuff->Unlock();

	//// ��ɓ��ꂽ���f����u���ꏊ�̔w�i
	//object = &(uiGetModelUnitBg[0]);
	//ZeroMemory(object, sizeof(object));
	//object->pos = D3DXVECTOR3(110.0f, 500.0f, 30.0f);	// �ʒu
	//object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);	// �F
	//object->Height = 150.0f;							// ����
	//object->Width = 150.0f;								// ��
	//object->bUse = true;								// �g�p�ɐ؂�ւ�

	//D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// �e�N�X�`���̓Ǎ�

	//																// ���_�o�b�t�@�̐���
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&object->vtxBuff,
	//	NULL);

	//// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//{
	//	float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
	//	float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

	//	SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
	//	SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
	//	InitRectRhw(pVtx);														// rhw�̐ݒ�
	//	InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
	//}
	//// ���_�o�b�t�@���A�����b�N����
	//object->vtxBuff->Unlock();

	//// ��ɓ��ꂽ���f����u���ꏊ�̔w�i
	//object = &(uiGetModelUnitBg[1]);
	//ZeroMemory(object, sizeof(object));
	//object->pos = D3DXVECTOR3(110.0f, 500.0f, 30.0f);	// �ʒu
	//object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);	// �F
	//object->Height = 200.0f;							// ����
	//object->Width = 200.0f;								// ��
	//object->bUse = true;								// �g�p�ɐ؂�ւ�

	//D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// �e�N�X�`���̓Ǎ�

	//// ���_�o�b�t�@�̐���
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&object->vtxBuff,
	//	NULL);

	//// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//{
	//	float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
	//	float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

	//	SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
	//	SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
	//	InitRectRhw(pVtx);														// rhw�̐ݒ�
	//	InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
	//}
	//// ���_�o�b�t�@���A�����b�N����
	//object->vtxBuff->Unlock();
}

//=========================================
// �I��
//=========================================
void UninitGameUI(void)
{
	UninitObject(&uiLengthScoreBg);
	UninitObject(&uiLengthUnitBg);
	//UninitObject(&uiGetModelUnitBg[0]);
	//UninitObject(&uiGetModelUnitBg[1]);
}

//=========================================
// �X�V
//=========================================
void UpdateGameUI(void)
{
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

	//object = &(uiGetModelUnitBg[0]);
	//if (object->bUse)
	//{
	//	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	//	pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

	//	// ���_�t�H�[�}�b�g�̐ݒ�
	//	pDevice->SetFVF(FVF_VERTEX_2D);

	//	// �|���S���`��
	//	// �e�N�X�`���̐ݒ�
	//	RectDraw(pDevice, object->tex, 0);
	//}

	//object = &(uiGetModelUnitBg[1]);
	//if (object->bUse)
	//{
	//	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	//	pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

	//	// ���_�t�H�[�}�b�g�̐ݒ�
	//	pDevice->SetFVF(FVF_VERTEX_2D);

	//	// �|���S���`��
	//	// �e�N�X�`���̐ݒ�
	//	RectDraw(pDevice, object->tex, 0);
	//}
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
