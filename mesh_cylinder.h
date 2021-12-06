//=========================================
// 
// メッシュ(円柱)の作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_CYLINDER_H_
#define _MESH_CYLINDER_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitMeshCylinder(void);				// メッシュの初期化処理
void UninitMeshCylinder(void);				// メッシュの終了処理
void UpdateMeshCylinder(void);				// メッシュの更新処理
void DrawMeshCylinder(void);				// メッシュの描画処理

void SetMeshCylinder(void);			// メッシュで円柱の作成設定

#endif // !_MESH_CYLINDER_H_
