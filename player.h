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
	D3DXVECTOR3 axis;		// ��]��
	float moverot;			// �ړ����̉�]��
}Player;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitPlayer(void);		// ���f���̏���������
void UninitPlayer(void);		// ���f���̏I������
void UpdatePlayer(void);		// ���f���̍X�V����
void DrawPlayer(void);		// ���f���̍X�V����
void MovePlayer(void);			// ���f���̈ړ�
Player* GetPlayer(void);			// ���f���̎擾����

#endif // !_PLAYER_H_
