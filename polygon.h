//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPolygon(void);		// ポリゴンの初期化処理
void UninitPolygon(void);	// ポリゴンの終了処理
void UpdatePolygon(void);	// ポリゴンの更新処理
void DrawPolygon(void);		// ポリゴンの描画処理
void SetPolygon(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3 size, char* texFile);		// ポリゴンの設定処理
D3DXVECTOR3 GetPolygonPos(void);		// ポリゴンの取得処理

#endif // !_POLYGON_H_
