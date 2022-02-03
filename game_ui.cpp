//=========================================
// 
// リザルトのUI処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "game_ui.h"
#include "game.h"
#include "common.h"
#include "player.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SCORE_BG_TEX		"data/TEXTURE/Circle.png"
#define MODELCOUNT_MAX		(3)	// モデル数のスコアの最大桁数
#define SCORELENGTH_MAX		(3)	// 長さスコアの最大桁数

//------------------------------------
// UIの構造体
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 vtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 tex;				// テクスチャへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 角度
	D3DXCOLOR col;						// 色
	float Height;						// 高さ
	float Width;						// 幅
	bool bUse;							// 使用してるかどうか
}Object;

//------------------------------------
// 静的変数宣言
//------------------------------------
static Object uiLengthScoreBg;
static Object uiLengthUnitBg;
static Object uiGetModelUnitBg[2] = {};

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void UninitObject(Object *object);	// 終了

//=========================================
// 初期化
//=========================================
void InitGameUI(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	Object* object;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

	// 長さのスコアを置く場所の背景
	object = &(uiLengthScoreBg);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 120.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	object->Height = 250.0f;							// 高さ
	object->Width = 250.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);														// rhwの設定
		InitRectTex(pVtx);														// テクスチャ座標の設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

	// 長さのスコアの長さ単位を出す。
	object = &(uiLengthUnitBg);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(170.0f, 170.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 色
	object->Height = 150.0f;							// 高さ
	object->Width = 150.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/mm.png", &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);														// rhwの設定
		InitRectTex(pVtx);														// テクスチャ座標の設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

	// 長さのスコアを置く場所の背景
	object = &(uiLengthScoreBg);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 120.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	object->Height = 250.0f;							// 高さ
	object->Width = 250.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);														// rhwの設定
		InitRectTex(pVtx);														// テクスチャ座標の設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

	// 手に入れたモデルを置く場所の背景
	object = &(uiGetModelUnitBg[0]);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 500.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);	// 色
	object->Height = 200.0f;							// 高さ
	object->Width = 200.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);														// rhwの設定
		InitRectTex(pVtx);														// テクスチャ座標の設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

	// 手に入れたモデルを置く場所の背景
	object = &(uiGetModelUnitBg[0]);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 500.0f, 30.0f);	// 位置
	object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);	// 色
	object->Height = 150.0f;							// 高さ
	object->Width = 150.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// テクスチャの読込

																	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);														// rhwの設定
		InitRectTex(pVtx);														// テクスチャ座標の設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

	// 手に入れたモデルを置く場所の背景
	object = &(uiGetModelUnitBg[1]);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(110.0f, 500.0f, 30.0f);	// 位置
	object->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);	// 色
	object->Height = 200.0f;							// 高さ
	object->Width = 200.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);														// rhwの設定
		InitRectTex(pVtx);														// テクスチャ座標の設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();
}

//=========================================
// 終了
//=========================================
void UninitGameUI(void)
{
	UninitObject(&uiLengthScoreBg);
	UninitObject(&uiLengthUnitBg);
	UninitObject(&uiGetModelUnitBg[0]);
	UninitObject(&uiGetModelUnitBg[1]);
}

//=========================================
// 更新
//=========================================
void UpdateGameUI(void)
{
}

//=========================================
// 描画
//=========================================
void DrawGameUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Object* object;

	object = &(uiLengthScoreBg);
	if (object->bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiLengthUnitBg);
	if (object->bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiGetModelUnitBg[0]);
	if (object->bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, object->tex, 0);
	}

	object = &(uiGetModelUnitBg[1]);
	if (object->bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, object->tex, 0);
	}
}

//=========================================
// テクスチャの切り替え
//=========================================
void SetGameUITex(char * file)
{
	Object* object = &(uiLengthUnitBg);
	// テクスチャの破棄
	if (object->tex != NULL)
	{
		object->tex->Release();
		object->tex = NULL;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

	D3DXCreateTextureFromFile(pDevice, file, &object->tex);	// テクスチャの読込
}

//=========================================
// オブジェクトの終了
//=========================================
void UninitObject(Object * object)
{
	// テクスチャの破棄
	if (object->tex != NULL)
	{
		object->tex->Release();
		object->tex = NULL;
	}

	// 頂点バッファの破棄
	if (object->vtxBuff != NULL)
	{
		object->vtxBuff->Release();
		object->vtxBuff = NULL;
	}
}
