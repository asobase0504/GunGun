//=========================================
// 
// �^�C������
// Author KOZUNA HIROHITO
// Author YudaKaito
// 
//=========================================
#ifndef _TIMER_H_			//���̃}�N����`������Ȃ�������
#define _TIMER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
//�^�C��(�S��)�̍\���̂̒�`
//------------------------------------
typedef struct
{
	LPD3DXFONT font;
	D3DXVECTOR3 pos;
	int nSecond;
	int nFps;
	int nDigit;
	int nNumber;
	bool bUse;
	bool bCount;
}Timer;

//------------------------------------
//�^�C��(1��)�̍\���̂̒�`
//------------------------------------
typedef struct
{
	float fWidth;
	float fHeight;
	D3DXVECTOR3 pos;
	int nPatn;
	int nNumber;
	bool bUse;

}TimerDigit;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
Timer* GetTimer(int nNumber);
void StartTimer(int nSecond, int nDigit, float fWidth, float fHeight, D3DXVECTOR3 pos , int nPatn);
bool TimerUp(int nNumber);
void BreakTimer(int nNumber);
void CountRestartStop(bool bCount, int nNumber);

#endif
