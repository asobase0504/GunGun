//============================================================================================================
//
// ポーズ処理
// Author : Kishimoto Eiji
// Author : Yuda Kaito
//
//============================================================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
#include "common.h"
#include "fade.h"
#include "sound.h"

//============================================================================================================
//マクロ定義
//============================================================================================================
#define NUM_PAUSE				(6)			//ポーズの種類
#define PAUSEMENU_HALF_WIDTH	(200)		//ポーズメニューの幅(半分)
#define PAUSEMENU_HALF_HEIGHT	(50)		//ポーズメニューの高さ(半分)
#define PAUSEMENU_TITLE_WIDTH	(300)		//ポーズメニューの幅(半分)
#define PAUSEMENU_TITLE_HEIGHT	(150)		//ポーズメニューの高さ(半分)
#define PAUSEMENU_SPACE			(200)		//ポーズメニューの間隔
#define PAUSE_HALF_WIDTH		(350)		//ポーズの幅(半分)
#define PAUSE_HALF_HEIGHT		(400)		//ポーズの高さ(半分)

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9	   s_apTexture[NUM_PAUSE] = { NULL, NULL, NULL, NULL };		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPause		  = NULL;							//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseTitle = NULL;								//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseCursol = NULL;							//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseMenu	  = NULL;							//頂点バッファへのポインタ
static PAUSE_MENU			   s_pauseMenu;												//ポーズメニュー
static PauseMenu			   s_aPauseMenu[PAUSE_MENU_MAX];							//ポーズメニューの情報
static D3DXVECTOR3			   s_posPause;												//ポーズの位置
static D3DXVECTOR3			   s_posPauseCusol;											//ポーズの位置
static D3DXVECTOR3			   s_rotPauseCusol;											//ポーズの位置

//============================================================================================================
//ポーズの初期化処理
//============================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ
	int nCntPause;								//カウント
	s_pauseMenu = PAUSE_MENU_CONTINUE;			//ポーズメニュー

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/WORD/back.png",
		&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/WORD/Restart.png",
		&s_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/WORD/back_Title.png",
		&s_apTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause100.png",
		&s_apTexture[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause_Menu.png",
		&s_apTexture[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cursor.png",
		&s_apTexture[5]);


	s_posPause = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//ポーズの位置を初期化
	s_rotPauseCusol = ZERO_VECTOR;

	//情報の初期化
	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		s_aPauseMenu[nCntPause].pos = D3DXVECTOR3(s_posPause.x, s_posPause.y + ((nCntPause - 1) * PAUSEMENU_SPACE), 0.0f);
		s_aPauseMenu[nCntPause].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}

	s_pauseMenu = PAUSE_MENU_CONTINUE;					//ポーズメニューを初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPause,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPauseTitle,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPauseMenu,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPauseCursol,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//ポーズの半透明ポリゴン
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		else if (nCntPause == 1)
		{//ポーズ
			//頂点座標の設定
			pVtx[0].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[0].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[1].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[2].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[3].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[3].pos.z = 0.0f;
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntPause == 0)
		{//ポーズの半透明ポリゴン
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		}
		else if (nCntPause == 1)
		{//ポーズ
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
		}

		//テクスチャ座標の設定
		InitRectTex(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPause->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//ポーズ
		 //頂点座標の設定
		pVtx[0].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[0].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[1].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[2].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[3].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
		pVtx[3].pos.z = 0.0f;

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

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffPauseMenu->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPauseTitle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = s_posPause.x - PAUSEMENU_TITLE_WIDTH;
	pVtx[0].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f - PAUSEMENU_TITLE_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_posPause.x + PAUSEMENU_TITLE_WIDTH;
	pVtx[1].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f - PAUSEMENU_TITLE_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_posPause.x - PAUSEMENU_TITLE_WIDTH;
	pVtx[2].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f + PAUSEMENU_TITLE_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_posPause.x + PAUSEMENU_TITLE_WIDTH;
	pVtx[3].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f + PAUSEMENU_TITLE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

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
	s_pVtxBuffPauseTitle->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPauseCursol->Lock(0, 0, (void**)&pVtx, 0);

	s_posPauseCusol.x = s_aPauseMenu[s_pauseMenu].pos.x - 250.0f;
	s_posPauseCusol.y = s_aPauseMenu[s_pauseMenu].pos.y;
	s_posPauseCusol.z = s_aPauseMenu[s_pauseMenu].pos.z;

	// 頂点座標の設定
	SetRectUpLeftPos(pVtx, s_posPauseCusol, 90.0f, 90.0f);

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
	s_pVtxBuffPauseCursol->Unlock();

}

//============================================================================================================
//ポーズの終了処理
//============================================================================================================
void UninitPause(void)
{
	int nCntPause;		//カウント

	//テクスチャの破棄
	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (s_apTexture[nCntPause] != NULL)
		{
			s_apTexture[nCntPause]->Release();
			s_apTexture[nCntPause] = NULL;
		}
	}

	//頂点バッファの破棄
	if (s_pVtxBuffPause != NULL)
	{
		s_pVtxBuffPause->Release();
		s_pVtxBuffPause = NULL;
	}

	if (s_pVtxBuffPauseMenu != NULL)
	{
		s_pVtxBuffPauseMenu->Release();
		s_pVtxBuffPauseMenu = NULL;
	}

	if (s_pVtxBuffPauseTitle != NULL)
	{
		s_pVtxBuffPauseTitle->Release();
		s_pVtxBuffPauseTitle = NULL;
	}
}

//============================================================================================================
//ポーズの更新処理
//============================================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	if (GetFade() == FADE_NONE)
	{//フェードしていないとき

		if (GetJoypadTrigger(JOYKEY_UP) || GetKeyboardTrigger(DIK_W))
		{//Wキーが押された

			//1つ上を選ぶ
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu - 1) + 3) % 3);
			//白にする
			s_posPauseCusol.x = s_aPauseMenu[s_pauseMenu].pos.x - 250.0f;
			s_posPauseCusol.y = s_aPauseMenu[s_pauseMenu].pos.y;
			s_posPauseCusol.z = s_aPauseMenu[s_pauseMenu].pos.z;
			PlaySound(SOUND_LABEL_SE_SERECT_POUSE);
		}

		if (GetJoypadTrigger(JOYKEY_DOWN) || GetKeyboardTrigger(DIK_S))
		{//Sキーが押された
			 //1つ下を選ぶ
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu + 1) + 3) % 3);

			//白にする
			s_posPauseCusol.x = s_aPauseMenu[s_pauseMenu].pos.x - 250.0f;
			s_posPauseCusol.y = s_aPauseMenu[s_pauseMenu].pos.y;
			s_posPauseCusol.z = s_aPauseMenu[s_pauseMenu].pos.z;
			PlaySound(SOUND_LABEL_SE_SERECT_POUSE);
		}
		s_rotPauseCusol.z += 0.01f;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffPauseCursol->Lock(0, 0, (void**)&pVtx, 0);

		float fLength = sqrtf(90.0f  * 90.0f + 90.0f * 90.0f) / 2.0f;	// 中心座標から上の長さを算出する。
		float fAngle = atan2f(90.0f, 90.0f);							// 中心座標から上の頂点の角度を算出する

		// 頂点座標の設定
		SetRectCenterRotPos(pVtx, s_posPauseCusol, s_rotPauseCusol, fAngle, fLength);

		//頂点バッファをアンロックする
		s_pVtxBuffPauseCursol->Unlock();

		if (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN))
		{//Enterキーが押された
			PlaySound(SOUND_LABEL_SE_SERECT);
			switch (s_pauseMenu)
			{
			case PAUSE_MENU_CONTINUE:	//ゲームに戻る

				//ポーズ解除
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:		//ゲームをやり直す

				//ゲーム画面に移行
				ResetFade(MODE_GAME);
				break;

			case PAUSE_MENU_QUIT:		//タイトル画面に戻る

				//タイトル画面に移行
				SetFade(MODE_TITLE);
				break;
			}
		}
	}
}

//============================================================================================================
//ポーズの描画処理
//============================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	int nCntPause;								//カウント

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//ポーズの半透明ポリゴン
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		else if (nCntPause == 1)
		{//ポーズ
			//テクスチャの設定
			pDevice->SetTexture(0, s_apTexture[3]);
		}

		//ポーズの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntPause * 4,			//描画する最初の頂点インデックス
			2);						//描画するプリミティブ数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPauseMenu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTexture[nCntPause]);

		//ポーズの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntPause * 4,								//描画する最初の頂点インデックス
			2);											//描画するプリミティブ数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPauseTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_apTexture[4]);

	//ポーズの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,			//描画する最初の頂点インデックス
		2);						//描画するプリミティブ数

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffPauseCursol, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_apTexture[5]);

	//ポーズの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,			//描画する最初の頂点インデックス
		2);						//描画するプリミティブ数

}