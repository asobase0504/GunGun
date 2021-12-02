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
#include "mesh_build.h"
#include "polygon.h"
#include "setup.h"

#define SIZE	(50)

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
} Mesh;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};	// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTexture = {};		// �e�N�X�`���ւ̃|�C���^
static Mesh s_abillboard;					// �|���S���̍\����

//=========================================
// ������
//=========================================
void InitMeshBuild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ����������
	s_abillboard.pos = ZERO_VECTOR;	// ���_���W
	s_abillboard.rot = ZERO_VECTOR;	// ��]���W

									// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/07.�ޕ��ւ̌N�ɕ���.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-SIZE, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-SIZE, 0.0f, SIZE);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, SIZE);
	pVtx[4].pos = D3DXVECTOR3(SIZE, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SIZE, 0.0f, SIZE);
	pVtx[6].pos = D3DXVECTOR3(SIZE, 0.0f, SIZE);
	pVtx[7].pos = D3DXVECTOR3(-SIZE, 0.0f, -SIZE);
	pVtx[8].pos = D3DXVECTOR3(-SIZE, 0.0f, -SIZE);
	pVtx[9].pos = D3DXVECTOR3(-SIZE, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -SIZE);
	pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[12].pos = D3DXVECTOR3(SIZE, 0.0f, -SIZE);
	pVtx[13].pos = D3DXVECTOR3(SIZE, 0.0f, 0.0f);

	// �e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[11].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[12].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.5f);;

	pVtx += 4;

	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

}

//=========================================
// �I��
//=========================================
void UninitMeshBuild(void)
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
void UpdateMeshBuild(void)
{
}

//=========================================
// �`��
//=========================================
void DrawMeshBuild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_abillboard.mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_abillboard.rot.y, s_abillboard.rot.x, s_abillboard.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_abillboard.mtxWorld, &s_abillboard.mtxWorld, &mtxRot);							// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_abillboard.pos.x, s_abillboard.pos.y, s_abillboard.pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_abillboard.mtxWorld, &s_abillboard.mtxWorld, &mtxTrans);							// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_abillboard.mtxWorld);	// ���[���h���W�s��̐ݒ�

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);

}

D3DXVECTOR3 GetMeshBuildPos(void)
{
	return s_abillboard.pos;
}
