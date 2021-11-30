//=========================================
// 
// �r���{�[�h�̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_


//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitBillboard(void);			// �r���{�[�h�̂̏���������
void UninitBillboard(void);			// �r���{�[�h�̂̏I������
void UpdateBillboard(void);			// �r���{�[�h�̂̍X�V����
void DrawBillboard(void);			// �r���{�[�h�̂̕`�揈��
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �r���{�[�h�̂̐ݒ菈��
D3DXVECTOR3* GetBillboardPos(void);	// �r���{�[�h�̂̎擾����

#endif // !_BILLBOARD_H_
