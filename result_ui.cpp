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
#include <stdio.h>
#include <time.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define SELECTBG			"data/TEXTURE/number000.png"
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
static Object scoreLengthWord;
static LPD3DXFONT s_pFont = NULL;		// フォントへのポインタ
static char s_aLength[5];
static char s_aModelCnt[5];
static char s_aModelName[50];
static float s_fLength;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void UninitObject(Object *object);	// 終了

//=========================================
// 初期化
//=========================================
void InitResultUI(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	Object* object;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	srand((unsigned int)time(NULL));

	//
	// モデルの個数
	//
	object = &(nextButton);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(270.0f, 250.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	object->Height = 75.0f;								// 高さ
	object->Width = 475.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/ResultModel.png", &object->tex);	// テクスチャの読込

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

	//
	// モデルの長さのスコア文字
	//
	object = &(scoreLengthWord);
	ZeroMemory(object, sizeof(object));
	object->pos = D3DXVECTOR3(1000.0f, 250.0f, 0.0f);	// 位置
	object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	object->Height = 75.0f;								// 高さ
	object->Width = 475.0f;								// 幅
	object->bUse = true;								// 使用に切り替え

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/ResultLength.png", &object->tex);	// テクスチャの読込

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

	Player* player = GetPlayer();

	// モデルの大きさを表示
	s_fLength = player->fLength;

	if (s_fLength < 0.001f)
	{
		s_fLength *= 10000.0f;
	}
	else if (s_fLength > 0.001f && s_fLength < 1.0f)
	{
		s_fLength *= 100.0f;
	}
	else if (s_fLength > 1.0f && s_fLength < 1000.0f)
	{
	}
	else if (s_fLength >= 1000.0f)
	{
		s_fLength /= 1000.0f;
	}

	if (player->fLength < 1.0f)
	{
		sprintf(s_aLength, "%.1fmm", s_fLength);
	}
	if (player->fLength > 1.0f &&player->fLength < 100.0f)
	{
		sprintf(s_aLength, "%.1fcm", s_fLength);
	}
	if (player->fLength > 100.0f && player->fLength < (100.0f * 1000.0f))
	{
		if (s_fLength >= 100.0f)
		{
			sprintf(s_aLength, "%.0fm", s_fLength);
		}
		else
		{
			sprintf(s_aLength, "%.1fm", s_fLength);
		}
	}
	if (player->fLength >= (100.0f * 1000.0f))
	{
		sprintf(s_aLength, "%.1fkm", s_fLength);
	}
	sprintf(s_aModelCnt, "%03d コ", GetPlayer()->nModelCnt);

	int nRand;

	if (GetPlayer()->nModelCnt != 0)
	{

		nRand = rand() % GetPlayer()->nModelCnt;
	}
	else
	{
		nRand = GetPlayer()->nModelCnt;
	}

	for (int nModelCnt = 0; nModelCnt < MODEL_MAX; nModelCnt++)
	{
		Model* model = GetPlayerModel()[nModelCnt];
		if ((model == NULL) || (model->nIdxModelParent != 0))
		{
			continue;
		}	

		nRand--;
		if (nRand <= 0)
		{
			sprintf(s_aModelName, "%s星", model->name);
			break;
		}
	}

	// プレイヤーの大きさ用フォントの生成
	D3DXCreateFont(GetDevice(), 100, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07あかずきんポップ Heavy", &s_pFont);
}

//=========================================
// 終了
//=========================================
void UninitResultUI(void)
{
	UninitObject(&nextButton);
	UninitObject(&scoreLengthWord);

	// デバッグ表示用フォントの破棄
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateResultUI(void)
{
}

//=========================================
// 描画
//=========================================
void DrawResultUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (scoreLengthWord.bUse)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, scoreLengthWord.vtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴン描画
		// テクスチャの設定
		RectDraw(pDevice, scoreLengthWord.tex, 0);
	}

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

	// 表示領域の作成
	RECT rect = { 160,300,SCREEN_WIDTH,SCREEN_HEIGHT };
	// テキストの描画
	s_pFont->DrawText(NULL, s_aModelCnt, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// 表示領域の作成
	rect = { 900,300,SCREEN_WIDTH,SCREEN_HEIGHT };
	// テキストの描画
	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	// 表示領域の作成
	rect = { 0,550,SCREEN_WIDTH,SCREEN_HEIGHT };
	// テキストの描画
	s_pFont->DrawText(NULL, s_aModelName, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
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