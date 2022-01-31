//=========================================
// 
// ���U���g�̏���(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "player.h"

//------------------------------------
// ���U���g�̒i�K
//------------------------------------
typedef enum
{
	RESULT_0,
	RESULT_1,
	RESULT_MAX
}RESULT_MODE;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitResult(void);					// ������
void UninitResult(void);				// �I��
void UpdateResult(void);				// �X�V
void DrawResult(int cameraData);		// �`��
RESULT_MODE* GetResult(void);			// ���[�h�̎擾
void SetResultPlayer(Player player);	// ���U���g�p�̃v���C���[�f�[�^�̎擾

#endif