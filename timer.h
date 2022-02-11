//=========================================
// 
// タイム処理
// Author KOZUNA HIROHITO
// Author YudaKaito
// 
//=========================================
#ifndef _TIMER_H_			//このマクロ定義がされなかったら
#define _TIMER_H_			//2重インクルード防止のマクロ定義

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
//タイム(全体)の構造体の定義
//------------------------------------
typedef struct
{
	int nSecond;
	int nFps;
	int nDigit;
	int nNumber;
	bool bUse;
	bool bCount;
}Timer;

//------------------------------------
//タイム(1桁)の構造体の定義
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
// プロトタイプ宣言
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
