//=========================================
// 
// デバッグ用のラインの作成
// Author YudaKaito
// 
//=========================================
#include "Line.h"
#include "main.h"

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// テクスチャへのポインタ
static int s_nShadowCnt;						// 影の割り当て

void InitLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 40,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

}

void UninitLine(void)
{
}

void UpdateLine(void)
{
}

void DrawLine(void)
{

}
