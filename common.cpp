//=========================================
// 
// 共通処理
// Author YudaKaito
// 
//=========================================
#include "common.h"

//=========================================
// 長方形の初期化
//=========================================
void InitRect(VERTEX_2D * vtx)
{
	// 頂点座標の設定
	InitRectPos(vtx);

	// 頂点カラーの設定
	InitRectColor(vtx);

	// テクスチャ座標の設定
	InitRectTex(vtx);

	// rhw の設定
	InitRectRhw(vtx);
}

//=========================================
// 頂点バッファのPOSを初期化
//=========================================
void InitRectPos(VERTEX_2D * vtx)
{
	vtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// 頂点座標のPOSを設定(中心から)
//=========================================
void SetRectCenterPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x - fWidth;
	vtx[0].pos.y = pos.y - fHeigth;
	vtx[0].pos.z = pos.z + 0.0f;

	vtx[1].pos.x = pos.x + fWidth;
	vtx[1].pos.y = pos.y - fHeigth;
	vtx[1].pos.z = pos.z + 0.0f;

	vtx[2].pos.x = pos.x - fWidth;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z + 0.0f;

	vtx[3].pos.x = pos.x + fWidth;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z + 0.0f;
}

//=========================================
// 頂点座標を設定(中心)(角度対応)
//=========================================
void SetRectCenterRotPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength)
{
	//頂点座標の設定
	vtx[0].pos.x = pos.x + sinf(rot.z + (-D3DX_PI + fAngle)) * fLength;
	vtx[0].pos.y = pos.y + cosf(rot.z + (-D3DX_PI + fAngle)) * fLength;
	vtx[0].pos.z = 0.0f;

	vtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
	vtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
	vtx[1].pos.z = 0.0f;

	vtx[2].pos.x = pos.x + sinf(rot.z + (fAngle * -1.0f)) * fLength;
	vtx[2].pos.y = pos.y + cosf(rot.z + (fAngle * -1.0f)) * fLength;
	vtx[2].pos.z = 0.0f;

	vtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	vtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
	vtx[3].pos.z = 0.0f;

}

//=========================================
// 頂点座標を設定(左上から)
//=========================================
void SetRectUpLeftPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x;
	vtx[0].pos.y = pos.y;
	vtx[0].pos.z = pos.z;

	vtx[1].pos.x = pos.x + fWidth;
	vtx[1].pos.y = pos.y;
	vtx[1].pos.z = pos.z + 0.0f;

	vtx[2].pos.x = pos.x;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z + 0.0f;

	vtx[3].pos.x = pos.x + fWidth;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z + 0.0f;
}

//=========================================
// 頂点座標を設定(右上から)
//=========================================
void SetRectUpRightPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x - fWidth;
	vtx[0].pos.y = pos.y;
	vtx[0].pos.z = pos.z;

	vtx[1].pos.x = pos.x;
	vtx[1].pos.y = pos.y;
	vtx[1].pos.z = pos.z;

	vtx[2].pos.x = pos.x - fWidth;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z;

	vtx[3].pos.x = pos.x;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z;
}

//=========================================
// 頂点バッファのカラーを初期化
//=========================================
void InitRectColor(VERTEX_2D * vtx)
{
	for (int i = 0; i < 4; ++i)
	{
		vtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vtx++;
	}
}

//=========================================
// 頂点バッファのカラーを設定
//=========================================
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor)
{
	for (int i = 0; i < 4; ++i)
	{
		vtx->col = *inColor;
		vtx++;
	}
}

//=========================================
// 頂点バッファのテクスチャ座標の初期化
//=========================================
void InitRectTex(VERTEX_2D * vtx)
{
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=========================================
// 頂点バッファのテクスチャ座標の設定
//=========================================
void SetRectTex(VERTEX_2D * vtx, float top, float bottom, float left, float right)
{
	vtx[0].tex = D3DXVECTOR2(left, top);
	vtx[1].tex = D3DXVECTOR2(right, top);
	vtx[2].tex = D3DXVECTOR2(left, bottom);
	vtx[3].tex = D3DXVECTOR2(right, bottom);

}

//=========================================
// rhw の初期化
//=========================================
void InitRectRhw(VERTEX_2D * vtx)
{
	// rhw の設定
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;
}


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
// 描写処理に前提として必要な部分
//=========================================
void InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
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

////=========================================
//// 線分同士の当たり判定
////=========================================
//bool SegmentColision(Segment seg1, Segment seg2)
//{
//	// ベクトルの始点同士の距離。
//	D3DXVECTOR3 v = seg2.s - seg1.s;
//
//	// ブロックのベクトルと被対象のベクトルが平行か調べる
//	float Bv_Tv = D3DXVec2Cross(&(seg1.v), &(seg2.v));
//	if (Bv_Tv == 0.0f)
//	{
//		// 並行である。
//		return false;
//	}
//
//	D3DXVec3Cross()
//
//	float v_Bv = D3DXVec2Cross(&(v), &(seg1.v));
//	float v_Tv = D3DXVec2Cross(&(v), &(seg2.v));
//
//	float hit1 = v_Tv / Bv_Tv;
//	float hit2 = v_Bv / Bv_Tv;
//
//	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
//	{
//		return false;
//	}
//
//	if (Outpos != NULL)
//	{
//		*Outpos = seg1.s + seg1.v * hit1;
//	}
//	return true;
//}

//=========================================
// モデルのサイズを算出(球と球の当たり判定)
//=========================================
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
{
#if 1
	D3DXVECTOR3 v = pos1 - pos2;
	return D3DXVec3LengthSq(&v) <= (fLength1 + fLength2)*(fLength1 + fLength2);
#else
	return (pow((pos2.x - pos1.x), 2.0f) + pow((pos2.y - pos1.y), 2.0f) + pow((pos2.z - pos1.z), 2.0f)) <= (pow((fLength1 + fLength2), 2.0f));
#endif
}
