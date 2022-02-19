//=============================================================================
// 
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// *****************************************************************************
// �T�E���h�t�@�C��
// *****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGM�^�C�g��
	SOUND_LABEL_BGM_GAME,			// BGM�Q�[��
	SOUND_LABEL_BGM_RESULT,			// BGM���U���g
	SOUND_LABEL_SE_GETITEM,			// ����������
	SOUND_LABEL_SE_CHARGE,			// �^�C�g���Ń��[�h�I�����̉�
	SOUND_LABEL_SE_SERECT,			// �I����
	SOUND_LABEL_SE_TIME,			// ���Ԑ������߂��Ƃ�
	SOUND_LABEL_SE_ENDGAME,			// �Q�[���I����
	SOUND_LABEL_SE_SERECT_POUSE,	// �I����
	SOUND_LABEL_SE_ENTER,			// ���艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// *****************************************************************************
// �v���g�^�C�v�錾
// *****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
