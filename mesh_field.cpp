//=========================================
// 
// ���b�V���̍쐬
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "mesh_field.h"
#include "polygon.h"
#include "setup.h"
#include "input.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define SIZE	(50.0f)

//------------------------------------
// �|���S���̎�ނ̗񋓌^
//------------------------------------
typedef enum
{
	POLYGON_FLOOR = 0,
	POLYGON_MAX
}POLYGON_TYPE;

//------------------------------------
// ���b�V���̍\���̂��`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 rot;	// ��]���W
	int nSurfaceWidth;	// �ʂ̕�
	int nSurfaceHeight;	// �ʂ̍���
	float fLineWidth;	// �ӂ̕�
	float fLineHeight;	// �ӂ̍���
	int vertexCnt;		// ���_��
	int polygonCnt;		// �|���S����
	int IdxCnt;			// �C���f�b�N�X��
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
} Mesh;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTexture = {};			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9 s_pIdxBuff = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
static Mesh s_aMesh[2] = {};						// �|���S���̍\����

//=========================================
// ������
//=========================================
void InitMeshBuild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// nSurfaceWidth �~ nSurfaceHeight
	s_aMesh[0].nSurfaceWidth = 10;									// X���̖ʂ̐�
	s_aMesh[0].nSurfaceHeight = 10;									// Y���̖ʂ̐�
	s_aMesh[0].fLineWidth = 50.0f;									// X���̖ʂ̐�
	s_aMesh[0].fLineHeight = 50.0f;									// Y���̖ʂ̐�

	int nLineVtx = (s_aMesh[0].nSurfaceWidth + 1);					// X���̒��_��

	s_aMesh[0].vertexCnt = nLineVtx * (s_aMesh[0].nSurfaceHeight + 1);	// ���_��

	// �|���S���������߂�v�Z
	s_aMesh[0].polygonCnt
		= 2 * s_aMesh[0].nSurfaceWidth * s_aMesh[0].nSurfaceHeight		// ��s���̃|���S����
		+ 4 * (s_aMesh[0].nSurfaceHeight - 1);				// �k�ރ|���S����

	s_aMesh[0].IdxCnt = s_aMesh[0].polygonCnt + 2;	// �C���f�b�N�X��

//�ʉ�
/*
	// �C���f�b�N�X�������߂�v�Z
	s_aMesh[0].IdxCnt
	= 2 * (s_aMesh[0].nSurfaceWidth + 1) * s_aMesh[0].nSurfaceHeight	// ��s���̃C���f�b�N�X��
	+ 2 * (s_aMesh[0].nSurfaceHeight - 1);					// ���s���ɔ�������d�����Ă���C���f�b�N�X��

	s_aMesh[0].polygonCnt = s_aMesh[0].IdxCnt - 2;	// �|���S����
*/

// ����������
	s_aMesh[0].pos = ZERO_VECTOR;	// ���_���W
	s_aMesh[0].rot = ZERO_VECTOR;	// ��]���W

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/07.�ޕ��ւ̌N�ɕ���.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * s_aMesh[0].polygonCnt,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * s_aMesh[0].IdxCnt,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int Z = 0; Z <= s_aMesh[0].nSurfaceHeight;Z++)
	{
		for (int X = 0; X <= s_aMesh[0].nSurfaceWidth; X++)
		{
			pVtx[X + Z * nLineVtx].pos = D3DXVECTOR3((X - s_aMesh[0].nSurfaceWidth * 0.5f) *  s_aMesh[0].fLineWidth,0.0f,(Z - s_aMesh[0].nSurfaceHeight * 0.5f) * -s_aMesh[0].fLineHeight);

			pVtx[X + Z * nLineVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[X + Z * nLineVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[X + Z * nLineVtx].tex = D3DXVECTOR2((float)X, (float)Z);
		}
	}

	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

	WORD* pIdx;
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�����߂�
	for (int Y = 0; Y < s_aMesh[0].nSurfaceHeight; Y++)
	{
		int nlineTop = Y * (nLineVtx * 2 + 2);
		for (int X = 0; X <= s_aMesh[0].nSurfaceWidth; X++)
		{
			int nIdxData = X * 2 + nlineTop;
			pIdx[nIdxData + 1] = X + nLineVtx * Y;
			pIdx[nIdxData] = pIdx[nIdxData + 1] + nLineVtx;
		}

		if (Y < s_aMesh[0].nSurfaceHeight - 1)
		{
			pIdx[nLineVtx * 2 + 0 + nlineTop] = s_aMesh[0].nSurfaceWidth + nLineVtx * Y;

//			pIdx[X���̒��_�� * 2 + 1 + ��s�Ŏg�p����C���f�b�N�X��] = X���̒��_�� * 2 + X���̒��_�� * Z���̌J��Ԃ�;

			pIdx[nLineVtx * 2 + 1 + nlineTop] = nLineVtx * 2 + nLineVtx * Y;

		}
	}

	s_pIdxBuff->Unlock();
}

//=========================================
// �I��
//=========================================
void UninitMeshBuild(void)
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

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (s_pIdxBuff != NULL)
	{
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
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
	D3DXMatrixIdentity(&s_aMesh[0].mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aMesh[0].rot.y, s_aMesh[0].rot.x, s_aMesh[0].rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_aMesh[0].mtxWorld, &s_aMesh[0].mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_aMesh[0].pos.x, s_aMesh[0].pos.y, s_aMesh[0].pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_aMesh[0].mtxWorld, &s_aMesh[0].mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_aMesh[0].mtxWorld);	// ���[���h���W�s��̐ݒ�

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(s_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,s_aMesh[0].vertexCnt,0,s_aMesh[0].polygonCnt);

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//=========================================
// ���b�V���̍��W���擾
//=========================================
D3DXVECTOR3 GetMeshBuildPos(void)
{
	return s_aMesh[0].pos;
}

//=========================================
// ���b�V���Œ����`�̍쐬�ݒ�
//=========================================
void SetMeshRect(void)
{
}

//=========================================
// ���b�V���ŉ~���̍쐬�ݒ�
//=========================================
void SetMeshCylinder(void)
{
}
