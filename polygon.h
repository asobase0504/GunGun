//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// ポリゴンの構造体を定義
//------------------------------------
typedef struct
{
	char* name;
	LPDIRECT3DVERTEXBUFFER9	VtxBuff;	// 頂点バッファーへのポインタ
	LPDIRECT3DTEXTURE9		Tex;		// テクスチャへのポインタ
	D3DXVECTOR3				pos;		// 頂点座標
	D3DXVECTOR3				size;		// 頂点座標
	D3DXVECTOR3				rot;		// 回転座標
	D3DXCOLOR				col;		// 色
	D3DXMATRIX				mtxWorld;	// ワールドマトリックス
	bool					bUse;		// 使用しているか

	bool					HitPlayer;	// プレイヤーにくっつくか否か
	D3DXQUATERNION			quaternion;	// クオータニオン
} ObjectPolygon;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPolygon(void);		// ポリゴンの初期化処理
void UninitPolygon(void);	// ポリゴンの終了処理
void UpdatePolygon(void);	// ポリゴンの更新処理
void DrawPolygon(void);		// ポリゴンの描画処理
void SetPolygon(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3* size, D3DXCOLOR* col, char* texFile, char* name);		// ポリゴンの設定処理
void DrawPolygonUI(void);	// ポリゴンのUI描画処理
void SetPolygonUI(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3 size, char* texFile);	// ポリゴンのUI設定処理
bool CollisionPolygon(D3DXVECTOR3* pos, char* name);		// ポリゴンの設定処理
ObjectPolygon* GetPolygon(char* name);			// ポリゴンの取得処理
D3DXVECTOR3 GetPolygonPos(void);		// ポリゴンの取得処理

#endif // !_POLYGON_H_
