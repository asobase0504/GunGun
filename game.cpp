//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
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

//------------------------------------
// スタティック変数
//------------------------------------
static bool s_bPause;			// ポーズ中かどうか
static int	s_nSizeCnt;			// 大きさの区切り回数

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	s_bPause = false;
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

	// タイムの設定処理
	StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	//// 壁の設定処理
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
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

#ifdef _DEBUG
	// ライン
	UninitLine();
#endif // !_DEBUG

	// タイマーの破棄
	BreakTimer(0);
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	if (GetJoypadTrigger(JOYKEY_START))
	{
		s_bPause = !s_bPause;
	}

	if (s_bPause)
	{
		UpdatePause();	// ポーズ
	}
	else
	{
		// 更新
		UpdateModel();			// モデル
		UpdatePlayer();			// プレイヤー
		UpdateGameCamera();		// カメラ
		UpdateLight();			// ライト
		UpdatePolygon();		// ポリゴン
		UpdateShadow();			// 影
		UpdateMeshField();		// メッシュ
		UpdateWall();			// 壁
		UpdateTimer();			// タイム
#ifdef _DEBUG
		UpdateLine();	// ライン
#endif // !_DEBUG
	}

	if ((int)GetPlayer()->fLength / 14 == s_nSizeCnt)
	{
		GetCamera()->fDistance *= 1.5f;
		s_nSizeCnt++;
	}

	// 時間が切れたらリザルトに以降
	if (TimerUp(0) || GetJoypadTrigger(JOYKEY_X))
	{
		SetFade(MODE_RESULT);
	}
}

//=========================================
// 描画
//=========================================
void DrawGame(void)
{
	SetCamera();			// カメラ
	DrawWall();				// 壁
	DrawModel();			// モデル
	DrawPlayer();			// プレイヤー
	DrawMeshField();		// メッシュ
	DrawShadow();			// 影
	DrawModelUI();			// モデルUI

	// タイム
	DrawTimer();

#ifdef _DEBUG
	DrawLine();	// ライン
#endif // !_DEBUG

	if (s_bPause)
	{
		DrawPause();
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
