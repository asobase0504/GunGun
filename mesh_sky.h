//=========================================
//
// メッシュ(空)の作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_SKY_H_
#define _MESH_SKY_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitMeshSky(void);				// メッシュの初期化処理
void UninitMeshSky(void);				// メッシュの終了処理
void UpdateMeshSky(void);				// メッシュの更新処理
void DrawMeshSky(void);				// メッシュの描画処理

void SetMeshSky(void);			// メッシュで円柱の作成設定

#endif // !_MESH_SKY_H_
