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
	D3DXVECTOR3 oldmovevec;	// �x�N�g��
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 axis;		// ��]��
	float moverot;			// �ړ����̉�]��
	D3DXQUATERNION quaternion;
	D3DXQUATERNION oldquaternion;
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
