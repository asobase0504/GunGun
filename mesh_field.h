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
// セットメッシュの構造体を定義
//------------------------------------
typedef struct
{
	char* file;
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 rot;		// 回転座標
	int nSurfaceWidth;		// 面の幅
	int nSurfaceHeight;		// 面の高さ
	float fLineWidth;		// 辺の幅
	float fLineHeight;		// 辺の高さ
} SetMesh;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitMeshField(void);				// メッシュの初期化処理
void UninitMeshField(void);				// メッシュの終了処理
void UpdateMeshField(void);				// メッシュの更新処理
void DrawMeshField(void);				// メッシュの描画処理
void SetMeshField(SetMesh* mesh);		// メッシュの描画処理
D3DXVECTOR3 GetMeshFieldPos(void);		// メッシュの取得処理
void CollisionMeshField(D3DXVECTOR3* pos);						// メッシュフィールドの当たり判定
void CollisionMeshField(D3DXVECTOR3* pos, D3DXVECTOR3* HitPos);	// 戻すposと当たり判定のposが違う場合

#endif // !_MESH_FIELD_H_
