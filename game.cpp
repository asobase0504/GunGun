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
static bool s_bPause;			//ポーズ中かどうか

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	s_bPause = false;
#ifdef _DEBUG
	// ラインの初期化処理
	InitLine();
#endif // !_DEBUG

	// 初期化
	InitTimer();		// タイム
	InitPause();		// ポーズ
	InitCamera();		// カメラ
	InitLight();		// ライト
	InitPolygon();		// ポリゴン
	InitModel();		// モデル
	InitPlayer();		// プレイヤー
	InitShadow();		// 影
	InitBillboard();	// ビルボード
	InitMeshField();	// メッシュ
	InitMeshCylinder();	// メッシュ(円柱)
	InitMeshSphere();	// メッシュ(球)
	InitMeshSky();		// メッシュ(空)
	InitWall();			// 壁

	// タイムの設定処理
	StartTimer(90, 1, 20.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 60.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	// ビルボードの設定処理
	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

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
	//UninitModel();			// モデル
	//UninitPlayer();			// プレイヤー
	UninitTimer();			// タイム
	UninitPause();			// ポーズ
	UninitPolygon();		// ポリゴン
	UninitCamera();			// カメラ
	UninitLight();			// ライト
	UninitShadow();			// 影
	UninitBillboard();		// ビルボード
	UninitMeshField();		// メッシュ
	UninitMeshCylinder();	// メッシュ(円柱)
	UninitMeshSphere();		// メッシュ(球)
	UninitMeshSky();		// メッシュ(空)
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
		UpdateBillboard();		// ビルボード
		UpdateMeshField();		// メッシュ
		UpdateMeshCylinder();	// メッシュ(円柱)
		UpdateMeshSphere();		// メッシュ(球)
		UpdateMeshSky();		// メッシュ(空)
		UpdateWall();			// 壁
		UpdateTimer();			// タイム
#ifdef _DEBUG
		UpdateLine();	// ライン
#endif // !_DEBUG
	}

	// 時間が切れたらリザルトに以降
	if (TimerUp(0) == 0 || GetJoypadTrigger(JOYKEY_X))
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
	//DrawPolygon();		// ポリゴン
	DrawWall();				// 壁
	DrawModel();			// モデル
	DrawPlayer();			// プレイヤー
	DrawMeshField();		// メッシュ
	//DrawMeshCylinder();	// メッシュ(円柱)
	//DrawMeshSphere();		// メッシュ(球)
	//DrawMeshSky();		// メッシュ(空)
	DrawShadow();			// 影
	//DrawModel();			// モデル
	//DrawBillboard();		// ビルボード

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
