//=========================================
// 
// winプロジェクトの作成
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "player.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "mesh_field.h"
#include "mesh_cylinder.h"
#include "mesh_sphere.h"
#include "mesh_sky.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include <stdio.h>

//-----------------------------------------
// マクロ定義
//-----------------------------------------
#define CLASS_NAME	"windowclass"			// ウインドウクラスの名前
#define WINDOW_NAME	"3Dアクション"		// ウインドウクラスの名前（キャプションに表示）
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HRESULT Init(HINSTANCE hInstance, HWND hWmd, BOOL bWindow);
static void Uninit(void);
static void Update(void);
static void Draw(void);
static void DrawFPS(void);
static MODE s_mode = MODE_TITLE;

//-----------------------------------------
// グローバル変数
//-----------------------------------------
LPDIRECT3D9	g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;	// フォントへのポインタ
int g_nCountFPS = 0;		// FPSカウンタ

//=========================================
// メイン関数
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 0にする(通常は使用しない)
		0,									// 0にする(通常は使用しない)
		hInstance,							// インスタンスバンドル
		LoadIcon(NULL,IDI_APPLICATION),		// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// インスタンス
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		// ファイルのアイコン
	};

	HWND hWnd;		// ウィンドウハンドル(識別子)
	MSG msg;		// メッセージと格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの生成
	hWnd = CreateWindowEx(0,		// 拡張ウインドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウのスタイル
		CW_USEDEFAULT,				// ウィンドウの左上X座標
		CW_USEDEFAULT,				// 　　""　　左上Y座標
		(rect.right - rect.left),	// 　　""　　幅
		(rect.bottom - rect.top),	// 　　""　　高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウID
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ作成データ

	if (FAILED(Init(hInstance, hWnd, true)))	// true = window,false = fullscreen
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化処理
	dwExecLastTime = timeGetTime();		// 現在時刻を取得(保存)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);			// クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);	// 仮想メッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送信
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();	// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
			 // FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPSを測定した時刻を保存
				dwFrameCount = 0;				// フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻[現在時刻]を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;	// フレームカウントを加算

			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================
// ウィンドウプロシージャ
//=========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (GetExit())
	{
		// ウィンドウを破棄する
		DestroyWindow(hWnd);
	}

	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]キーが押された
			// ウィンドウを破棄する(WM_DESTOROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:		// 閉じるボタン押下のメッセージ
		// ウィンドウを破棄する
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 規定の処理を返す
}

//=========================================
// 初期化
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ
									// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));		// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行なう)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行なう)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}

		}
	}

	// レンダーステート設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// アルファブレンドの設定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの大きさを変えても綺麗に貼る
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの大きさを変えても綺麗に貼る
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャを繰り返して貼る
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャを繰り返して貼る

	// テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ポリゴンとテクスチャのアルファ値を混ぜる。
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// ポリゴンとテクスチャのアルファ値を混ぜる。テクスチャ指定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ポリゴンとテクスチャのアルファ値を混ぜる。ポリゴン指定

	// デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 32, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	// 入力処理の初期化
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// モードの設定
	InitFade(s_mode);

	return S_OK;
}

//=========================================
// 終了
//=========================================
void Uninit(void)
{
	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力処理の終了
	UninitInput();

	// ゲームの終了処理
	UninitGame();

	// タイトル画面の終了処理
	UninitTitle();
}

//=========================================
// 更新
//=========================================
void Update(void)
{	
	// 入力処理の更新
	UpdateInput();

	// 現在の画面(モード)の更新処理
	switch (s_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UpdateTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		UpdateGame();
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
		//UpdateTutorial();
		break;

	case MODE_RANKING:	// ランキング画面
		//UpdateRanking();
		break;
	}

	UpdateFade();

	if (GetKeyboardTrigger(DIK_F1))
	{
		// ワイヤーフレームモードの設定
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		// ワイヤーフレームモードの設定をもとに戻す
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//=========================================
// 描画
//=========================================
void Draw(void)
{
	// 画面クリア(バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 	描画開始が成功した場合

		switch (s_mode)
		{
		case MODE_TITLE:	// タイトル画面
			DrawTitle();
			break;

		case MODE_GAME:		// ゲーム画面
			DrawGame();
			break;

		case MODE_TUTORIAL:	// チュートリアル画面
			//DrawTutorial();
			break;

		case MODE_RANKING:	// ランキング画面
			//DrawRanking();
			break;
		}

		// フェード処理
		DrawFade();

#ifdef _DEBUG
		// FPSの表示
		DrawFPS();
#endif // _DEBUG

		// 描画終了
		g_pD3DDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=========================================
// デバイスの取得
//=========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=========================================
// FPSの表示
//=========================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[12][256];	// 表示文字
	D3DXVECTOR3 camerarot = GetRotCamera();
	Player* player = GetPlayer();
	Model* model = GetModel();
	Camera* camera = GetCamera();
	Shadow* shadow = GetShadow();
	MODE mode = GetMode();
	// 文字列に代入
	wsprintf(&aStr[0][0], "FPS: %d\n", g_nCountFPS);

	// 文字列に代入
	sprintf(&aStr[1][0], "rot: %f\n", player->rot.y);
	// 文字列に代入
	sprintf(&aStr[2][0], "Player.pos     : %.3f|%.3f|%.3f\n", player->pos.x, player->pos.y, player->pos.z);
	// 文字列に代入
	sprintf(&aStr[3][0], "Player.fLength : %.3f\n", player->fLength);
	// 文字列に代入
	sprintf(&aStr[4][0], "Player.MinVtx  : %.3f|%.3f|%.3f\n", player->MinVtx.x, player->MinVtx.y, player->MinVtx.z);
	// 文字列に代入
	sprintf(&aStr[5][0], "Player.MaxVtx  : %.3f|%.3f|%.3f\n", player->MaxVtx.x, player->MaxVtx.y, player->MaxVtx.z);
	// 文字列に代入
	sprintf(&aStr[6][0], "quaternion     : %.3f|%.3f|%.3f|%.3f\n", player->aModel[0].quaternion.x, player->aModel[0].quaternion.y, player->aModel[0].quaternion.z, player->aModel[0].quaternion.w);
	// 文字列に代入
	sprintf(&aStr[7][0], "posV: %.3f|%.3f|%.3f\n", camera->posV.x, camera->posV.y, camera->posV.z);
	// 文字列に代入
	sprintf(&aStr[8][0], "posR: %.3f|%.3f|%.3f\n", camera->posR.x, camera->posR.y, camera->posR.z);
	// 文字列に代入
	sprintf(&aStr[9][0], "posVDest: %.3f|%.3f|%.3f\n", camera->posVDest.x, camera->posVDest.y, camera->posVDest.z);
	// 文字列に代入
	sprintf(&aStr[10][0], "posRDest: %.3f|%.3f|%.3f\n", camera->posRDest.x, camera->posRDest.y, camera->posRDest.z);
	// 文字列に代入
	sprintf(&aStr[11][0], "MODE: %d", mode);

	for (int i = 0; i < 12; i++)
	{
		// テキストの描画
		rect = { 0,i * 30,SCREEN_WIDTH,SCREEN_HEIGHT };

		g_pFont->DrawText(NULL, &aStr[i][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 255, 255));

	}
}

//=========================================
// モードの設定
//=========================================
void SetMode(MODE mode)
{
	// 現在の画面(モード)の終了処理
	switch (s_mode)
	{
	case MODE_TITLE:	// タイトル画面
		UninitTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		UninitGame();
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
//		UninitTutorial();
		break;

	case MODE_RANKING:	// ランキング画面
//		UninitRanking();
		break;
	}

	// 新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面
		InitTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		InitGame();
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
//		InitTutorial();
		break;

	case MODE_RANKING:	// ランキング画面
//		InitRanking();
//		SetRanking(GetScore());
		break;
	}

	s_mode = mode;	// 現在の画面(モード)を切り替える

}

//=========================================
// モードの取得
//=========================================
MODE GetMode(void)
{
	return s_mode;
}