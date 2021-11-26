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
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// �p�x
	D3DXVECTOR3 rotDest;	// �ړI�̊p�x
	D3DXVECTOR3 vec;		// �x�N�g��
	D3DXVECTOR3 move;		// �ړ���
}Model;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitModel(void);		// ���f���̏���������
void UninitModel(void);		// ���f���̏I������
void UpdateModel(void);		// ���f���̍X�V����
void DrawModel(void);		// ���f���̍X�V����
void MoveModel(void);			// ���f���̈ړ�
Model* GetModel(void);			// ���f���̎擾����

#endif // !_MODEL_H_
