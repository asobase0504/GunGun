//==================================================
//
// 扇状の円
// Author : Yuda Kaito
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "fan.h"
#include "common.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define START_HORIZONTAL		(10)		// 横の最初の値
#define MAX_VERTEX				(100)		// 頂点数

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;		// 円の頂点バッファへのポインタ
static Fan						s_Fan;					// メッシュ空の情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitFan(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\sky.jpg",&s_pTexture);

	memset(&s_Fan, NULL, sizeof(s_Fan));	// メモリのクリア
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitFan(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 円の頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateFan(void)
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawFan(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_Fan.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Fan.rot.y, s_Fan.rot.x, s_Fan.rot.z);
	D3DXMatrixMultiply(&s_Fan.mtxWorld, &s_Fan.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_Fan.pos.x, s_Fan.pos.y, s_Fan.pos.z);
	D3DXMatrixMultiply(&s_Fan.mtxWorld, &s_Fan.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_Fan.mtxWorld);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,MAX_VERTEX - 2);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetFan(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタの取得
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	D3DXCOLOR col(0.0f, 0.55f, 1.0f, 1.0f);

	// メモリのクリア
	memset(&s_Fan, NULL, sizeof(s_Fan));

	s_Fan.rot = rot;
	s_Fan.pos = pos;
	s_Fan.fLength = fLength;

	// 円の頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = pos;

	D3DXVECTOR3 nor;

	// 正規化する ( 大きさ 1 のベクトルにする )
	D3DXVec3Normalize(&nor, &pos);

	pVtx[0].nor = nor;	// 各頂点の法線の設定
	pVtx[0].col = col;	// 頂点カラーの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// テクスチャ座標の設定

	DEBUG_PRINT("%f	%f\n", pVtx[0].pos.x, pVtx[0].pos.z);

	for (int i = 1; i < MAX_VERTEX; i++)
	{
		float fRot = (D3DX_PI * 2.0f / (MAX_VERTEX-2)) * (i - 1);

		// 角度の正規化
		NormalizeRot(&fRot);

		// 頂点座標の設定
		pVtx[i].pos.x = pos.x + sinf(fRot) * fLength;
		pVtx[i].pos.y = pos.y;
		pVtx[i].pos.z = pos.z + cosf(fRot) * fLength;

		// 正規化する ( 大きさ 1 のベクトルにする )
		D3DXVec3Normalize(&nor, &pVtx[i].pos);

		// 各頂点の法線の設定
		pVtx[i].nor = nor;

		// 頂点カラーの設定
		pVtx[i].col = col;

		// テクスチャ座標の設定
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		DEBUG_PRINT("%f	%f\n", pVtx[i].pos.x, pVtx[i].pos.z);
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 扇状に開く
//--------------------------------------------------
void OpenFan(int nCnt)
{
	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ
	
	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 1; i < MAX_VERTEX; i++)
	{
		if (i < nCnt)
		{
			continue;
		}

		float fRot = (D3DX_PI * 2.0f / (MAX_VERTEX - 2)) * nCnt;

		// 角度の正規化
		NormalizeRot(&fRot);

		// 頂点座標の設定
		pVtx[i].pos.x = s_Fan.pos.x + sinf(fRot) * s_Fan.fLength;
		pVtx[i].pos.z = s_Fan.pos.z + cosf(fRot) * s_Fan.fLength;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 扇上に閉じる
//--------------------------------------------------
void CloseFan(int nCnt)
{
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Fan *GetFan(void)
{
	return &s_Fan;
}