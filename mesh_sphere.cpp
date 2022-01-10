//=========================================
// 
// メッシュ(球)の作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "mesh_sphere.h"
#include "polygon.h"
#include "input.h"
#include "common.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SIZE	(50.0f)

//------------------------------------
// ポリゴンの種類の列挙型
//------------------------------------

//------------------------------------
// メッシュの構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 rot;	// 回転座標
	int nSurfaceWidth;	// 面の幅数
	int nSurfaceHeight;	// 面の高さ数
	float fLineWidth;	// 辺の幅
	float fLineHeight;	// 辺の高さ
	int vertexCnt;		// 頂点数
	int polygonCnt;		// ポリゴン数
	int IdxCnt;			// インデックス数
	D3DXMATRIX mtxWorld;// ワールドマトリックス
} Mesh;

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9 s_pTexture = {};			// テクスチャへのポインタ
static LPDIRECT3DINDEXBUFFER9 s_pIdxBuff = NULL;	// インデックスバッファへのポインタ
static Mesh s_aMesh[2] = {};						// ポリゴンの構造体

//=========================================
// 初期化
//=========================================
void InitMeshSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// nSurfaceWidth × nSurfaceHeight
	s_aMesh[0].nSurfaceWidth = 10;					// 横軸の面の数
	s_aMesh[0].nSurfaceHeight = 10;					// 縦軸の面の数
	s_aMesh[0].fLineWidth = 10.0f;					// 横軸の面の数
	s_aMesh[0].fLineHeight = 10.0f;					// 縦軸の面の数

	int nLineVtx = (s_aMesh[0].nSurfaceWidth + 1);	// 横軸の頂点数

	s_aMesh[0].vertexCnt = nLineVtx * (s_aMesh[0].nSurfaceHeight + 1);	// 頂点数

	// ポリゴン数を求める計算
	s_aMesh[0].polygonCnt
		= 2 * s_aMesh[0].nSurfaceWidth * s_aMesh[0].nSurfaceHeight		// 一行分のポリゴン数
		+ 4 * (s_aMesh[0].nSurfaceHeight - 1);							// 縮退ポリゴン数

	s_aMesh[0].IdxCnt = s_aMesh[0].polygonCnt + 2;	// インデックス数

	//別解
	/*
	// インデックス数を求める計算
	s_aMesh[0].IdxCnt
	= 2 * (s_aMesh[0].nSurfaceWidth + 1) * s_aMesh[0].nSurfaceHeight	// 一行分のインデックス数
	+ 2 * (s_aMesh[0].nSurfaceHeight - 1);					// 改行時に発生する重複しているインデックス数

	s_aMesh[0].polygonCnt = s_aMesh[0].IdxCnt - 2;	// ポリゴン数
	*/

	// 初期化処理
	s_aMesh[0].pos = ZERO_VECTOR;	// 頂点座標
	s_aMesh[0].rot = ZERO_VECTOR;	// 回転座標

									// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/暗転.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * s_aMesh[0].polygonCnt,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * s_aMesh[0].IdxCnt,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nHeight = 0; nHeight <= s_aMesh[0].nSurfaceHeight; nHeight++)
	{
		float fRotHeight = D3DX_PI / s_aMesh[0].nSurfaceHeight;
		float fHeight = cosf(fRotHeight * nHeight) * s_aMesh[0].fLineHeight;
		for (int nWidth = 0; nWidth <= s_aMesh[0].nSurfaceWidth; nWidth++)
		{
			float fRotWidth = 2.0f * D3DX_PI / s_aMesh[0].nSurfaceWidth * nWidth;
			NormalizeRot(fRotWidth);

			pVtx[nWidth + nHeight * nLineVtx].pos.x = cosf(fRotWidth) * sinf(fRotHeight * nHeight) * s_aMesh[0].fLineHeight;
			pVtx[nWidth + nHeight * nLineVtx].pos.y = fHeight;
			pVtx[nWidth + nHeight * nLineVtx].pos.z = sinf(fRotWidth) * sinf(fRotHeight * nHeight) * s_aMesh[0].fLineHeight;

			pVtx[nWidth + nHeight * nLineVtx].pos.x += 25.0f;
			pVtx[nWidth + nHeight * nLineVtx].pos.y += 25.0f;
			pVtx[nWidth + nHeight * nLineVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nWidth + nHeight * nLineVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nWidth + nHeight * nLineVtx].tex = D3DXVECTOR2((float)nWidth, (float)nHeight);
		}
	}

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

	WORD* pIdx;
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスを求める
	for (int Y = 0; Y < s_aMesh[0].nSurfaceHeight; Y++)
	{
		int nlineTop = Y * (nLineVtx * 2 + 2);
		for (int X = 0; X <= s_aMesh[0].nSurfaceWidth; X++)
		{
			int nIdxData = X * 2 + nlineTop;
			pIdx[nIdxData + 1] = X + nLineVtx * Y;
			pIdx[nIdxData] = pIdx[nIdxData + 1] + nLineVtx;
		}

		if (Y < s_aMesh[0].nSurfaceHeight - 1)
		{
			pIdx[nLineVtx * 2 + 0 + nlineTop] = s_aMesh[0].nSurfaceWidth + nLineVtx * Y;
			pIdx[nLineVtx * 2 + 1 + nlineTop] = nLineVtx * 2 + nLineVtx * Y;
		}
	}

	s_pIdxBuff->Unlock();
}

//=========================================
// 終了
//=========================================
void UninitMeshSphere(void)
{
	// テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 頂点バッファーの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (s_pIdxBuff != NULL)
	{
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateMeshSphere(void)
{
}

//=========================================
// 描画
//=========================================
void DrawMeshSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_aMesh[0].mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aMesh[0].rot.y, s_aMesh[0].rot.x, s_aMesh[0].rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_aMesh[0].mtxWorld, &s_aMesh[0].mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_aMesh[0].pos.x, s_aMesh[0].pos.y, s_aMesh[0].pos.z);			// 行列移動関数(第１引数にnWidth,Y,nHeight方向の移動行列を作成)
	D3DXMatrixMultiply(&s_aMesh[0].mtxWorld, &s_aMesh[0].mtxWorld, &mtxTrans);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_aMesh[0].mtxWorld);	// ワールド座標行列の設定

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, s_aMesh[0].vertexCnt, 0, s_aMesh[0].polygonCnt);

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}
