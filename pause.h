//============================================================================================================
//
//君をたずねてにゃん千里[pause.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//============================================================================================================
//ポーズメニュー
//============================================================================================================
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	//ゲームに戻る
	PAUSE_MENU_RETRY,			//ゲームをやり直す
	PAUSE_MENU_QUIT,			//タイトル画面に戻る
	PAUSE_MENU_MAX
}PAUSE_MENU;

//============================================================================================================
//ポーズメニューの構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
}PauseMenu;

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
bool SetEnablePause(bool bUse);

#endif