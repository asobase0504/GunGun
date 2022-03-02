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
#include "billboard.h"
#include "mesh_field.h"
#include "mesh_cylinder.h"
#include "mesh_sphere.h"
#include "mesh_sky.h"
#include "line.h"
#include "pause.h"
#include "fan.h"
#include "timer.h"
#include "fade.h"
#include "debug.h"
#include "game_ui.h"
#include "particle.h"
#include "sound.h"
#include "shadow.h"
#include <stdio.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define MESH_FIELD	"data/TEXTURE/kusa.jpg"
#define WORLD_EDGE	(1300.0f)						// 世界の端
#define GAME_TIME	(90)

//------------------------------------
// スタティック変数
//------------------------------------
static bool s_bPause;				// ポーズ中かどうか
static bool s_bDebug;				// デバッグモードかどうか
static bool s_bCountDownTime;		// カウントダウン中かどうか 
static int nDelayCnt;				// 遅延時のカウント
static bool s_bGame;				// ゲーム中かゲームが始まる前か
static bool s_bEndGame;				// ゲームが終了
static int s_nEndCnt;				// ゲームが終了するまでの時間
static bool s_bAssat;				// ゲーム終了間際かどうか
static LPD3DXFONT pFont;

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	s_bPause = false;
	s_bDebug = false;
	s_bCountDownTime = false;
	s_bGame = false;
	s_bEndGame = false;
	s_bAssat = false;
	s_nEndCnt = 0;
	nDelayCnt = 0;

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
	InitShadow();		// 影
	InitPlayer();		// プレイヤー
	InitMeshField();	// メッシュ
	InitGameUI();		// UI
	InitMeshSky();		// メッシュスカイ
	InitParticle();		// パーティクル
	InitFan();

	// メッシュスカイの設定処理
	SetMeshSky();

	// タイムの設定処理
	StartTimer(3, 0, 40.0f, 180.0f, D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2.0f - 90.0f, 0.0f), 0);
	CountRestartStop(true, 0);

	// ポリゴンの設定処理
	SetPolygon(&D3DXVECTOR3(0.0f, -200.0f, 0.0f), &D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), &D3DXVECTOR3(100.0f, 0.0f, 100.0f),&D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), NULL, "floar");

	// メッシュフィールドの設定処理
	SetMesh setMesh;
	setMesh.file = MESH_FIELD;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 75;
	setMesh.nSurfaceWidth = 75;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);

	// 扇の設定処理
	SetFan(D3DXVECTOR3(10.0f, 30.0f, 10.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), 30.0f);

	// BGMの再生
	PlaySound(SOUND_LABEL_BGM_GAME);

	// フォントの初期化
	D3DXCreateFont(GetDevice(), 140, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07あかずきんポップ Heavy", &pFont);
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	StopSound();	// 音の再生を全てを停止

	// 終了
	UninitTimer();			// タイム
	UninitPause();			// ポーズ
	UninitPolygon();		// ポリゴン
	UninitCamera();			// カメラ
	UninitLight();			// ライト
	UninitShadow();			// 影
	UninitMeshField();		// メッシュ
	UninitMeshSky();		// メッシュ
	UninitGameUI();			// UI
	UninitParticle();		// パーティクル
	UninitFan();

#ifdef _DEBUG
	// ライン
	UninitLine();
#endif // !_DEBUG

	// タイマーの破棄
	BreakTimer(0);
	BreakTimer(1);

	// デバッグ表示用フォントの破棄
	if (pFont != NULL)
	{
		pFont->Release();
		pFont = NULL;
	}
}

static int nCnt = 1;

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	// ポーズの機能
	if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardTrigger(DIK_P))
	{
		s_bPause = !s_bPause;
	}

	// ポーズの機能
	if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardPress(DIK_O))
	{
		OpenFan(nCnt);
		nCnt++;
	}

	// ポーズ中ならポーズ以外を更新しない
	if (s_bPause)
	{
		UpdatePause();		// ポーズ
	}
	else
	{
		UpdateTimer();			// タイム
		UpdateGameCamera();		// カメラ

		// カウントダウン判定
		if (!TimerUp(0) && GetTimer(0)->bUse && !GetTimer(1)->bUse)
		{
			return;
		}

		// ゲームスタート時の遅延
		if (nDelayCnt < 30)
		{
			nDelayCnt++;
			return;
		}

		if (!s_bGame)
		{
			StartTimer(GAME_TIME, 1, 25.0f, 100.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			// タイマーの破棄
			BreakTimer(0);
			s_bCountDownTime = true;
			s_bGame = true;
		}

		if (GetTimer(1)->nSecond == 25 && !s_bAssat)
		{
			PlaySound(SOUND_LABEL_SE_TIME);
			s_bAssat = true;
		}

		if (GetTimer(1)->nSecond == 5 && !GetTimer(0)->bUse)
		{
			StartTimer(5, 0, 40.0f, 180.0f, D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2.0f - 90.0f, 0.0f), 0);
		}

		// 時間が切れたらリザルトに以降
		if (TimerUp(1))
		{
			BreakTimer(1);
			BreakTimer(0);
			PlaySound(SOUND_LABEL_SE_ENDGAME);
			s_bEndGame = true;
		}

		if (s_bEndGame)
		{
			s_nEndCnt++;

			if (s_nEndCnt >= 40)
			{
				GetPlayer()->pos.y += 1.0f;
			}
			if (s_nEndCnt >= 120)
			{
				SetFade(MODE_RESULT);
			}
		}
		else
		{
			// 更新
			UpdateModel();			// モデル
			UpdatePlayer();			// プレイヤー
			UpdateLight();			// ライト
			UpdatePolygon();		// ポリゴン
			UpdateShadow();			// 影
			UpdateMeshField();		// メッシュ
			UpdateMeshSky();		// メッシュスカイ
			UpdateGameUI();			// UI
			UpdateParticle();		// パーティクル
			UpdateFan();

#ifndef _GETMODEL_POP
			SetModelUI(SetJustModel());
#endif // !_GETMODEL_POP

		}

#ifdef _DEBUG
		UpdateLine();	// ライン

						// リザルト画面に移動
		if (GetJoypadTrigger(JOYKEY_X))
		{
			SetFade(MODE_RESULT);
		}
#endif // !_DEBUG

		// 世界の端
		Player* player = GetPlayer();
		if (player->pos.x + player->fLength >= WORLD_EDGE)
		{
			player->pos.x = WORLD_EDGE - player->fLength;
		}
		if (player->pos.x - player->fLength <= -WORLD_EDGE)
		{
			player->pos.x = -WORLD_EDGE + player->fLength;
		}
		if (player->pos.z + player->fLength >= WORLD_EDGE)
		{
			player->pos.z = WORLD_EDGE - player->fLength;
		}
		if (player->pos.z - player->fLength <= -WORLD_EDGE)
		{
			player->pos.z = -WORLD_EDGE + player->fLength;
		}
	}
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

		DrawModel();		// モデル
		DrawPlayer();		// プレイヤー
		DrawMeshField();	// メッシュ
		DrawMeshSky();		// メッシュスカイ
		DrawShadow();		// 影
		DrawFan();

		if (s_bGame)
		{
			DrawParticle();		// パーティクル
			DrawGameUI();		// UI
			DrawPolygonUI();	// ポリゴンUI
		}

		//2Dの前に3Dを置ける可能性
		pDevice->Clear(0, NULL,
			(D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			DrawModelUI();		// モデルUI
		//DrawTimer();		// タイム

		if (s_bPause)
		{
			DrawPause();
		}

		if (s_bEndGame)
		{
			RECT rect = { (LONG)0.0f,(LONG)(SCREEN_HEIGHT / 2.0f - 90.0f),(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };

			pFont->DrawText(NULL, "しゅうりょう～～～！！", -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
		}

#ifdef _DEBUG
		DrawLine();		// ライン
		//DrawFPS();	// FPSの表示
#endif // !_DEBUG
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
