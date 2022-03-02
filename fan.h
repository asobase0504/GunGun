//==================================================
//
// 扇状の円
// Author : Yuda Kaito
//
//==================================================
#ifndef _FAN_H_		// このマクロ定義がされてなかったら
#define _FAN_H_		// ２重インクルード防止のマクロ定義

#include "main.h"
//--------------------------------------------------
// 構造体を定義
//--------------------------------------------------
/*↓ メッシュ空 ↓*/
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	D3DXMATRIX		mtxWorld;		// ワールドマトリックス
	float fLength;					// 長さ
}Fan;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitFan(void);					// 初期化
void UninitFan(void);				// 終了
void UpdateFan(void);				// 更新
void DrawFan(void);					// 描画
void SetFan(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength);					// 設定
void OpenFan(int nCnt);						// 扇状に開く
void CloseFan(int nCnt);					// 扇状に閉じる
Fan *GetFan(void);				// 位置等の情報取得

#endif // !_FAN_H_
