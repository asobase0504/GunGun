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
// マクロ定義
//------------------------------------
#define RAD(deg)			(deg * D3DX_PI / 180.0f)

//------------------------------------
// プロトタイプ宣言
//------------------------------------

// 初期化系統

// 長方形処理(2D)
void SetVtxBuff2D(LPDIRECT3DVERTEXBUFFER9 *vtxBuff,int nData);
void InitRect(VERTEX_2D *vtx);	// 長方形の初期化
void InitRectPos(VERTEX_2D *vtx);	// 頂点座標初期化
void SetRectCenterPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// 中心座標が中心にあった場合の頂点バッファの座標設定
void SetRectCenterRotPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength);	// 中心座標が中心にあった場合の頂点バッファの座標設定(角度の変化に対応)
void SetRectUpLeftPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// 中心座標が左上にあった場合の頂点バッファの座標設定
void SetRectUpRightPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// 中心座標が右上にあった場合の頂点バッファの座標設定

void InitRectColor(VERTEX_2D *vtx);						// 頂点バッファの色の初期化
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor);	// 頂点バッファの色設定

void InitRectTex(VERTEX_2D *vtx);															// 頂点バッファのテクスチャ座標の初期化
void SetRectTex(VERTEX_2D *vtx, float top, float bottom, float left, float right);			// 頂点バッファのテクスチャ座標の設定

void InitRectRhw(VERTEX_2D *vtx);

// モデル系統
void ModelSize(D3DXVECTOR3* minOut, D3DXVECTOR3* maxOut, const LPD3DXMESH mesh);	// モデルのサイズを算出

// 正規化系統
float NormalizeRot(float* rot);	// 角度の正規化

// 描画系統
void InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);		// 描写前の処理
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 通常の描写
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 加算処理の描写

// 算出系統
bool isSharpAngle(D3DXVECTOR3* pos1, D3DXVECTOR3* pos2, D3DXVECTOR3* pos3);	// 鋭角か否か
float CalculateDistPointLine(D3DXVECTOR3* posPoint, D3DXVECTOR3* posLine, D3DXVECTOR3* vecLine, D3DXVECTOR3* posDist, float* fLength);	// 点と線分の最短距離を算出
float CalculateDistPointSegment(D3DXVECTOR3* posPoint, Segment* segment, D3DXVECTOR3* vecLine, D3DXVECTOR3* posDist, float* fLength);	// 点と線分の最短距離を算出

#endif // !_COMMON_H_
