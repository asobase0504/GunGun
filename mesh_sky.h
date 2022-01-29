//==================================================
//
// 3Dゲーム制作 ( mesh_sky.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MESH_SKY_H_		// このマクロ定義がされてなかったら
#define _MESH_SKY_H_		// ２重インクルード防止のマクロ定義

//--------------------------------------------------
// 構造体を定義
//--------------------------------------------------

/*↓ メッシュ空 ↓*/

typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
}MeshSky;

/*↓ メッシュ空の数系 ↓*/

typedef struct
{
	int		nHorizontal;		// 横
	int		nVertical;			// 縦
	int		nVtx;				// 頂点数
	int		nIdx;				// インデックス数
	int		nPolygon;			// ポリゴン数
}MeshSkyNumber;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMeshSky(void);					// 初期化
void UninitMeshSky(void);				// 終了
void UpdateMeshSky(void);				// 更新
void DrawMeshSky(void);					// 描画
void SetMeshSky(void);					// 設定
MeshSky *GetMeshSky(void);				// 位置等の情報取得
MeshSkyNumber *GetMeshSkyNumber(void);	// 基礎情報取得

#endif // !_MESH_SKY_H_
