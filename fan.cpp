//==================================================
//
// ���̉~
// Author : Yuda Kaito
//
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "fan.h"
#include "common.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define START_HORIZONTAL		(10)		// ���̍ŏ��̒l
#define MAX_VERTEX				(100)		// ���_��

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		// �~�̒��_�o�b�t�@�ւ̃|�C���^
static Fan						s_Fan;					// ���b�V����̏��

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitFan(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\sky.jpg",&s_pTexture);

	memset(&s_Fan, NULL, sizeof(s_Fan));	// �������̃N���A
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitFan(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// �~�̒��_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateFan(void)
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawFan(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_Fan.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Fan.rot.y, s_Fan.rot.x, s_Fan.rot.z);
	D3DXMatrixMultiply(&s_Fan.mtxWorld, &s_Fan.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_Fan.pos.x, s_Fan.pos.y, s_Fan.pos.z);
	D3DXMatrixMultiply(&s_Fan.mtxWorld, &s_Fan.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_Fan.mtxWorld);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,MAX_VERTEX - 2);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetFan(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^�̎擾
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	D3DXCOLOR col(0.0f, 0.55f, 1.0f, 1.0f);

	// �������̃N���A
	memset(&s_Fan, NULL, sizeof(s_Fan));

	s_Fan.rot = rot;
	s_Fan.pos = pos;
	s_Fan.fLength = fLength;

	// �~�̒��_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos;

	D3DXVECTOR3 nor;

	// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
	D3DXVec3Normalize(&nor, &pos);

	pVtx[0].nor = nor;	// �e���_�̖@���̐ݒ�
	pVtx[0].col = col;	// ���_�J���[�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// �e�N�X�`�����W�̐ݒ�

	DEBUG_PRINT("%f	%f\n", pVtx[0].pos.x, pVtx[0].pos.z);

	for (int i = 1; i < MAX_VERTEX; i++)
	{
		float fRot = (D3DX_PI * 2.0f / (MAX_VERTEX-2)) * (i - 1);

		// �p�x�̐��K��
		NormalizeRot(&fRot);

		// ���_���W�̐ݒ�
		pVtx[i].pos.x = pos.x + sinf(fRot) * fLength;
		pVtx[i].pos.y = pos.y;
		pVtx[i].pos.z = pos.z + cosf(fRot) * fLength;

		// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
		D3DXVec3Normalize(&nor, &pVtx[i].pos);

		// �e���_�̖@���̐ݒ�
		pVtx[i].nor = nor;

		// ���_�J���[�̐ݒ�
		pVtx[i].col = col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		DEBUG_PRINT("%f	%f\n", pVtx[i].pos.x, pVtx[i].pos.z);
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// ���ɊJ��
//--------------------------------------------------
void OpenFan(int nCnt)
{
	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^
	
	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 1; i < MAX_VERTEX; i++)
	{
		if (i < nCnt)
		{
			continue;
		}

		float fRot = (D3DX_PI * 2.0f / (MAX_VERTEX - 2)) * nCnt;

		// �p�x�̐��K��
		NormalizeRot(&fRot);

		// ���_���W�̐ݒ�
		pVtx[i].pos.x = s_Fan.pos.x + sinf(fRot) * s_Fan.fLength;
		pVtx[i].pos.z = s_Fan.pos.z + cosf(fRot) * s_Fan.fLength;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// ���ɕ���
//--------------------------------------------------
void CloseFan(int nCnt)
{
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Fan *GetFan(void)
{
	return &s_Fan;
}