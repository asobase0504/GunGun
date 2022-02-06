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
float NormalizeRot(float* rot)
{
	if (*rot > D3DX_PI)
	{
		*rot -= D3DX_PI * 2;
	}
	if (*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2;
	}

	return *rot;
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
// 引数1 minOut	モデルの最小頂点の出力
// 引数2 maxOut	モデルの最大頂点の出力
// 引数3 mesh	サイズを計測するモデル
//=========================================
void ModelSize(D3DXVECTOR3* minOut, D3DXVECTOR3* maxOut, LPD3DXMESH mesh)
{
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファーへのポイント

	nNumVtx = mesh->GetNumVertices();	// 頂点数の取得

	sizeFVF = D3DXGetFVFVertexSize(mesh->GetFVF());	// 頂点フォーマットのサイズを取得

	// 初期化
	*minOut = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	*maxOut = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 頂点バッファーのロック
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < minOut->x)
		{
			minOut->x = vtx.x;
		}
		if (vtx.y < minOut->y)
		{
			minOut->y = vtx.y;
		}
		if (vtx.z < minOut->z)
		{
			minOut->z = vtx.z;
		}
		if (vtx.x > maxOut->x)
		{
			maxOut->x = vtx.x;
		}
		if (vtx.y > maxOut->y)
		{
			maxOut->y = vtx.y;
		}
		if (vtx.z > maxOut->z)
		{
			maxOut->z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファーのアンロック
	mesh->UnlockVertexBuffer();
}

//=========================================
// 鋭角か否か
// 引数1	pos1	点の位置
// 引数2	pos2	直線の位置
// 引数3	pos3	直線のベクトル
// 返り値	鋭角か、否か
//=========================================
bool isSharpAngle(D3DXVECTOR3* pos1, D3DXVECTOR3* pos2, D3DXVECTOR3* pos3)
{
	D3DXVECTOR3 vec1 = *pos2 - *pos1;
	D3DXVECTOR3 vec2 = *pos2 - *pos3;
	D3DXVec3Dot(&vec1, &vec2);
	return false;
}

//=========================================
// 点と直線の最短距離を算出
// 引数1	posPoint	点の位置
// 引数2	posLine		直線の位置
// 引数3	vecLine		直線のベクトル
// 引数3	posDist		最短距離の位置
// 引数3	fLength		サイズを計測するモデル(関数内で、初期化する)
// 返り値	最短距離の長さ
//=========================================
float CalculateDistPointLine(D3DXVECTOR3* posPoint, D3DXVECTOR3* posLine, D3DXVECTOR3* vecLine, D3DXVECTOR3* posDist, float* fLength)
{
	float fVecLineLength = D3DXVec3LengthSq(vecLine);
	*fLength = 0.0f;
	if (fVecLineLength > 0.0f)
	{
		D3DXVECTOR3 vec = *posPoint - *posLine;
		*fLength = D3DXVec3Dot(vecLine, &vec);
	}
	D3DXVECTOR3 vec = *posPoint - *posDist;
	return D3DXVec3Length(&vec);
}
