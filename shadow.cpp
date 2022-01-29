//=========================================
// 
// �e�̏���
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "shadow.h"
#include "common.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define SHADOW_MAX	(5)

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};	// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTexture = {};		// �e�N�X�`���ւ̃|�C���^
static Shadow s_aShadow[SHADOW_MAX];				// �|���S���̍\����

//=========================================
// ������
//=========================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ����������
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		s_aShadow[i].pos = ZERO_VECTOR;		// ���_���W
		s_aShadow[i].rot = ZERO_VECTOR;		// ��]���W
		s_aShadow[i].bUse = false;			// �g�p���Ă��Ȃ����
	}

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * SHADOW_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	for (int i = 0; i < SHADOW_MAX; i++)
	{
		VERTEX_3D* pVtx;

		// ���_���W�����b�N
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(s_aShadow[i].pos.x - 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + 5.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aShadow[i].pos.x + 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + 5.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aShadow[i].pos.x - 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + -10.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aShadow[i].pos.x + 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + -10.0f);

		// �e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_���W���A�����b�N
		s_pVtxBuff->Unlock();

	}
}

//=========================================
// �I��
//=========================================
void UninitShadow(void)
{
	// �e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// ���_�o�b�t�@�[�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// �X�V
//=========================================
void UpdateShadow(void)
{
}

//=========================================
// �`��
//=========================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		if (!(s_aShadow[i].bUse))
		{
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&s_aShadow[i].mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aShadow[i].rot.y, s_aShadow[i].rot.x, s_aShadow[i].rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixMultiply(&s_aShadow[i].mtxWorld, &s_aShadow[i].mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, s_aShadow[i].pos.x, s_aShadow[i].pos.y, s_aShadow[i].pos.z);		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&s_aShadow[i].mtxWorld, &s_aShadow[i].mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &s_aShadow[i].mtxWorld);	// ���[���h���W�s��̐ݒ�

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �`�揈��
		RectDraw(pDevice, s_pTexture, i * 4);

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================
// �ݒ�
//=========================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int i;
	for (i = 0; i < SHADOW_MAX; i++)
	{
		if (s_aShadow[i].bUse)
		{	// ���Ɏg�p���Ă���ꍇ
			continue;
		}

		s_aShadow[i].pos = pos;
		s_aShadow[i].rot = rot;
		s_aShadow[i].bUse = true;

		break;
	}
	return i;
}

//=========================================
// �e�ʒu�̐ݒ�
//=========================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{

	if (s_aShadow[nIdxShadow].bUse)
	{	// ���Ɏg�p���Ă���ꍇ
		s_aShadow[nIdxShadow].pos = pos;
	}
}

//=========================================
// �e�ʒu�̐ݒ�
//=========================================
Shadow* GetShadow(void)
{
	return &(s_aShadow[0]);
}
