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
void CollisionMeshField(D3DXVECTOR3* pos);						// メッシュフィールドの当たり判定
void CollisionMeshField(D3DXVECTOR3* pos, D3DXVECTOR3* HitPos);	// 戻すposと当たり判定のposが違う場合

#endif // !_MESH_FIELD_H_
