//=========================================
// 
// モデルの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//------------------------------------
// モデルの構造体定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	D3DXVECTOR3 rotDest;	// 目的の角度
	D3DXVECTOR3 vec;		// ベクトル
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 MinVtx;		// 頂点の最小値
	D3DXVECTOR3 MaxVtx;		// 頂点の最大値
}Model;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitModel(void);		// モデルの初期化処理
void UninitModel(void);		// モデルの終了処理
void UpdateModel(void);		// モデルの更新処理
void DrawModel(void);		// モデルの更新処理
void MoveModel(void);			// モデルの移動
Model* GetModel(void);			// モデルの取得処理

#endif // !_MODEL_H_
