//============================
//
// アイテム設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

#define MAX_ITEMWAID (50.0f)	//マップのサイズ/分割数で出たマップサイズ計算例MIN-１５００MAX+１５００はなる３０００になる
#define MINX (-1500.0f)			
#define MINY (-1500.0f)
#define MAXX (1500.0f)
#define MAXY (1500.0f)

//プロトタイプ宣言
void InitItem(void);//初期化
void UninitItem(void);//破棄
void UpdateItem(void);//更新
void DrawItem(void);//描画
#endif