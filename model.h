//=========================================
// 
// ���f���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//------------------------------------
// ���f���̍\���̒�`
//------------------------------------
typedef struct
{
	LPD3DXMESH pMesh;				// ���b�V�����ւ̃|�C���^		// ���_�̏W�܂�̂���
	LPD3DXBUFFER pBuffMat;			// �}�e���A�����ւ̃|�C���^	// 1��X�t�@�C���ɕ����̃}�e���A���������Ă���
	LPDIRECT3DTEXTURE9 *pTexture;	// �e�N�X�`���ւ̃|�C���^
	DWORD nNumMat;					// �}�e���A�����̐�
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// �p�x
	D3DXVECTOR3 rotDest;			// �ړI�̊p�x
	D3DXVECTOR3 vec;				// �x�N�g��
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 MinVtx;				// ���_�̍ŏ��l
	D3DXVECTOR3 MaxVtx;				// ���_�̍ő�l
	int nIdxModelParent;			// �e���f���̃C���f�b�N�X
	bool bUse;						// �g�p��
}Model;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitModel(void);		// ���f���̏���������
void UninitModel(void);		// ���f���̏I������
void UpdateModel(void);		// ���f���̍X�V����
void DrawModel(void);		// ���f���̍X�V����
void CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3* pos_old,D3DXVECTOR3 min, D3DXVECTOR3 max);		// ���f���̏Փˏ���
Model* GetModel(void);		// ���f���̎擾����

#endif // !_MODEL_H_
