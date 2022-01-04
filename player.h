//=========================================
// 
// �v���C���[�̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "model.h"

//------------------------------------
// ���f���p�[�c�̍\���̒�`
//------------------------------------
#define PARTS_NUM	(20)		// �p�[�c�̐�

//------------------------------------
// ���f���p�[�c�̍\���̒�`
//------------------------------------
typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXQUATERNION quaternion;		// �N�I�[�^�j�I��
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// �p�x
	int nIdxModelParent;	// �e���f���̃C���f�b�N�X
}ModelParts;

//------------------------------------
// ���f���̍\���̒�`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 pos_old;			// �O��ʒu
	D3DXVECTOR3 rot;				// �p�x
	D3DXVECTOR3 rotDest;			// �ړI�̊p�x
	D3DXVECTOR3 movevec;			// �x�N�g��
	float moverot;					// �ړ����̉�]��
	D3DXVECTOR3 MinVtx;				// ���_�̍ŏ��l
	D3DXVECTOR3 MaxVtx;				// ���_�̍ő�l
	Model aModel[PARTS_NUM];	// ���f����
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
}Player;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitPlayer(void);		// �v���C���[�̏���������
void UninitPlayer(void);	// �v���C���[�̏I������
void UpdatePlayer(void);	// �v���C���[�̍X�V����
void DrawPlayer(void);		// �v���C���[�̍X�V����
void MovePlayer(void);		// �v���C���[�̈ړ�
Player* GetPlayer(void);	// �v���C���[�̎擾����
void LoadPlayerModel(void);	// �v���C���[�p�[�c�̓ǂݍ��ݏ���
void ColisionPartsModel(void);
#endif // !_PLAYER_H_
