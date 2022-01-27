//=========================================
// 
// リザルトの処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "player.h"

//------------------------------------
// リザルトの段階
//------------------------------------
typedef enum
{
	RESULT_0,
	RESULT_1,
	RESULT_MAX
}RESULT_MODE;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitResult(void);					// 初期化
void UninitResult(void);				// 終了
void UpdateResult(void);				// 更新
void DrawResult(void);					// 描画
RESULT_MODE* GetResult(void);			// モードの取得
void SetResultPlayer(Player player);	// リザルト用のプレイヤーデータの取得

#endif