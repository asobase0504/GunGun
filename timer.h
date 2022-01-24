#ifndef _TIMER_H_			//このマクロ定義がされなかったら
#define _TIMER_H_			//2重インクルード防止のマクロ定義

#include "main.h"

// プロトタイプ宣言
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void StartTimer(int nSecond, int nDigit, float fWidth, float fHeight, D3DXVECTOR3 pos , int nPatn);
int TimerUp(int nNumber);
void BreakTimer(int nNumber);
void CountRestartStop(bool bCount, int nNumber);

#endif
