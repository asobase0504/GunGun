//=========================================
// 
// �v���C���[�̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//------------------------------------
// ���f���̍\���̒�`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// �p�x
	D3DXVECTOR3 rotDest;	// �ړI�̊p�x
	D3DXVECTOR3 movevec;	// �x�N�g��
	D3DXVECTOR3 move;		// �ړ���
	float moverot;			// �ړ����̉�]��
	D3DXQUATERNION quaternion;
	D3DXVECTOR3 MinVtx;		// ���_�̍ŏ��l
	D3DXVECTOR3 MaxVtx;		// ���_�̍ő�l
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

#endif // !_PLAYER_H_
