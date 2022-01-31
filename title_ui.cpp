//=========================================
// 
// �^�C�g����UI����
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "title_ui.h"
#include "title.h"
#include "common.h"
#include "player.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define SELECTBG			"data/TEXTURE/number002.png"
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
static Object scoreLength;
static Object scoreModel;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
//static void InitObject(Object *object);		// ������
static void UninitObject(Object *object);	// �I��
//static void UpdateObject(Object *object);	// �X�V
//static void DrawObject(Object *object);		// �`��

//=========================================
// ������
//=========================================
void InitTitleUI(void)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	Object* object;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	//
	// ���ւ̃{�^��
	//
	object = &(nextButton);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(10.0f, 10.0f, 0.0f);		// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	object->Height = 10.0f;								// ����
	object->Width = 10.0f;								// ��
	object->bUse = true;								// �g�p�ɐ؂�ւ�

	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// �e�N�X�`���̓Ǎ�

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

	//
	// �����̃X�R�A
	//
	object = &(scoreLength);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 300.0f, 0.0f);	// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	object->Height = 60.0f;									// ����
	object->Width = 40.0f;									// ��
	object->bUse = true;									// �g�p�ɐ؂�ւ�

	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// �e�N�X�`���̓Ǎ�

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORELENGTH_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float aTex[SCORELENGTH_MAX];
		int aNumber[SCORELENGTH_MAX];
		int nStep = 0;	// �i�K���m�F
		int nLengthScore = (int)GetPlayer()->fLength;	// �X�R�A

		while(nLengthScore / 100 != 0)
		{
			nStep++;
			nLengthScore /= 100;
		}

		for (int i = 0; i < SCORELENGTH_MAX; i++)
		{
			SetRectUpRightPos(pVtx, D3DXVECTOR3(object->pos.x - 25.0f * i, object->pos.y, object->pos.z), object->Width, object->Height);
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�
	
			aNumber[i] = nLengthScore % 10;
			aTex[i] = aNumber[i] * 0.1f;
			nLengthScore /= 10;

			SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);			// �e�N�X�`�����W�̐ݒ�
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	object->vtxBuff->Unlock();

	//
	// ���f�����̃X�R�A
	//
	object = &(scoreModel);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(100.0f, 10.0f, 0.0f);		// �ʒu
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	object->Height = 100.0f;							// ����
	object->Width = 50.0f;								// ��
	object->bUse = true;								// �g�p�ɐ؂�ւ�

	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// �e�N�X�`���̓Ǎ�

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MODELCOUNT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float aTex[SCORELENGTH_MAX];
		int aNumber[SCORELENGTH_MAX];
		int nStep = 0;	// �i�K���m�F
		int nModelScore = (int)GetPlayer()->nModelCnt;	// �X�R�A

		while (nModelScore / 100 != 0)
		{
			nStep++;
			nModelScore /= 100;
		}

		for (int i = 0; i < SCORELENGTH_MAX; i++)
		{
			SetRectUpRightPos(pVtx, D3DXVECTOR3(object->pos.x - 25.0f * i, object->pos.y, object->pos.z), object->Width, object->Height);
			SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
			InitRectRhw(pVtx);														// rhw�̐ݒ�

			aNumber[i] = nModelScore % 10;
			aTex[i] = aNumber[i] * 0.1f;
			nModelScore /= 10;

			SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);			// �e�N�X�`�����W�̐ݒ�
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	object->vtxBuff->Unlock();
}

//=========================================
// �I��
//=========================================
void UninitTitleUI(void)
{
	UninitObject(&nextButton);
	UninitObject(&scoreLength);
	UninitObject(&scoreModel);
}

//=========================================
// �X�V
//=========================================
void UpdateTitleUI(void)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// �����̌���UI�̍X�V
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	scoreLength.vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_�o�b�t�@���A�����b�N����
	scoreLength.vtxBuff->Unlock();

}

//=========================================
// �`��
//=========================================
void DrawTitleUI(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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

	if (scoreLength.bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, scoreLength.vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < MODELCOUNT_MAX; i++)
		{
			// �|���S���`��
			// �e�N�X�`���̐ݒ�
			RectDraw(pDevice, scoreLength.tex, i * 4);
		}
	}

	if (scoreModel.bUse)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, scoreModel.vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SCORELENGTH_MAX; i++)
		{
			// �|���S���`��
			// �e�N�X�`���̐ݒ�
			RectDraw(pDevice, scoreModel.tex, i * 4);
		}
	}
}

////=========================================
//// �I�u�W�F�N�g�̏�����
////=========================================
//static void InitObject(Object * object)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
//
//	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// �e�N�X�`���̓Ǎ�
//
//	// ���_�o�b�t�@�̐���
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&object->vtxBuff,
//		NULL);
//
//	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
//	
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//	{
//		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
//		float fAngle = atan2f(object->Width, object->Height);	// ���S���W�����̒��_�̊p�x���Z�o����
//
//		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// ���_���W�̐ݒ�
//		SetRectColor(pVtx, &(object->col));										// ���_�J���[�̐ݒ�
//		InitRectRhw(pVtx);														// rhw�̐ݒ�
//		InitRectTex(pVtx);														// �e�N�X�`�����W�̐ݒ�
//
//	}
//	// ���_�o�b�t�@���A�����b�N����
//	object->vtxBuff->Unlock();
//
//	// �g�p�ɐ؂�ւ�
//	object->bUse = true;
//}

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

////=========================================
//// �I�u�W�F�N�g�̍X�V
////=========================================
//void UpdateObject(Object * object)
//{
//
//}

////=========================================
//// �I�u�W�F�N�g�̕`��
////=========================================
//void DrawObject(Object * object)
//{
//	// �f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
//	pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	if (object->bUse)
//	{
//		// �|���S���`��
//		// �e�N�X�`���̐ݒ�
//		RectDraw(pDevice, object->tex, 0);
//	}
//}
