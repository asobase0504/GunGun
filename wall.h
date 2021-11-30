//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _WALL_H_
#define _WALL_H_


//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitWall(void);			// �ǂ̏���������
void UninitWall(void);			// �ǂ̏I������
void UpdateWall(void);			// �ǂ̍X�V����
void DrawWall(void);			// �ǂ̕`�揈��
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �ǂ̐ݒ菈��
D3DXVECTOR3* GetWallPos(void);	// �ǂ̎擾����

#endif // !_WALL_H_
