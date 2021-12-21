//=========================================
// 
// ���f�������֐�
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "model.h"
#include "common.h"
#include "input.h"
#include "setup.h"
#include "camera.h"
#include "shadow.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define MODEL_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)

//------------------------------------
// ���f���X�e�[�^�X�񋓌^
//------------------------------------
typedef enum
{
	MODELSTATEL_NONE = 0,
	MODELSTATEL_MOVE,
	MODELSTATEL_MAX
}MODEL_STATE;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPD3DXMESH s_pMesh = NULL;				// ���b�V�����ւ̃|�C���^		// ���_�̏W�܂�̂���
static LPD3DXBUFFER s_pBuffMat = NULL;			// �}�e���A�����ւ̃|�C���^	// 1��X�t�@�C���ɕ����̃}�e���A���������Ă���
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
static DWORD s_nNumMat = 0;						// �}�e���A�����̐�
static D3DXMATRIX s_mtxWorld;					// ���[���h�}�g���b�N�X
static Model s_model;							// ���f���̍\����
static MODEL_STATE s_state;						// ���f���̃X�e�[�^�X
static int s_nShadowCnt;						// �e�̊��蓖��

//=========================================
// ������
//=========================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�[�ւ̃|�C���g

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/bee_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	// ���f���̃T�C�Y�v��
	ModelSize(&(s_model.MinVtx), &(s_model.MaxVtx), s_pMesh);

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		s_pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_pTexture[i]);
		}
		else
		{
			s_pTexture[i] = NULL;
		}
	}

	// ���f���̃T�C�Y�v��
	ModelSize(&s_model.MinVtx, &s_model.MaxVtx, s_pMesh);

	s_model.pos = ZERO_VECTOR;
	s_model.pos = D3DXVECTOR3(20.0f,s_model.MinVtx.y * -1.0f,0.0f);
	s_model.rot = ZERO_VECTOR;
	s_model.vec = ZERO_VECTOR;

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = s_model.pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = s_model.pos.z;
	s_nShadowCnt = SetShadow(ShadowPos, s_model.rot);
}

//=========================================
// �I��
//=========================================
void UninitModel(void)
{
	if (s_pTexture != NULL)
	{
		for (int i = 0; i < (int)s_nNumMat; i++)
		{
			if (s_pTexture[i] != NULL)
			{// �e�N�X�`���̉��
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	// ���b�V���̉��
	if(s_pMesh != NULL)
	{
		s_pMesh->Release();
		s_pMesh = NULL;
	}
	// �}�e���A���̉��
	if (s_pBuffMat != NULL)
	{
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//=========================================
// �X�V
//=========================================
void UpdateModel(void)
{
	Model* pModel = &(s_model);
	//MoveModel();

	pModel->pos += pModel->vec;
	pModel->rot.y += NormalizeRot(pModel->rotDest.y - pModel->rot.y) * MODEL_ROT_ATTENUATION;

	// �p�x�̐��K��
	NormalizeRot(pModel->rot.y);

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = pModel->pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = pModel->pos.z;
	SetPositionShadow(s_nShadowCnt,ShadowPos);
}

//=========================================
// �`��
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);										// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);								// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);;

	// ���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		// ���f���p�[�c�̕`��
		s_pMesh->DrawSubset(i);
	}
	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �ړ�
//=========================================
void MoveModel()
{
	Model* pModel = &(s_model);
	D3DXVECTOR3 CameraRot = GetRotCamera();	// �J�����̊p�x���擾
	D3DXVECTOR3 move = ZERO_VECTOR;			// �ړ��ʂ̏�����

	// ���f���̈ړ�
	if (GetKeyboardPress(DIK_UP))
	{
		move.x += sinf(CameraRot.y);
		move.z += cosf(CameraRot.y);
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		move.x += sinf(D3DX_PI * -0.5f + CameraRot.y);
		move.z += cosf(D3DX_PI * -0.5f + CameraRot.y);
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		move.x += sinf(D3DX_PI + CameraRot.y);
		move.z += cosf(D3DX_PI + CameraRot.y);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		move.x += sinf(D3DX_PI * 0.5f + CameraRot.y);
		move.z += cosf(D3DX_PI * 0.5f + CameraRot.y);
	}

	// ���f���̏㉺�̈ړ�
	if (GetKeyboardPress(DIK_T))
	{
		pModel->pos.y += MODEL_MOVE;
	}
	if (GetKeyboardPress(DIK_B))
	{
		pModel->pos.y += -(MODEL_MOVE);
	}

	if (D3DXVec3Length(&move) == 0.0f)
	{
		pModel->vec *= 0.95f;
	}
	else
	{
		D3DXVec3Normalize(&move, &move);	// ���K������(�傫���P�̃x�N�g���ɂ���)
		pModel->rotDest.y = atan2f(move.x, move.z);
	}

	pModel->vec = pModel->vec * 0.95f + move * MODEL_MOVE * 0.05f;
}

//=========================================
// �����蔻��
//=========================================
void CollisionModel(D3DXVECTOR3 * pos, D3DXVECTOR3 * pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{

}

//=========================================
// �擾
//=========================================
Model *GetModel(void)
{
	return &s_model;
}
