//=========================================
// 
// 共通処理
// Author YudaKaito
// 
//=========================================
#include "common.h"

//=========================================
// 角度の正規化
//=========================================
float NormalizeRot(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2;
	}

	return rot;
}

//=========================================
// 加算合成なしの描写(長方形)
//=========================================
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// テクスチャの設定
	pDevice->SetTexture(0, Texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt, 2);
}

//=========================================
// 加算合成有りの描写(長方形)
//=========================================
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	RectDraw(pDevice, Texture, nCnt);

	// aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=========================================
// モデルのサイズを算出
//=========================================
void ModelSize(D3DXVECTOR3* Min, D3DXVECTOR3* Max, LPD3DXMESH Mesh)
{
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファーへのポイント

	nNumVtx = Mesh->GetNumVertices();	// 頂点数の取得

	sizeFVF = D3DXGetFVFVertexSize(Mesh->GetFVF());	// 頂点フォーマットのサイズを取得

	// 初期化
	*Min = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	*Max = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点バッファーのロック
	Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < Min->x)
		{
			Min->x = vtx.x;
		}
		if (vtx.y < Min->y)
		{
			Min->y = vtx.y;
		}
		if (vtx.z < Min->z)
		{
			Min->z = vtx.z;
		}
		if (vtx.x > Max->x)
		{
			Max->x = vtx.x;
		}
		if (vtx.y > Max->y)
		{
			Max->y = vtx.y;
		}
		if (vtx.z > Max->z)
		{
			Max->z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファーのアンロック
	Mesh->UnlockVertexBuffer();
}

//=========================================
// 線分同士の当たり判定
//=========================================
bool SegmentColision(Segment seg1, Segment seg2)
{
	return false;
}
