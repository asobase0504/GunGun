//=========================================
// 
// 処理の設定
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"

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
