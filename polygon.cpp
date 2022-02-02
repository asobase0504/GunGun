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
#include "common.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define MAX_POLYGON	(64)

//------------------------------------
// �|���S���̍\���̂��`
//------------------------------------
typedef struct
{
	char* name;
	LPDIRECT3DVERTEXBUFFER9	VtxBuff;	// ���_�o�b�t�@�[�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		Tex;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				pos;		// ���_���W
	D3DXVECTOR3				size;		// ���_���W
	D3DXVECTOR3				rot;		// ��]���W
	D3DXMATRIX				mtxWorld;	// ���[���h�}�g���b�N�X
	bool					bUse;		// �g�p���Ă��邩
} ObjectPolygon;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static ObjectPolygon s_aPolygon[MAX_POLYGON];		// �|���S���̍\����

//=========================================
// ������
//=========================================
void InitPolygon(void)
{
	ZeroMemory(s_aPolygon, sizeof(s_aPolygon));	// ������
}

//=========================================
// �I��
//=========================================
void UninitPolygon(void)
{
	for (int i = 0; i < MAX_POLYGON; i++)
	{
		// �e�N�X�`���̔j��
		if (s_aPolygon[i].Tex != NULL)
		{
			s_aPolygon[i].Tex->Release();
			s_aPolygon[i].Tex = NULL;
		}

		// ���_�o�b�t�@�[�̔j��
		if (s_aPolygon[i].VtxBuff != NULL)
		{
			s_aPolygon[i].VtxBuff->Release();
			s_aPolygon[i].VtxBuff = NULL;
		}
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

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		ObjectPolygon *polygon = &s_aPolygon[i];

		if (!polygon->bUse)
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&polygon->mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, polygon->rot.y, polygon->rot.x, polygon->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, polygon->pos.x, polygon->pos.y, polygon->pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxTrans);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &polygon->mtxWorld);	// ���[���h���W�s��̐ݒ�

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, polygon->VtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, polygon->Tex);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);

	}
}

//=========================================
// �ݒ�
//=========================================
void SetPolygon(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3 size, char* texFile, char* name)
{
	for (int i = 0; i <= MAX_POLYGON; i++)
	{
		ObjectPolygon* polygon = &s_aPolygon[i];

		if (polygon->bUse)
		{
			continue;
		}

		polygon->name = name;
		polygon->pos = *pos;
		polygon->size = size;
		polygon->rot = *rot;
		polygon->bUse = true;

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// �e�N�X�`���̓Ǎ�
		D3DXCreateTextureFromFile(pDevice, texFile,&polygon->Tex);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&polygon->VtxBuff,
			NULL);

		VERTEX_3D* pVtx = NULL;

		// ���_���W�����b�N
		polygon->VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-size.x, size.y,  size.z);
		pVtx[1].pos = D3DXVECTOR3( size.x, size.y,  size.z);
		pVtx[2].pos = D3DXVECTOR3(-size.x, size.y, -size.z);
		pVtx[3].pos = D3DXVECTOR3( size.x, size.y, -size.z);

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
		polygon->VtxBuff->Unlock();

		break;
	}
}

//=========================================
// �|���S���̓����蔻��
//=========================================
bool CollisionPolygon(D3DXVECTOR3* pos, char* name)
{

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		ObjectPolygon* polygon = &s_aPolygon[i];

		if (s_aPolygon[i].name == NULL)
		{
			continue;
		}

		if(strcmp(s_aPolygon[i].name, name) == 0 && polygon->bUse)
		{
			if (((polygon->pos.z - polygon->size.z) < pos->z) && ((polygon->pos.z + polygon->size.z) >  pos->z) &&
				((polygon->pos.x - polygon->size.x) < pos->x) && ((polygon->pos.x + polygon->size.x) > pos->x))
			{//(PL��O�� < ���f������) ���� (PL���� > ���f����O��)
				return true;
			}
		}
	}
	return false;
}

//=========================================
// �|���S���̈ʒu���擾
//=========================================
D3DXVECTOR3 GetPolygonPos(void)
{
	return s_aPolygon[0].pos;
}
