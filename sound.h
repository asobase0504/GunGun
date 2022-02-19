//=============================================================================
// 
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// *****************************************************************************
// サウンドファイル
// *****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGMタイトル
	SOUND_LABEL_BGM_GAME,			// BGMゲーム
	SOUND_LABEL_BGM_RESULT,			// BGMリザルト
	SOUND_LABEL_SE_GETITEM,			// くっついた音
	SOUND_LABEL_SE_CHARGE,			// タイトルでモード選択時の音
	SOUND_LABEL_SE_SERECT,			// 選択音
	SOUND_LABEL_SE_TIME,			// 時間制限が近いとき
	SOUND_LABEL_SE_ENDGAME,			// ゲーム終了時
	SOUND_LABEL_SE_SERECT_POUSE,	// 選択音
	SOUND_LABEL_SE_ENTER,			// 決定音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// *****************************************************************************
// プロトタイプ宣言
// *****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
