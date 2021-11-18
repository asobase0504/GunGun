//=========================================
// 
// ���f�������֐�
// Author YudaKaito
// 
//=========================================
#include "model.h"
#include "main.h"

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPD3DXMESH s_pMesh = NULL;		// ���b�V�����ւ̃|�C���^		// ���_�̏W�܂�̂���
static LPD3DXBUFFER s_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^	// 1��X�t�@�C���ɕ����̃}�e���A���������Ă���
static DWORD s_nNumMat = 0;				// �}�e���A�����̐�
static D3DXVECTOR3 s_pos;				// �ʒu
static D3DXVECTOR3 s_rot;				// ����
static D3DXMATRIX s_mtxWorld;			// ���[���h�}�g���b�N�X


//=========================================
// ������
//=========================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/bee_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	s_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=========================================
// �I��
//=========================================
void UninitModel(void)
{
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
}

//=========================================
// �`��
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_rot.y, s_rot.x, s_rot.z);		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_pos.y, s_pos.x, s_pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

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

		// ���f���p�[�c�̕`��
		s_pMesh->DrawSubset(i);
	}
	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
