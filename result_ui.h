//=========================================
// 
// リザルトのUI処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _RESULT_UI_H_
#define _RESULT_UI_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitResultUI(void);	// リザルトUIの初期化処理
void UninitResultUI(void);	// リザルトUIの終了処理
void UpdateResultUI(void);	// リザルトUIの更新処理
void DrawResultUI(void);	// リザルトUIの描画処理

#endif // !_RESULT_UI_H_
