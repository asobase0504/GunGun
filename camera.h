//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクトマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
}Camera;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitCamera(void);		// ポリゴンの初期化処理
void UninitCamera(void);	// ポリゴンの終了処理
void UpdateCamera(void);	// ポリゴンの更新処理
void SetCamera(void);		// ポリゴンの描画処理

#endif // !_CAMERA_H_
