//=========================================
// 
// 影の処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "shadow.h"
#include "common.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SHADOW_MAX	(5)

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};	// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9 s_pTexture = {};		// テクスチャへのポインタ
static Shadow s_aShadow[SHADOW_MAX];			// ポリゴンの構造体

//=========================================
// 初期化
//=========================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化処理
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		s_aShadow[i].pos = ZERO_VECTOR;		// 頂点座標
		s_aShadow[i].rot = ZERO_VECTOR;		// 回転座標
		s_aShadow[i].bUse = false;			// 使用していない状態
	}

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * SHADOW_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	for (int i = 0; i < SHADOW_MAX; i++)
	{
		VERTEX_3D* pVtx;

		// 頂点座標をロック
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(s_aShadow[i].pos.x - 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + 5.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aShadow[i].pos.x + 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + 5.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aShadow[i].pos.x - 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + -10.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aShadow[i].pos.x + 5.0f, s_aShadow[i].pos.y + 0.0f, s_aShadow[i].pos.z + -10.0f);

		// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点座標をアンロック
		s_pVtxBuff->Unlock();

	}
}

//=========================================
// 終了
//=========================================
void UninitShadow(void)
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
}

//=========================================
// 更新
//=========================================
void UpdateShadow(void)
{
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		VERTEX_3D* pVtx;

		// 頂点座標をロック
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(- s_aShadow[i].size, 0.0f, + s_aShadow[i].size);
		pVtx[1].pos = D3DXVECTOR3(+ s_aShadow[i].size, 0.0f, + s_aShadow[i].size);
		pVtx[2].pos = D3DXVECTOR3(- s_aShadow[i].size, 0.0f, - s_aShadow[i].size);
		pVtx[3].pos = D3DXVECTOR3(+ s_aShadow[i].size, 0.0f, - s_aShadow[i].size);

		// 頂点座標をアンロック
		s_pVtxBuff->Unlock();
	}
}

//=========================================
// 描画
//=========================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		Shadow* pShadow = &s_aShadow[i];

		if (!(pShadow->bUse))
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pShadow->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);	// ワールド座標行列の設定

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// 描画処理
		RectDraw(pDevice, s_pTexture, i * 4);

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================
// 設定
//=========================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int i;
	for (i = 0; i < SHADOW_MAX; i++)
	{
		if (s_aShadow[i].bUse)
		{	// 既に使用している場合
			continue;
		}

		s_aShadow[i].pos = pos;
		s_aShadow[i].pos.y = 0.1f;
		s_aShadow[i].rot = rot;
		s_aShadow[i].bUse = true;

		break;
	}
	return i;
}

//=========================================
// 影位置の設定
//=========================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	if (s_aShadow[nIdxShadow].bUse)
	{	// 既に使用している場合
		s_aShadow[nIdxShadow].pos = pos;
		s_aShadow[nIdxShadow].pos.y = 0.1f;
	}
}

//=========================================
// 影位置の設定
//=========================================
Shadow* GetShadow(int nIdxShadow)
{
	return &(s_aShadow[nIdxShadow]);
}
