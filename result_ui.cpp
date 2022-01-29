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
#include "result_ui.h"
#include "result.h"
#include "common.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SELECTBG			"data/TEXTURE/number002.png"
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
static Object nextButton;
static Object scoreLength;
static Object scoreModel;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
//static void InitObject(Object *object);		// 初期化
static void UninitObject(Object *object);	// 終了
//static void UpdateObject(Object *object);	// 更新
//static void DrawObject(Object *object);		// 描画

//=========================================
// 初期化
//=========================================
void InitResultUI(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	Object* object;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

	//
	// 次へのボタン
	//
	object = &(nextButton);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(10.0f, 10.0f, 0.0f);		// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	object->Height = 10.0f;								// 高さ
	object->Width = 10.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);
	
	//
	// 長さのスコア
	//
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

	//
	// 長さのスコア
	//
	object = &(scoreLength);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 300.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	object->Height = 60.0f;									// 高さ
	object->Width = 40.0f;									// 幅
	object->bUse = true;									// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORELENGTH_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float aTex[SCORELENGTH_MAX];
		int aNumber[SCORELENGTH_MAX];
		int nStep = 0;	// 段階を確認
		int nLengthScore = (int)GetPlayer()->fLength;	// スコア

		while(nLengthScore / 100 != 0)
		{
			nStep++;
			nLengthScore /= 100;
		}

		for (int i = 0; i < SCORELENGTH_MAX; i++)
		{
			SetRectUpRightPos(pVtx, D3DXVECTOR3(object->pos.x - 25.0f * i, object->pos.y, object->pos.z), object->Width, object->Height);
			SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
			InitRectRhw(pVtx);														// rhwの設定
	
			aNumber[i] = nLengthScore % 10;
			aTex[i] = aNumber[i] * 0.1f;
			nLengthScore /= 10;

			SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);			// テクスチャ座標の設定
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

	//
	// モデル数のスコア
	//
	object = &(scoreModel);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(100.0f, 10.0f, 0.0f);		// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	object->Height = 100.0f;							// 高さ
	object->Width = 50.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// テクスチャの読込

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MODELCOUNT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&object->vtxBuff,
		NULL);
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float aTex[SCORELENGTH_MAX];
		int aNumber[SCORELENGTH_MAX];
		int nStep = 0;	// 段階を確認
		int nModelScore = (int)GetPlayer()->nModelCnt;	// スコア

		while (nModelScore / 100 != 0)
		{
			nStep++;
			nModelScore /= 100;
		}

		for (int i = 0; i < SCORELENGTH_MAX; i++)
		{
			SetRectUpRightPos(pVtx, D3DXVECTOR3(object->pos.x - 25.0f * i, object->pos.y, object->pos.z), object->Width, object->Height);
			SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
			InitRectRhw(pVtx);														// rhwの設定

			aNumber[i] = nModelScore % 10;
			aTex[i] = aNumber[i] * 0.1f;
			nModelScore /= 10;

			SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);			// テクスチャ座標の設定
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();
}

//=========================================
// 終了
//=========================================
void UninitResultUI(void)
{
	UninitObject(&nextButton);
	UninitObject(&scoreLength);
	UninitObject(&scoreModel);
}

//=========================================
// 更新
//=========================================
void UpdateResultUI(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 長さの結果UIの更新
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	scoreLength.vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点バッファをアンロックする
	scoreLength.vtxBuff->Unlock();

}

//=========================================
// 描画
//=========================================
void DrawResultUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nextButton.bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, nextButton.vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, nextButton.tex, 0);
	}

	if (scoreLength.bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, scoreLength.vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < MODELCOUNT_MAX; i++)
		{
			// ポリゴン描画
			// テクスチャの設定
			RectDraw(pDevice, scoreLength.tex, i * 4);
		}
	}

	if (scoreModel.bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, scoreModel.vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SCORELENGTH_MAX; i++)
		{
			// ポリゴン描画
			// テクスチャの設定
			RectDraw(pDevice, scoreModel.tex, i * 4);
		}
	}
}

////=========================================
//// オブジェクトの初期化
////=========================================
//static void InitObject(Object * object)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
//
//	D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// テクスチャの読込
//
//	// 頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&object->vtxBuff,
//		NULL);
//
//	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
//	
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//	{
//		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
//		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する
//
//		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
//		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
//		InitRectRhw(pVtx);														// rhwの設定
//		InitRectTex(pVtx);														// テクスチャ座標の設定
//
//	}
//	// 頂点バッファをアンロックする
//	object->vtxBuff->Unlock();
//
//	// 使用に切り替え
//	object->bUse = true;
//}
//
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
//
////=========================================
//// オブジェクトの更新
////=========================================
//void UpdateObject(Object * object)
//{
//
//}
//
////=========================================
//// オブジェクトの描画
////=========================================
//void DrawObject(Object * object)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// 頂点バッファをデータストリーム設定
//	pDevice->SetStreamSource(0, object->vtxBuff, 0, sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	if (object->bUse)
//	{
//		// ポリゴン描画
//		// テクスチャの設定
//		RectDraw(pDevice, object->tex, 0);
//	}
//}
