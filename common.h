//=========================================
// 
// 共通処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _COMMON_H_
#define _COMMON_H_

#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------

// 初期化系統

// モデル系統
void ModelSize(D3DXVECTOR3 *Min, D3DXVECTOR3 *Max, LPD3DXMESH Mesh);	// モデルのサイズを算出

// 正規化系統
float NormalizeRot(float rot);	// 角度の正規化

// 描画系統
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 通常の描写
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 加算処理の描写

#endif // !_COMMON_H_
