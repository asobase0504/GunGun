//============================================================================================================
//
//�N�������˂Ăɂ��痢[pause.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//============================================================================================================
//�|�[�Y���j���[
//============================================================================================================
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	//�Q�[���ɖ߂�
	PAUSE_MENU_RETRY,			//�Q�[������蒼��
	PAUSE_MENU_QUIT,			//�^�C�g����ʂɖ߂�
	PAUSE_MENU_MAX
}PAUSE_MENU;

//============================================================================================================
//�|�[�Y���j���[�̍\���̂̒�`
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
}PauseMenu;

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
bool SetEnablePause(bool bUse);

#endif