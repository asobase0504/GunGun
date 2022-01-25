//============================================================================================================
//
// ���U���g����
// Author:Kishimoto Eiji
// Author:Yuda Kaito
//
//============================================================================================================
//------------------------------------
// include
//------------------------------------
#include "result.h"
#include "input.h"
#include "common.h"
#include "fade.h"
//#include "sound.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define FILENAME	("data/TEXTURE/Result_Hunter.png")

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9 s_pTexture[GAME_MAX] = { NULL,NULL };		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTextureWinner = NULL;					//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;					//���_�o�b�t�@�ւ̃|�C���^
static bool bWin;

//============================================================================================================
//���U���g�̏���������
//============================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		FILENAME,
		&s_pTexture[GAME_HUNTER]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Result_Survivor.png",
		&s_pTexture[GAME_SURVIVOR]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Winner.png",
		&s_pTextureWinner);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	InitRectTex(pVtx);

	pVtx += 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	InitRectTex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//���U���g�̏I������
//============================================================================================================
void UninitResult(void)
{
	for (int i = 0; i < GAME_MAX; i++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//���U���g�̍X�V����
//============================================================================================================
void UpdateResult(void)
{
	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�
		if (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN))
		{//Enter�L�[�������ꂽ��
			// �^�C�g����ʂɈڍs
			SetFade(MODE_TITLE);
		}
	}
}

//============================================================================================================
//���U���g�̕`�揈��
//============================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bWin)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[GAME_HUNTER]);
	}
	else
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[GAME_SURVIVOR]);
	}

	//���U���g�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}
