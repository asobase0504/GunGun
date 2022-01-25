//============================================================================================================
//
// リザルト処理
// Author:Kishimoto Eiji
// Author:Yuda Kaito
//
//============================================================================================================
//------------------------------------
// include
//------------------------------------
#include "result.h"
#include "input.h"
#include "common.h"
#include "fade.h"
//#include "sound.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define FILENAME	("data/TEXTURE/Result_Hunter.png")

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9 s_pTexture[GAME_MAX] = { NULL,NULL };		//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9 s_pTextureWinner = NULL;					//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;					//頂点バッファへのポインタ
static bool bWin;

//============================================================================================================
//リザルトの初期化処理
//============================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		FILENAME,
		&s_pTexture[GAME_HUNTER]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Result_Survivor.png",
		&s_pTexture[GAME_SURVIVOR]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Winner.png",
		&s_pTextureWinner);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	InitRectTex(pVtx);

	pVtx += 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	InitRectTex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================================================================================================
//リザルトの終了処理
//============================================================================================================
void UninitResult(void)
{
	for (int i = 0; i < GAME_MAX; i++)
	{
		//テクスチャの破棄
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================================================================================================
//リザルトの更新処理
//============================================================================================================
void UpdateResult(void)
{
	if (GetFade() == FADE_NONE)
	{//フェードしていないとき
		if (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN))
		{//Enterキーが押されたら
			// タイトル画面に移行
			SetFade(MODE_TITLE);
		}
	}
}

//============================================================================================================
//リザルトの描画処理
//============================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bWin)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[GAME_HUNTER]);
	}
	else
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[GAME_SURVIVOR]);
	}

	//リザルトの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}
