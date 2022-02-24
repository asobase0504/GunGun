//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//------------------------------------
// 影の構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 rot;		// 回転座標
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	float size;				// 半径
	bool bUse;				// 使用状況
} Shadow;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitShadow(void);			// 影の初期化処理
void UninitShadow(void);		// 影の終了処理
void UpdateShadow(void);		// 影の更新処理
void DrawShadow(void);			// 影の描画処理
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot);			// 影の設定処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);	// 影位置の設定処理
Shadow* GetShadow(int nIdxShadow);			// 影の取得処理

#endif // !_SHADOW_H_
