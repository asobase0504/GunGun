//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitMeshBuild(void);				// ポリゴンの初期化処理
void UninitMeshBuild(void);				// ポリゴンの終了処理
void UpdateMeshBuild(void);				// ポリゴンの更新処理
void DrawMeshBuild(void);				// ポリゴンの描画処理
D3DXVECTOR3 GetMeshBuildPos(void);		// ポリゴンの取得処理

#endif // !_MESH_FIELD_H_
