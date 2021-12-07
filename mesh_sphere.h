//=========================================
// 
// メッシュ(球)の作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_SPHERE_H_
#define _MESH_SPHERE_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitMeshSphere(void);				// メッシュの初期化処理
void UninitMeshSphere(void);				// メッシュの終了処理
void UpdateMeshSphere(void);				// メッシュの更新処理
void DrawMeshSphere(void);				// メッシュの描画処理

void SetMeshSphere(void);			// メッシュで円柱の作成設定

#endif // !_MESH_CYLINDER_H_
