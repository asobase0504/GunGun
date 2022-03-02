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
#include "ui.h"
#include "common.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define SELECTBG			"data/TEXTURE/number002.png"
#define MODELCOUNT_MAX		(3)	// ���f�����̃X�R�A�̍ő包��
#define SCORELENGTH_MAX		(3)	// �����X�R�A�̍ő包��
#define MAX_UI				(255)

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
	int digit;							// ����
	float Height;						// ����
	float Width;						// ��
	bool bUse;							// �g�p���Ă邩�ǂ���
}Object;

//------------------------------------
// �ÓI�ϐ��錾
//------------------------------------
static Object objectUI[MAX_UI];

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------

//=========================================
// ������
//=========================================
void InitUI(void)
{
	ZeroMemory(objectUI, sizeof(objectUI));
}

//=========================================
// �I��
//=========================================
void UninitUI(void)
{
	for (int i = 0; i < MAX_UI; i++)
	{
		Object* pObject = &objectUI[i];
		
		// �e�N�X�`���̔j��
		if (pObject->tex != NULL)
		{
			pObject->tex->Release();
			pObject->tex = NULL;
		}

		// ���_�o�b�t�@�̔j��
		if (pObject->vtxBuff != NULL)
		{
			pObject->vtxBuff->Release();
			pObject->vtxBuff = NULL;
		}
	}
}

//=========================================
// �X�V
//=========================================
void UpdateUI(void)
{
}

//=========================================
// �`��
//=========================================
void DrawUI(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_UI; i++)
	{
		Object* pObject = &objectUI[i];

		if (!pObject->bUse)
		{
			continue;
		}

		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, pObject->vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���`��
		// �e�N�X�`���̐ݒ�
		RectDraw(pDevice, pObject->tex, 0);
	}
}

//=========================================
// �ݒ�
//=========================================
void SetUI(char * pFile, D3DXVECTOR3 * pos, D3DXCOLOR * col, D3DXVECTOR3 * size)
{
	for (int i = 0; i < MAX_UI; i++)
	{
		Object* pObject = &objectUI[i];

		if (pObject->bUse)
		{
			continue;
		}
		ZeroMemory(pObject, sizeof(pObject));

		pObject->pos = *pos;		// �ʒu
		pObject->col = *col;		// �F
		pObject->Height = size->y;	// ����
		pObject->Width = size->x;	// ��
		pObject->bUse = true;		// �g�p�ɐ؂�ւ�

		LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

		D3DXCreateTextureFromFile(pDevice, pFile, &pObject->tex);	// �e�N�X�`���̓Ǎ�

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pObject->vtxBuff,
			NULL);

		VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pObject->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fLength = sqrtf(pObject->Width  * pObject->Width + pObject->Height * pObject->Height) / 2.0f;	// ���S���W�����̒������Z�o����B
		float fAngle = atan2f(pObject->Width, pObject->Height);	// ���S���W�����̒��_�̊p�x���Z�o����

		SetPosRectCenterRot(pVtx, pObject->pos, pObject->rot, fAngle, fLength);		// ���_���W�̐ݒ�
		SetRectColor(pVtx, &(pObject->col));										// ���_�J���[�̐ݒ�
		InitRectRhw(pVtx);															// rhw�̐ݒ�
		InitRectTex(pVtx);															// �e�N�X�`�����W�̐ݒ�
		
		// ���_�o�b�t�@���A�����b�N����
		pObject->vtxBuff->Unlock();

		break;
	}
}


//=========================================
// �^�C�g��UI�̓ǂݍ��ݏ���
//=========================================
void LoadTitleUI(void)
{
	SetUI("/data/TEXTURE/07.�ޕ��ւ̌N�ɕ���.png", &D3DXVECTOR3(10.0f, 10.0f, 0.0f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), &D3DXVECTOR3(100.0f, 100.0f, 0.0f));
}

//=========================================
// �Q�[��UI�̓ǂݍ��ݏ���
//=========================================
void LoadGameUI(void)
{
}

//=========================================
// ���U���gUI�̓ǂݍ��ݏ���
//=========================================
void LoadResultUI(void)
{
	// ���ւ̃{�^��
	SetUI(SELECTBG, &D3DXVECTOR3(10.0f, 10.0f, 0.0f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), &D3DXVECTOR3(10.0f, 10.0f, 0.0f));
}