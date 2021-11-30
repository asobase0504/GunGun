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
#include "polygon.h"
#include "setup.h"

//------------------------------------
// �|���S���̎�ނ̗񋓌^
//------------------------------------
typedef enum
{
	POLYGON_FLOOR = 0,
	POLYGON_MAX
}POLYGON_TYPE;

//------------------------------------
// �|���S���̍\���̂��`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 rot;	// ��]���W
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
} Wall;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};	// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTexture[POLYGON_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
static Wall wall;								// �|���S���̍\����

//=========================================
// ������
//=========================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ����������
	wall.pos = ZERO_VECTOR;	// ���_���W
	wall.rot = ZERO_VECTOR;	// ��]���W

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/07.�ޕ��ւ̌N�ɕ���.png",
		&s_pTexture[0]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * POLYGON_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-50.0f, 0.0f, 50.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	pVtx[2].pos = D3DXVECTOR3(-50.0f, 0.0f, -100.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, -100.0f);

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

	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

}

//=========================================
// �I��
//=========================================
void UninitPolygon(void)
{
	// �e�N�X�`���̔j��
	for (int i = 0; i < POLYGON_MAX; i++)
	{
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
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
void UpdatePolygon(void)
{
}

//=========================================
// �`��
//=========================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&wall.mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, wall.rot.y, wall.rot.x, wall.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&wall.mtxWorld, &wall.mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, wall.pos.x, wall.pos.y, wall.pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&wall.mtxWorld, &wall.mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &wall.mtxWorld);	// ���[���h���W�s��̐ݒ�

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < POLYGON_MAX; i++)
	{
		switch (i)
		{
		case POLYGON_FLOOR:
			RectDraw(pDevice, s_pTexture[i], i * 4);
			break;
		default:
			break;
		}
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);

	}

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

}

D3DXVECTOR3 GetPolygonPos(void)
{
	return wall.pos;
}
