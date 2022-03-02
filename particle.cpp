//=========================================
// 
// パーティクルの処理
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "common.h"
#include "particle.h"
#include <assert.h>

//-----------------------------------------
// マクロ定義
//-----------------------------------------
#define MAX_PARTCLE	(1024)	// パーティクルの最大数
#define MAX_TEX		(MAX_PARTICLE)		// テクスチャの種類
#define JUMP_TEX	"data/TEXTURE/Circle_01.png"

//-----------------------------------------
// パーティクル構造体
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 角度
	D3DXCOLOR col;		// 色
	float fRaduus;		// 半径
	float fLength;		// 中心座標からの半径
	float fAngle;		// 角度の算出
	int nMaxLife;		// 最大寿命
	int nLife;			// 寿命
	PARTICLE_TYPE type;	// パーティクルの種類
	bool bUse;			// 使用しているかどうかe
}Particle;

//-----------------------------------------
// static変数
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_PARTICLE] = {};		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static Particle s_aParticle[MAX_PARTCLE];
static float s_fAngle;

//-----------------------------------------
// プロトコール
//-----------------------------------------
#if 0
float angle = rand() / (float)RAND_MAX * 2.0f * D3DX_PI;
float length = 50.0f + rand() / (float)RAND_MAX * 50.0f;
float diffX = sinf(angle) * length;
float diffY = cosf(angle) * length;
float speed = ? ? ? ? ;
float life = length / speed;

pParticle->pos.x = pos.x + diffX;
pParticle->pos.y = pos.y + diffY;
pParticle->move.x = sinf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.y = cosf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.z = 0.0f;
pParticle->col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
pParticle->fRaduus = 10.0f;
pParticle->nLife = (rand() % 20) + 10;
#endif

//=========================================
// パーティクルの初期化処理
//=========================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		JUMP_TEX,
		&s_pTexture[PARTICLE_PLAYER_GETMODEL]);

	ZeroMemory(s_aParticle, sizeof(s_aParticle));;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* vtxBase = pVtx;

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		assert(pVtx + 3 < vtxBase + (4 * MAX_PARTCLE));

		pParticle = &(s_aParticle[nCntParticle]);

		// 頂点座標の設定
		SetPosRectCenter(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(pParticle->col));

		// テクスチャ座標の設定
		InitRectTex(pVtx);
		
		// rhwの設定
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
// パーティクルの終了処理
//=========================================
void UninitParticle(void)
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// テクスチャの破棄
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//=========================================
// パーティクルの更新処理
//=========================================
void UpdateParticle(void)
{
	Particle *pParticle;

	for (int i = 0; i < MAX_PARTCLE; i++)
	{
		pParticle = s_aParticle + i;

		if (!pParticle->bUse)
		{	// パーティクルが使用されていない
			continue;
		}

		// パーティクルが使用されている
		// 位置の更新
		pParticle->pos += pParticle->move;

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_GETMODEL:	// プレイヤーのジャンプパーティクル	// 列挙型に変更する。
			pParticle->nLife--;			// 体力の更新
			pParticle->col.a -= (float)1.0f / pParticle->nMaxLife;			// 透明度の更新
			pParticle->fRaduus += 0.75f;	// 半径の拡大
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		default:
			assert(false);
			break;
		}

		// 中心座標から上の長さを算出する。
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// 中心座標から上の頂点の角度を算出する
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		// 頂点座標の設定
		SetPosRectCenterRot(pVtx, pParticle->pos, pParticle->rot, pParticle->fAngle, pParticle->fLength);
		// 頂点カラーの設定
		SetRectColor(pVtx, &(pParticle->col));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

	}
}

//=========================================
// パーティクルの描画処理
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	Particle *pParticle;

	// 描画のための設定
	InitDraw(pDevice, s_pVtxBuff);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (!pParticle->bUse)
		{// パーティクルが使用されている
			continue;
		}

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_GETMODEL:	// プレイヤーのジャンプ時
			// テクスチャを加算合成で貼り付けて描画する
			RectDraw(pDevice, s_pTexture[pParticle->type], nCntParticle * 4);
			break;
		default:
			assert(false);
			break;
		}
	}
}

//=========================================
// パーティクルの設定処理
//=========================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLE_TYPE type)
{
	Particle *pParticle;
	int nCntParticle;


	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (pParticle->bUse)
		{// パーティクルが使用されてる
			continue;
		}

		ZeroMemory(pParticle, sizeof(s_aParticle[nCntParticle]));

		// パーティクルが使用されていない
		pParticle->type = type;
		switch (type)
		{
		case PARTICLE_PLAYER_GETMODEL:	// プレイヤーのジャンプパーティクル
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->move.z = 0.0f;
			pParticle->col = col;
			pParticle->fRaduus = 300.0f;
			pParticle->nMaxLife = 60;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntParticle * 4;

		// 中心座標から上の長さを算出する。
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// 中心座標から上の頂点の角度を算出する
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		pParticle->bUse = true;

		SetPosRectCenter(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		SetRectColor(pVtx, &(pParticle->col));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}
