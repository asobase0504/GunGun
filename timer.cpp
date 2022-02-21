//=========================================
// 
// タイム処理
// Author KOZUNA HIROHITO
// Author Yuda Kaito
// 
//=========================================
#include "timer.h"
#include <stdio.h>

//マクロ定義
#define MAX_TIMER_DITIT		(256)		// タイマー(1桁)の最大数
#define MAX_TIMER			(64)		// タイマー(全体)の最大数
#define TIME_TEX			"data/TEXTURE/number003.png"	// タイマーのテクスチャ

//グローバル変数
static LPDIRECT3DTEXTURE9 g_pTextureTimer = { NULL };					//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = { NULL };				//頂点バッファへのポインタ
static TimerDigit g_TimerDigit[MAX_TIMER_DITIT];						//タイム構造体の変数
static Timer g_Timer[MAX_TIMER];

//プロトタイプ宣言
static void TimerDataInit(void);
static void SetTimer(int nNumber, int nSecond);

//=========================================
//タイムの初期化
//=========================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEX,
		&g_pTextureTimer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER_DITIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	//タイムの情報の初期化
	TimerDataInit();


	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	{
		TimerDigit* timer = &(g_TimerDigit[nCnt]);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);
		
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();
}

//=========================================
//タイムの終了処理
//=========================================
void UninitTimer(void)
{
	//テクスチャの破棄
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}

	for (int i = 0; i < MAX_TIMER; i++)
	{
		Timer* timer = &(g_Timer[i]);

		// デバッグ表示用フォントの破棄
		if (g_Timer[i].font != NULL)
		{
			g_Timer[i].font->Release();
			g_Timer[i].font = NULL;
		}
	}
}

//=========================================
//タイムの更新処理
//=========================================
void UpdateTimer(void)
{
	for (int i = 0; i < MAX_TIMER; i++)
	{
		Timer* timer = &(g_Timer[i]);

		if (!timer->bUse && !timer->bCount)
		{
			continue;
		}

		timer->nFps++;
		if (timer->nFps >= 55)
		{
			timer->nFps = 0;

			if (timer->nSecond >= 0)
			{
				//タイムのセット
				SetTimer(timer->nNumber, timer->nSecond);
			}

			//タイマーを進める
			timer->nSecond--;
		}
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_TIMER_DITIT; i++)
	{
		TimerDigit* timer = &(g_TimerDigit[i]);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);

		pVtx += 4;  //頂点データのポインタを４* nCntDigitつ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();
}

//=========================================
//タイムの描画処理
//=========================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (!g_Timer[nCnt].bUse)
		{
			continue;
		}

		// 表示領域の作成
		RECT rect = { (LONG)g_Timer[nCnt].pos.x,g_Timer[nCnt].pos.y,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };

		char aLength[5];

		sprintf(aLength, "%d", g_Timer[nCnt].nSecond);

		g_Timer[nCnt].font->DrawText(NULL, aLength, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	//for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	//{
	//	if (!g_TimerDigit[nCnt].bUse)
	//	{
	//		continue;
	//	}

	//	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	//	//ポリゴンの描画
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
	//		4 * nCnt,						//描画する最初の頂点インデックス
	//		2);		//プリミティブ（ポリゴン）数
	//}
}

//=========================================
// タイムの取得処理
//=========================================
Timer* GetTimer(int nNumber)
{
	return &g_Timer[nNumber];
}

//=========================================
// タイムの開始処理
//=========================================
void StartTimer(int nSecond, int nDigit, float fWidth, float fHeight, D3DXVECTOR3 pos, int nPatn)
{
	int nCntTimer;
	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{//タイマーのグループのループ

		if (g_Timer[nCntTimer].bUse)
		{
			continue;
		}

		g_Timer[nCntTimer].nSecond = nSecond;	//表示時間
		
		//タイマーの番号
		if (nCntTimer - 1 < 0)
		{
			g_Timer[nCntTimer].nNumber = 0;
		}
		else
		{
			g_Timer[nCntTimer].nNumber = g_Timer[nCntTimer - 1].nNumber + 1;
		}
		g_Timer[nCntTimer].bUse = true;//使用してるかどうか
		g_Timer[nCntTimer].bCount = false;//カウントが止まっている状態

		g_Timer[nCntTimer].pos = pos;//位置
		D3DXCreateFont(GetDevice(), fHeight, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07あかずきんポップ Heavy", &g_Timer[nCntTimer].font);
		break;
	}

	for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	{//タイマーの１桁のループ
		TimerDigit *timerDigit = &(g_TimerDigit[nCnt]);

		if (timerDigit->bUse)
		{
			continue;
		}

		timerDigit->fWidth = fWidth;	//横幅
		timerDigit->fHeight = fHeight;	//縦幅
		timerDigit->pos = D3DXVECTOR3(pos.x - g_Timer[nCntTimer].nDigit * fWidth * 2, pos.y, 0.0f);//位置
		timerDigit->nPatn = nPatn;//表示のしかた
		timerDigit->nNumber = g_Timer[nCntTimer].nNumber;//タイマーの番号
		timerDigit->bUse = true;//使用してるかどうか
		g_Timer[nCntTimer].nDigit++;//必要な桁数カウント

		if (g_Timer[nCntTimer].nDigit == nDigit + 1)
		{
			break;
		}
	}

	//タイムセット処理
	SetTimer(g_Timer[nCntTimer].nNumber, nSecond);
}

//=========================================
//タイムのカウントが指定の値になったかどうか
//=========================================
bool TimerUp(int nNumber)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (g_Timer[nCnt].bUse && g_Timer[nCnt].nNumber == nNumber && g_Timer[nCnt].nSecond <= 0)
		{
				return true;
		}
	}
	return false;
}

//=========================================
//タイムの情報の初期化
//=========================================
static void TimerDataInit(void)
{
	ZeroMemory(g_Timer,sizeof(g_Timer));

	ZeroMemory(g_TimerDigit, sizeof(g_TimerDigit));
}

//=========================================
//タイマーを消す
//=========================================
void BreakTimer(int nNumber)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (g_Timer[nCnt].bUse && g_Timer[nCnt].nNumber == nNumber)
		{
			g_Timer[nCnt].nSecond = 0;//表示時間
			g_Timer[nCnt].nFps = 0;//現在のフレーム
			g_Timer[nCnt].nNumber = 0;//タイマーの番号
			g_Timer[nCnt].nDigit = 0;//桁数数
			g_Timer[nCnt].bUse = false;//使用してるかどうか
			g_Timer[nCnt].bCount= false;//カウントがストップしている
		}
	}

	for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	{
		if (g_TimerDigit[nCnt].bUse && g_TimerDigit[nCnt].nNumber == nNumber)
		{
			g_TimerDigit[nCnt].fWidth = 0.0f;//横幅
			g_TimerDigit[nCnt].fHeight = 0.0f;//縦幅
			g_TimerDigit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
			g_TimerDigit[nCnt].nPatn = 0;//表示のしかた
			g_TimerDigit[nCnt].nNumber = 0;//タイマーの番号
			g_TimerDigit[nCnt].bUse = false;//使用してるかどうか
		}
	}

}

//=========================================
//タイムのセット
//=========================================
static void SetTimer(int nNumber, int nSecond)
{
	int aPosTexU = 0;

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		Timer* timer = &(g_Timer[nCntTimer]);

		if (!timer->bUse || timer->nNumber != nNumber)
		{
			continue;
		}

		for (int nCntDigit = 0; nCntDigit < MAX_TIMER_DITIT; nCntDigit++)
		{
			TimerDigit *timerDigit = &(g_TimerDigit[nCntDigit]);

			if (!timerDigit->bUse || timerDigit->nNumber != nNumber)
			{
				continue;
			}

			VERTEX_2D *pVtx;		//頂点情報へのポインタ
			
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntDigit;  //頂点データのポインタを４* nCntDigitつ分進める

			for (int nCntTexU = 0, nCnt = 1; nCntTexU < timer->nDigit; nCntTexU++, nCnt *= 10)
			{
				aPosTexU = nSecond % (nCnt * 10) / nCnt;	//各位の数字を格納

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU + 1), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU + 1), 1.0f);

				pVtx += 4;  //頂点データのポインタを４つ分進める

			}
			//頂点バッファをアンロック
			g_pVtxBuffTimer->Unlock();

			return;
		}
	}
}

//=========================================
//カウントのリスタートとストップ処理
//=========================================
void CountRestartStop(bool bCount, int nNumber)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (!g_Timer[nCnt].bUse && g_Timer[nCnt].nNumber != nNumber)
		{
			continue;
		}

		g_Timer[nCnt].bCount = bCount;//カウントをリスタートかストップさせる
		break;
	}
}