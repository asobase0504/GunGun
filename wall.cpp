//=========================================
// 
// �ǂ̍쐬
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "polygon.h"
#include "common.h"
#include "wall.h"

//------------------------------------
// �|���S���̎�ނ̗񋓌^
//------------------------------------
#define BIILBOARD_MAX	(4)	// �ǂ̍ő吔

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
static Billboard s_abillboard[BIILBOARD_MAX];							// �ǂ̍\����

//=========================================
// ������
//=========================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
	//	ZeroMemory(&(wall[0]), sizeof(Wall));
		// ����������
		s_abillboard[i].pos = ZERO_VECTOR;	// ���_���W
		s_abillboard[i].rot = ZERO_VECTOR;	// ��]���W
		s_abillboard[i].bUse = false;
	}

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
		pVtx[0].pos = D3DXVECTOR3(-50.0f, 50.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);

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

		pVtx += 4;
	}
	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

}

//=========================================
// �I��
//=========================================
void UninitWall(void)
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
void UpdateWall(void)
{
}

//=========================================
// �`��
//=========================================
void DrawWall(void)
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

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_abillboard[i].rot.y, s_abillboard[i].rot.x, s_abillboard[i].rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixMultiply(&s_abillboard[i].mtxWorld, &s_abillboard[i].mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, s_abillboard[i].pos.x, s_abillboard[i].pos.y, s_abillboard[i].pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&s_abillboard[i].mtxWorld, &s_abillboard[i].mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &s_abillboard[i].mtxWorld);	// ���[���h���W�s��̐ݒ�

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		RectDraw(pDevice, s_pTexture, i * 4);

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);

	}
}

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
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

D3DXVECTOR3* GetWallPos(void)
{
	return &(s_abillboard[0].pos);
}