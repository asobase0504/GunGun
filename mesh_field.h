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
void InitMeshBuild(void);				// メッシュの初期化処理
void UninitMeshBuild(void);				// メッシュの終了処理
void UpdateMeshBuild(void);				// メッシュの更新処理
void DrawMeshBuild(void);				// メッシュの描画処理
D3DXVECTOR3 GetMeshBuildPos(void);		// メッシュの取得処理

void SetMeshRect(void);				// メッシュで長方形の作成設定
void SetMeshCylinder(void);			// メッシュで円柱の作成設定

#endif // !_MESH_FIELD_H_
