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
// �v���g�^�C�v�錾
//------------------------------------
float D3DXVec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2);

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTexture = {};			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9 s_pIdxBuff = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
static Mesh s_aMesh = {};							// �|���S���̍\����
static int* s_aIdx = {};							// �C���f�b�N�X�̔z��

//=========================================
// ������
//=========================================
void InitMeshBuild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// nSurfaceWidth �~ nSurfaceHeight
	s_aMesh.nSurfaceWidth = 30;									// X���̖ʂ̐�
	s_aMesh.nSurfaceHeight = 30;								// Y���̖ʂ̐�
	s_aMesh.fLineWidth = 50.0f;									// X���̖ʂ̐�
	s_aMesh.fLineHeight = 50.0f;								// Y���̖ʂ̐�

	int nLineVtx = (s_aMesh.nSurfaceWidth + 1);					// X���̒��_��

	s_aMesh.vertexCnt = nLineVtx * (s_aMesh.nSurfaceHeight + 1);	// ���_��

	// �|���S���������߂�v�Z
	s_aMesh.polygonCnt
		= 2 * s_aMesh.nSurfaceWidth * s_aMesh.nSurfaceHeight		// ��s���̃|���S����
		+ 4 * (s_aMesh.nSurfaceHeight - 1);				// �k�ރ|���S����

	s_aMesh.IdxCnt = s_aMesh.polygonCnt + 2;	// �C���f�b�N�X��

//�ʉ�
/*
	// �C���f�b�N�X�������߂�v�Z
	s_aMesh.IdxCnt
	= 2 * (s_aMesh.nSurfaceWidth + 1) * s_aMesh.nSurfaceHeight	// ��s���̃C���f�b�N�X��
	+ 2 * (s_aMesh.nSurfaceHeight - 1);					// ���s���ɔ�������d�����Ă���C���f�b�N�X��

	s_aMesh.polygonCnt = s_aMesh.IdxCnt - 2;	// �|���S����
*/

// ����������
	s_aMesh.pos = ZERO_VECTOR;	// ���_���W
	s_aMesh.rot = ZERO_VECTOR;	// ��]���W

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/�Ó].jpg",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * s_aMesh.polygonCnt,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * s_aMesh.IdxCnt,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int Z = 0; Z <= s_aMesh.nSurfaceHeight;Z++)
	{
		for (int X = 0; X <= s_aMesh.nSurfaceWidth; X++)
		{
			pVtx[X + Z * nLineVtx].pos = D3DXVECTOR3((X - s_aMesh.nSurfaceWidth * 0.5f) *  s_aMesh.fLineWidth,sin(Z + X) * 20.0f,(Z - s_aMesh.nSurfaceHeight * 0.5f) * -s_aMesh.fLineHeight);

			pVtx[X + Z * nLineVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[X + Z * nLineVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[X + Z * nLineVtx].tex = D3DXVECTOR2((float)X, (float)Z);
		}
	}

	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_aIdx = new int[s_aMesh.IdxCnt];

	WORD* pIdx;
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�����߂�
	for (int Y = 0; Y < s_aMesh.nSurfaceHeight; Y++)
	{
		int nlineTop = Y * (nLineVtx * 2 + 2);
		for (int X = 0; X <= s_aMesh.nSurfaceWidth; X++)
		{
			int nIdxData = X * 2 + nlineTop;
			pIdx[nIdxData + 1] = X + nLineVtx * Y;
			pIdx[nIdxData] = pIdx[nIdxData + 1] + nLineVtx;
			s_aIdx[nIdxData + 1] = pIdx[nIdxData + 1];
			s_aIdx[nIdxData] = pIdx[nIdxData];
		}

		if (Y < s_aMesh.nSurfaceHeight - 1)
		{
			pIdx[nLineVtx * 2 + 0 + nlineTop] = s_aMesh.nSurfaceWidth + nLineVtx * Y;
			pIdx[nLineVtx * 2 + 1 + nlineTop] = nLineVtx * 2 + nLineVtx * Y;
			s_aIdx[nLineVtx * 2 + nlineTop] = pIdx[nLineVtx * 2 + nlineTop];
			s_aIdx[nLineVtx * 2 + 1 + nlineTop] = pIdx[nLineVtx * 2 + 1 + nlineTop];

		}
	}

	s_pIdxBuff->Unlock();
}

//=========================================
// �I��
//=========================================
void UninitMeshBuild(void)
{
	// �C���f�b�N�X���̕ۑ�
	if (s_aIdx != NULL)
	{
		delete[](s_aIdx);
		s_aIdx = NULL;
	}

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
	D3DXMatrixIdentity(&s_aMesh.mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aMesh.rot.y, s_aMesh.rot.x, s_aMesh.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_aMesh.mtxWorld, &s_aMesh.mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_aMesh.pos.x, s_aMesh.pos.y, s_aMesh.pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_aMesh.mtxWorld, &s_aMesh.mtxWorld, &mtxTrans);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_aMesh.mtxWorld);	// ���[���h���W�s��̐ݒ�

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(s_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,s_aMesh.vertexCnt,0,s_aMesh.polygonCnt);

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//=========================================
// ���b�V���̍��W���擾
//=========================================
D3DXVECTOR3 GetMeshBuildPos(void)
{
	return s_aMesh.pos;
}

//=========================================
// ���b�V���t�B�[���h�̓����蔻��
//=========================================
void CollisionMeshField(D3DXVECTOR3 * pos)
{
	VERTEX_3D* pVtx = NULL;
	int nLineVtx = (s_aMesh.nSurfaceWidth + 1);		// X���̒��_��
	D3DXVECTOR3 vecField[3];	// �|���S���̐���
	D3DXVECTOR3 vecModel[3];	// ���f������|���S���̐���

	// ���_���W�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̔��f
	for (int i = 0; i <= (s_aMesh.IdxCnt - 3); i++)
	{
		if (s_aIdx[i] == s_aIdx[i + 1] || s_aIdx[i + 1] == s_aIdx[i + 2] || s_aIdx[i] == s_aIdx[i + 2])
		{
			continue;
		}

		vecField[0] = pVtx[s_aIdx[i + 1]].pos - pVtx[s_aIdx[i]].pos;
		vecField[1] = pVtx[s_aIdx[i + 2]].pos - pVtx[s_aIdx[i + 1]].pos;
		vecField[2] = pVtx[s_aIdx[i]].pos - pVtx[s_aIdx[i + 2]].pos;

		vecModel[0] = *pos - pVtx[s_aIdx[i]].pos;
		vecModel[1] = *pos - pVtx[s_aIdx[i + 1]].pos;
		vecModel[2] = *pos - pVtx[s_aIdx[i + 2]].pos;

		float crs_v1 = D3DXVec2Cross(&vecModel[0], &vecField[0]);
		float crs_v2 = D3DXVec2Cross(&vecModel[1], &vecField[1]);
		float crs_v3 = D3DXVec2Cross(&vecModel[2], &vecField[2]);

		// ����Ă郁�b�V�����`�F�b�N
		if (i % 2 == 0)
		{
			if (crs_v1 >= 0.0f && crs_v2 >= 0.0f && crs_v3 >= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);
				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}
				D3DXVec3Normalize(&N, &N);

				pVtx[s_aIdx[i + 0]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 1]].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 2]].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pos->y = pVtx[s_aIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_aIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_aIdx[i]].pos.z));
			}
		}
		else
		{
			if (crs_v1 <= 0.0f && crs_v2 <= 0.0f && crs_v3 <= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);
				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}
				D3DXVec3Normalize(&N, &N);

				pVtx[s_aIdx[i + 0]].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 1]].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				pVtx[s_aIdx[i + 2]].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				pos->y = pVtx[s_aIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_aIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_aIdx[i]].pos.z));
			}
		}
	}

	// ���_���W���A�����b�N
	s_pVtxBuff->Unlock();

}

//=========================================
// 2D�x�N�g���̊O��
//=========================================
float D3DXVec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->z - v1->z * v2->x;
}