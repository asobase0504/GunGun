//====================================
// 
// 画面遷移の処理
// Author Yuda Kaito
// 
//====================================

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "fade.h"
#include "common.h"
#include "camera.h"

//------------------------------------
// グローバル変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static FADE s_fade;									// フェードの状態
static MODE s_modeNext;								// 次の画面(モード)
static D3DXCOLOR s_colorFade;						// ポリゴン(フェード)の色

//====================================
// 画面遷移の初期化処理
//====================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	s_fade = FADE_IN;		// フェードイン状態に
	s_modeNext = modeNext;	// 次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒いポリゴン(不透明)にしておく

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetRectUpLeftPos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

	// rhwの設定
	InitRectRhw(pVtx);

	// 頂点カラーの設定
	SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

	// テクスチャ座標の設定
	InitRectTex(pVtx);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// モードの設定
	SetMode(s_modeNext);
}

//====================================
// 画面遷移の終了処理
//====================================
void UninitFade(void)
{
	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//====================================
// 画面遷移の更新処理
//====================================
void UpdateFade(void)
{
	if (s_fade != FADE_NONE)
	{
		if (s_fade == FADE_IN)
		{// フェードイン状態
			s_colorFade.a -= 0.01f;	// ポリゴンを透明にしていく

			if (s_colorFade.a <= 0.0f)
			{
				s_colorFade.a = 0.0f;
				s_fade = FADE_NONE;	// 何もしていない状態に
			}

		}
		else if (s_fade == FADE_OUT)
		{// フェードアウト状態
			s_colorFade.a += 0.01f;

			if (s_colorFade.a >= 1.0f)
			{
				s_colorFade.a = 1.0f;
				s_fade = FADE_IN;	// フェードイン常置に

				// モード設定(次の画面に移行)
				SetMode(s_modeNext);
			}

		}

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(s_colorFade));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//====================================
// 画面遷移の描画処理
//====================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	// 描画処理
	InitDraw(pDevice, s_pVtxBuff);

	// テクスチャの描画
	RectDraw(pDevice, NULL, 0);
}

//====================================
// 画面遷移の設定処理
//====================================
void SetFade(MODE modeNext)
{
	if (s_modeNext != modeNext)
	{
		s_fade = FADE_OUT;		// フェードアウト状態に
		s_modeNext = modeNext;	// 次の画面(モード)を設定
		s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒いポリゴン(透明)にしておく
	}
}

//====================================
// 画面遷移の強制設定処理
//====================================
void ResetFade(MODE modeNext)
{
	s_fade = FADE_OUT;		// フェードアウト状態に
	s_modeNext = modeNext;	// 次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒いポリゴン(透明)にしておく
}

//====================================
// 画面遷移の取得処理
//====================================
FADE GetFade(void)
{
	return s_fade;
}