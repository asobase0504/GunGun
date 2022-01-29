#ifndef _TIMER_H_			//���̃}�N����`������Ȃ�������
#define _TIMER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

// �v���g�^�C�v�錾
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void StartTimer(int nSecond, int nDigit, float fWidth, float fHeight, D3DXVECTOR3 pos , int nPatn);
bool TimerUp(int nNumber);
void BreakTimer(int nNumber);
void CountRestartStop(bool bCount, int nNumber);

#endif
