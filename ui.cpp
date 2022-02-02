//=========================================
// 
// タイトルのUI処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "ui.h"
#include "common.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SELECTBG			"data/TEXTURE/number002.png"
#define MODELCOUNT_MAX		(3)	// モデル数のスコアの最大桁数
#define SCORELENGTH_MAX		(3)	// 長さスコアの最大桁数
#define MAX_UI				(255)

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
	int digit;							// 桁数
	float Height;						// 高さ
	float Width;						// 幅
	bool bUse;							// 使用してるかどうか
}Object;

//------------------------------------
// 静的変数宣言
//------------------------------------
static Object objectUI[MAX_UI];

//------------------------------------
// プロトタイプ宣言
//------------------------------------

//=========================================
// 初期化
//=========================================
void InitUI(void)
{
	ZeroMemory(objectUI, sizeof(objectUI));
}

//=========================================
// 終了
//=========================================
void UninitUI(void)
{
	for (int i = 0; i < MAX_UI; i++)
	{
		Object* pObject = &objectUI[i];
		
		// テクスチャの破棄
		if (pObject->tex != NULL)
		{
			pObject->tex->Release();
			pObject->tex = NULL;
		}

		// 頂点バッファの破棄
		if (pObject->vtxBuff != NULL)
		{
			pObject->vtxBuff->Release();
			pObject->vtxBuff = NULL;
		}
	}
}

//=========================================
// 更新
//=========================================
void UpdateUI(void)
{
}

//=========================================
// 描画
//=========================================
void DrawUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_UI; i++)
	{
		Object* pObject = &objectUI[i];

		if (!pObject->bUse)
		{
			continue;
		}

		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, pObject->vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, pObject->tex, 0);
	}
}

//=========================================
// 設定
//=========================================
void SetUI(char * pFile, D3DXVECTOR3 * pos, D3DXCOLOR * col, D3DXVECTOR3 * size)
{
	for (int i = 0; i < MAX_UI; i++)
	{
		Object* pObject = &objectUI[i];

		if (pObject->bUse)
		{
			continue;
		}
		ZeroMemory(pObject, sizeof(pObject));

		pObject->pos = *pos;		// 位置
		pObject->col = *col;		// 色
		pObject->Height = size->y;	// 高さ
		pObject->Width = size->x;	// 幅
		pObject->bUse = true;		// 使用に切り替え

		LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

		D3DXCreateTextureFromFile(pDevice, pFile, &pObject->tex);	// テクスチャの読込

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pObject->vtxBuff,
			NULL);

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pObject->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fLength = sqrtf(pObject->Width  * pObject->Width + pObject->Height * pObject->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(pObject->Width, pObject->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, pObject->pos, pObject->rot, fAngle, fLength);		// 頂点座標の設定
		SetRectColor(pVtx, &(pObject->col));										// 頂点カラーの設定
		InitRectRhw(pVtx);															// rhwの設定
		InitRectTex(pVtx);															// テクスチャ座標の設定
		
		// 頂点バッファをアンロックする
		pObject->vtxBuff->Unlock();

		break;
	}
}


//=========================================
// タイトルUIの読み込み処理
//=========================================
void LoadTitleUI(void)
{
	SetUI("/data/TEXTURE/07.彼方への君に捧ぐ.png", &D3DXVECTOR3(10.0f, 10.0f, 0.0f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), &D3DXVECTOR3(100.0f, 100.0f, 0.0f));
}

//=========================================
// ゲームUIの読み込み処理
//=========================================
void LoadGameUI(void)
{
}

//=========================================
// リザルトUIの読み込み処理
//=========================================
void LoadResultUI(void)
{
	// 次へのボタン
	SetUI(SELECTBG, &D3DXVECTOR3(10.0f, 10.0f, 0.0f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), &D3DXVECTOR3(10.0f, 10.0f, 0.0f));

	////
	//// 長さのスコア
	////
	//object = &(scoreLength);
	//ZeroMemory(object, sizeof(object));
	//object->pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 300.0f, 0.0f);	// 位置
	//object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	//object->Height = 60.0f;									// 高さ
	//object->Width = 40.0f;									// 幅
	//object->bUse = true;									// 使用に切り替え

	//D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// テクスチャの読込

	//															// 頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORELENGTH_MAX,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&object->vtxBuff,
	//	NULL);

	//// 頂点バッファをロックし、頂点情報へのポインタを取得
	//object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//{
	//	float aTex[SCORELENGTH_MAX];
	//	int aNumber[SCORELENGTH_MAX];
	//	int nStep = 0;	// 段階を確認
	//	int nLengthScore = (int)GetPlayer()->fLength;	// スコア

	//	while (nLengthScore / 100 != 0)
	//	{
	//		nStep++;
	//		nLengthScore /= 100;
	//	}

	//	for (int i = 0; i < SCORELENGTH_MAX; i++)
	//	{
	//		SetRectUpRightPos(pVtx, D3DXVECTOR3(object->pos.x - 25.0f * i, object->pos.y, object->pos.z), object->Width, object->Height);
	//		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
	//		InitRectRhw(pVtx);														// rhwの設定

	//		aNumber[i] = nLengthScore % 10;
	//		aTex[i] = aNumber[i] * 0.1f;
	//		nLengthScore /= 10;

	//		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);			// テクスチャ座標の設定
	//		pVtx += 4;
	//	}
	//}
	//// 頂点バッファをアンロックする
	//object->vtxBuff->Unlock();

	////
	//// モデル数のスコア
	////
	//object = &(scoreModel);
	//ZeroMemory(object, sizeof(object));
	//object->pos = D3DXVECTOR3(100.0f, 10.0f, 0.0f);		// 位置
	//object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	//object->Height = 100.0f;							// 高さ
	//object->Width = 50.0f;								// 幅
	//object->bUse = true;								// 使用に切り替え

	//D3DXCreateTextureFromFile(pDevice, SELECTBG, &object->tex);	// テクスチャの読込

	//															// 頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MODELCOUNT_MAX,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&object->vtxBuff,
	//	NULL);

	//// 頂点バッファをロックし、頂点情報へのポインタを取得
	//object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//{
	//	float aTex[SCORELENGTH_MAX];
	//	int aNumber[SCORELENGTH_MAX];
	//	int nStep = 0;	// 段階を確認
	//	int nModelScore = (int)GetPlayer()->nModelCnt;	// スコア

	//	while (nModelScore / 100 != 0)
	//	{
	//		nStep++;
	//		nModelScore /= 100;
	//	}

	//	for (int i = 0; i < SCORELENGTH_MAX; i++)
	//	{
	//		SetRectUpRightPos(pVtx, D3DXVECTOR3(object->pos.x - 25.0f * i, object->pos.y, object->pos.z), object->Width, object->Height);
	//		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
	//		InitRectRhw(pVtx);														// rhwの設定

	//		aNumber[i] = nModelScore % 10;
	//		aTex[i] = aNumber[i] * 0.1f;
	//		nModelScore /= 10;

	//		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);			// テクスチャ座標の設定
	//		pVtx += 4;
	//	}
	//}
	//// 頂点バッファをアンロックする
	//object->vtxBuff->Unlock();

}