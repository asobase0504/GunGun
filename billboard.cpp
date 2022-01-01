//=========================================
// 
// �|���S���̍쐬
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "billboard.h"

//------------------------------------
// �|���S���̎�ނ̗񋓌^
//------------------------------------
#define BIILBOARD_MAX	(4)	// �r���{�[�h�̍ő吔

//------------------------------------
// �|���S���̍\���̂��`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 rot;	// ��]���W
	bool bUse;			// �g�p��
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
} Billboard;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTexture = {};			// �e�N�X�`���ւ̃|�C���^
static Billboard s_abillboard[BIILBOARD_MAX];			// �r���{�[�h�̍\����

//=========================================
// ������
//=========================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ZeroMemory(s_abillboard, sizeof(s_abillboard));

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/�Ó].jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BIILBOARD_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f, 0.0f);

		// �e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;
	}
	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

}

//=========================================
// �I��
//=========================================
void UninitBillboard(void)
{
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
void UpdateBillboard(void)
{
}

//=========================================
// �`��
//=========================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
		if (!(s_abillboard[i].bUse))
		{
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&s_abillboard[i].mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		s_abillboard->mtxWorld._11 = mtxView._11;
		s_abillboard->mtxWorld._12 = mtxView._21;
		s_abillboard->mtxWorld._13 = mtxView._31;
		s_abillboard->mtxWorld._31 = mtxView._13;
		s_abillboard->mtxWorld._32 = mtxView._23;
		s_abillboard->mtxWorld._33 = mtxView._33;

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, s_abillboard[i].pos.x, s_abillboard[i].pos.y, s_abillboard[i].pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&s_abillboard[i].mtxWorld, &s_abillboard[i].mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &s_abillboard[i].mtxWorld);	// ���[���h���W�s��̐ݒ�

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ���C�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		// Z�o�b�t�@�̏㏑���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, false);

		// �`��
		RectDraw(pDevice, s_pTexture, i * 4);

		// Z�o�b�t�@�̏㏑���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, true);

		// ���C�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, true);

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);

	}
}

void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
		if (s_abillboard[i].bUse)
		{	// ���Ɏg�p���Ă���ꍇ
			continue;
		}

		s_abillboard[i].pos = pos;
		s_abillboard[i].rot = rot;
		s_abillboard[i].bUse = true;

		break;
	}

}

D3DXVECTOR3* GetBillboardPos(void)
{
	return &(s_abillboard[0].pos);
}