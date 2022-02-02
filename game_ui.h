//=========================================
// 
// リザルトのUI処理(ヘッダーファイル)
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
// プロトタイプ宣言
//------------------------------------
void InitGameUI(void);		// ゲームUIの初期化処理
void UninitGameUI(void);	// ゲームUIの終了処理
void UpdateGameUI(void);	// ゲームUIの更新処理
void DrawGameUI(void);		// ゲームUIの描画処理
void SetGameUITex(char* file);		// ゲームUIのテクスチャの変更処理

#endif // !_GAME_UI_H_
