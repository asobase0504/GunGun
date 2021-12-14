//=========================================
// 
// プレイヤーの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//------------------------------------
// モデルの構造体定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	D3DXVECTOR3 rotDest;	// 目的の角度
	D3DXVECTOR3 movevec;	// ベクトル
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 axis;		// 回転軸
	float moverot;			// 移動時の回転量
}Player;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPlayer(void);		// モデルの初期化処理
void UninitPlayer(void);		// モデルの終了処理
void UpdatePlayer(void);		// モデルの更新処理
void DrawPlayer(void);		// モデルの更新処理
void MovePlayer(void);			// モデルの移動
Player* GetPlayer(void);			// モデルの取得処理

#endif // !_PLAYER_H_
