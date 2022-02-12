//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "game.h"
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
#include "line.h"
#include "pause.h"
#include "timer.h"
#include "fade.h"
#include "debug.h"
#include "game_ui.h"
#include "item.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MESH_FIELD	"data/TEXTURE/kusa.jpg"

//------------------------------------
// スタティック変数
//------------------------------------
static bool s_bPause;			// ポーズ中かどうか
static int	s_nSizeCnt;			// 大きさの区切り回数
static bool s_bDebug;			// デバッグモードかどうか
static bool s_bCountDownTime;	// カウントダウン中か否か 
static int	nDelayCnt;			// 遅延時のカウント

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	s_bPause = false;
	s_bDebug = false;
	s_bCountDownTime = false;

	s_nSizeCnt = 1;
#ifdef _DEBUG
	// ラインの初期化処理
	InitLine();
#endif // !_DEBUG

	// 初期化
	InitTimer();		// タイム
	InitPause();		// ポーズ
	InitPolygon();		// ポリゴン
	InitCamera();		// カメラ
	InitLight();		// ライト
	InitModel();		// モデル
	InitPlayer();		// プレイヤー
	InitShadow();		// 影
	InitMeshField();	// メッシュ
	InitWall();			// 壁
	InitGameUI();		// UI

	// タイムの設定処理
	StartTimer(3, 0, 40.0f, 80.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 0);
	CountRestartStop(true, 0);
	CountRestartStop(true, 1);

	// ポリゴンの設定処理
	SetPolygon(&D3DXVECTOR3(0.0f, -200.0f, 0.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), NULL, "floar");
	SetPolygonUI(&D3DXVECTOR3(-25.5f, -10.5f, 30.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 0.0f, 5.0f), "data/TEXTURE/Circle.png");

	// メッシュフィールドの設定処理
	SetMesh setMesh;
	setMesh.file = MESH_FIELD;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 30;
	setMesh.nSurfaceWidth = 30;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	// 終了
	UninitTimer();			// タイム
	UninitPause();			// ポーズ
	UninitPolygon();		// ポリゴン
	UninitCamera();			// カメラ
	UninitLight();			// ライト
	UninitShadow();			// 影
	UninitMeshField();		// メッシュ
	UninitWall();			// 壁
	UninitGameUI();			// UI

#ifdef _DEBUG
	// ライン
	UninitLine();
	
#endif // !_DEBUG

	// タイマーの破棄
	BreakTimer(1);
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	// ポーズ中ならポーズ以外を更新しない
	if (s_bPause)
	{
		UpdatePause();		// ポーズ
		return;
	}

	UpdateTimer();			// タイム

	// カウントダウン判定
	if (!TimerUp(0) && GetTimer(0)->bUse)
	{
		return;
	}

	// ゲームスタート時の遅延
	if (nDelayCnt < 30)
	{
		nDelayCnt++;
		return;
	}

	if (!s_bCountDownTime)
	{
		StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
		// タイマーの破棄
		BreakTimer(0);
		s_bCountDownTime = true;
	}

	if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardTrigger(DIK_P))
	{
		s_bPause = !s_bPause;
	}

	// 更新
	UpdateModel();			// モデル
	UpdatePlayer();			// プレイヤー
	UpdateGameCamera();		// カメラ
	UpdateLight();			// ライト
	UpdatePolygon();		// ポリゴン
	UpdateShadow();			// 影
	UpdateMeshField();		// メッシュ
	UpdateWall();			// 壁

	UpdateGameUI();			// UI

	Player* player = GetPlayer();
	// プレイヤーが画面一杯になったら画面の拡大
	if ((int)player->fLength / 24 == s_nSizeCnt)
	{
		GetCamera(0)->fDistance *= 1.5f;
		s_nSizeCnt++;
	}

	// 時間が切れたらリザルトに以降
	if (TimerUp(1))
	{
		SetFade(MODE_RESULT);
	}

#ifdef _DEBUG
	UpdateLine();	// ライン

	// エディタモードの切り替え
	if (GetJoypadTrigger(JOYKEY_BACK))
	{
		s_bDebug = !s_bDebug;

		if (s_bDebug)
		{
			InitItem();
		}
		else
		{
			UninitItem();
		}
	}
	// リザルト画面に移動
	if (GetJoypadTrigger(JOYKEY_X))
	{
		SetFade(MODE_RESULT);
	}
	// マップの更新
	if (GetJoypadTrigger(JOYKEY_Y))
	{
		OutputMap("data/map02.txt");
	}
#endif // !_DEBUG
}

//=========================================
// 描画
//=========================================
void DrawGame(int cameraData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
												
	switch (cameraData)
	{
	case 0:
		// ビューボードのクリア
		pDevice->SetViewport(&GetCamera(cameraData)->viewport);

		// 画面クリア(バックバッファ＆Zバッファのクリア)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		SetCamera(cameraData);			// カメラ

		DrawWall();			// 壁
		DrawModel();		// モデル
		DrawPlayer();		// プレイヤー
		DrawMeshField();	// メッシュ
		DrawShadow();		// 影
		DrawModelUI();		// モデルUI
		DrawGameUI();		// UI
		DrawPolygonUI();	// ポリゴンUI

		if (s_bPause)
		{
			DrawPause();
		}

#ifdef _DEBUG
		if (s_bDebug)
		{
			DrawItem();
		}

		DrawLine();		// ライン
		DrawFPS();		// FPSの表示
#endif // !_DEBUG

		DrawTimer();		// タイム
		break;
	//case 1:
	//	DrawPolygon();
	//	break;
	default:
		break;
	}
}

//=========================================
// ポーズの設定処理
//=========================================
void SetEnablePause(bool bUse)
{
	s_bPause = bUse;
}

//=========================================
// ポーズの状態を取得
//=========================================
bool GetPause(void)
{
	return s_bPause;
}
