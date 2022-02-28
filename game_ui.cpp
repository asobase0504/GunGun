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
#include "input.h"
#include "timer.h"
#include "camera.h"
#include <stdio.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define SCORE_BG_TEX		"data/TEXTURE/Circle_01.png"

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
static Object uiTimeBg;
static Object uiTimeGauge;
static Object uiLengthUnitBg;
static Object uiLv;
static Object uiGetModelUnitBg[2] = {};
static LPD3DXFONT s_pFont = NULL;		// フォントへのポインタ
static char s_aLength[5];
static char s_aLv[5];
static float s_fLength;

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
	s_fLength = 0;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

	// 長さのスコアの長さ単位を出す。
	{
		object = &(uiLengthUnitBg);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH - 65.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	// 位置
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		object->Height = 168.0f * 0.25f;					// 高さ
		object->Width = 417.0f * 0.25f;						// 幅
		object->bUse = true;								// 使用に切り替え

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WORD/mm.png", &object->tex);	// テクスチャの読込

		SetVtxBuff2D(&object->vtxBuff, 4);	// 頂点バッファの生成

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

	// 長さのスコアを置く場所の背景
	{
		object = &(uiLengthScoreBg);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH - 50.0f , SCREEN_HEIGHT + 25.0f, 0.0f);	// 位置
		object->col = D3DXCOLOR(0.0f, 0.9f, 0.0f, 1.0f);	// 色
		object->Height = 600.0f;							// 高さ
		object->Width = 600.0f;								// 幅
		object->bUse = true;								// 使用に切り替え
		
		D3DXCreateTextureFromFile(pDevice, SCORE_BG_TEX, &object->tex);	// テクスチャの読込

		SetVtxBuff2D(&object->vtxBuff, 4);	// 頂点バッファの生成

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

	// タイマー背景
	{
		object = &(uiTimeBg);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f);	// 位置
		object->rot = ZERO_VECTOR;	// 位置
		object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		object->Height = 270.0f;							// 高さ
		object->Width = 270.0f;								// 幅
		object->bUse = true;								// 使用に切り替え

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor.png", &object->tex);	// テクスチャの読込

		SetVtxBuff2D(&object->vtxBuff, 4);	// 頂点バッファの生成

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

	// タイマーのゲージ
	{
		object = &(uiTimeGauge);
		ZeroMemory(object, sizeof(object));
		object->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f);	// 位置
		object->rot = ZERO_VECTOR;	// 位置
		object->rot.z = D3DX_PI * 0.5f;	// 位置
		object->col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);	// 色
		object->Height = 200.0f;							// 高さ
		object->Width = 200.0f;								// 幅
		object->bUse = true;								// 使用に切り替え

		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor_02.png", &object->tex);	// テクスチャの読込

		SetVtxBuff2D(&object->vtxBuff, 4);	// 頂点バッファの生成

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

	// プレイヤーの大きさ用フォントの生成
	D3DXCreateFont(GetDevice(), 130, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07あかずきんポップ Heavy", &s_pFont);
}

//=========================================
// 終了
//=========================================
void UninitGameUI(void)
{
	UninitObject(&uiLengthScoreBg);
	UninitObject(&uiLengthUnitBg);
	UninitObject(&uiTimeBg);
	UninitObject(&uiTimeGauge);

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
void UpdateGameUI(void)
{
	Player* player = GetPlayer();
	if (player->fLength < 0.001f)
	{
		SetGameUITex("data/TEXTURE/WORD/mm.png");
	}
	if (player->fLength < 0.001f &&player->fLength < 1.0f)
	{
		SetGameUITex("data/TEXTURE/WORD/cm.png");
	}
	if (player->fLength > 1.0 && player->fLength < 1000.0f)
	{
		SetGameUITex("data/TEXTURE/WORD/m.png");
	}
	if (player->fLength >= 1000.0f)
	{
		SetGameUITex("data/TEXTURE/WORD/km.png");
	}

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

	if (s_fLength >= 100.0f)
	{
		sprintf(s_aLength, "%.0f", s_fLength);
	}
	else
	{
		sprintf(s_aLength, "%.1f", s_fLength);
	}

	sprintf(s_aLv, "Lv.%d", (int)((GetPlayer()->fLength / 14.0f) + 1));

	Object* object;
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	int nSecond = GetTimer(1)->nSecond;

	object = &(uiTimeGauge);
	object->rot.z = -RAD(nSecond * 2.0f);	// 位置

							
	if (nSecond <= 25)
	{
		if (nSecond % 2 == 0)
		{
			object->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 色
		}
		else
		{
			object->col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);	// 色
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	object->vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(object->Width, object->Height);	// 中心座標から上の頂点の角度を算出する

		SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);	// 頂点座標の設定
		SetRectColor(pVtx, &(object->col));										// 頂点カラーの設定
	}
	// 頂点バッファをアンロックする
	object->vtxBuff->Unlock();

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

	object = &(uiTimeBg);
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

	object = &(uiTimeGauge);
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

	// 表示領域の作成
	RECT rect = { (LONG)950.0f,(LONG)(SCREEN_HEIGHT - 185.0f),(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	// テキストの描画
	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));

	// LV表示
	// 表示領域の作成
	rect = { (LONG)150.0f,(LONG)(SCREEN_HEIGHT - 185.0f),(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	// テキストの描画
	s_pFont->DrawText(NULL, s_aLv, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
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
