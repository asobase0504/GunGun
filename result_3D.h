//=========================================
// 
// リザルトの3D処理(ヘッダーファイル)
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
void InitResult3D(void);	// リザルト3Dの初期化処理
void UninitResult3D(void);	// リザルト3Dの終了処理
void UpdateResult3D(void);	// リザルト3Dの更新処理
void DrawResult3D(void);	// リザルト3Dの描画処理

#endif // !_RESULT_UI_H_
