//=========================================
// 
// ���U���g��UI����(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_UI_H_
#define _GAME_UI_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitGameUI(void);		// �Q�[��UI�̏���������
void UninitGameUI(void);	// �Q�[��UI�̏I������
void UpdateGameUI(void);	// �Q�[��UI�̍X�V����
void DrawGameUI(void);		// �Q�[��UI�̕`�揈��
void SetGameUITex(char* file);		// �Q�[��UI�̃e�N�X�`���̕ύX����

#endif // !_GAME_UI_H_
