//=========================================
// 
// プレイヤーの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "model.h"

//------------------------------------
// モデルパーツの構造体定義
//------------------------------------
#define PARTS_NUM	(255)		// パーツの数

//------------------------------------
// モデルの構造体定義
//------------------------------------
typedef struct
{
	D3DXMATRIX	mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3	pos;				// 位置
	D3DXVECTOR3	pos_old;			// 前回位置
	D3DXVECTOR3	rot;				// 角度
	D3DXVECTOR3	rotDest;			// 目的の角度
	D3DXVECTOR3	movevec;			// ベクトル
	D3DXVECTOR3	MinVtx;				// 頂点の最小値
	D3DXVECTOR3	MaxVtx;				// 頂点の最大値
	Model*		aModel[PARTS_NUM];	// モデル
	float		moverot;			// 移動時の回転量
	float		fLength;			// 大きさ
	float		fLengthLand;		// 目標の大地までの距離
	int			nModelCnt;			// くっついているモデル数
}Player;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPlayer(void);		// プレイヤーの初期化処理
void UninitPlayer(void);	// プレイヤーの終了処理
void UpdatePlayer(void);	// プレイヤーの更新処理
void DrawPlayer(void);		// プレイヤーの更新処理
void MovePlayer(void);		// プレイヤーの移動
void DeleteModel(void);		// プレイヤー以外のモデルを
Player* GetPlayer(void);	// プレイヤーの取得処理
Model** GetPlayerModel(void);	// プレイヤーモデルの取得処理
#endif // !_PLAYER_H_
