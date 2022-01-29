//=========================================
//
// リザルト処理
// Author:Yuda Kaito
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "result.h"
#include "input.h"
#include "key_config.h"
#include "common.h"
#include "fade.h"
// 2D
#include "result_ui.h"
// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "player.h"
#include "mesh_sky.h"
#include "mesh_field.h"

//------------------------------------
// マクロ定義
//------------------------------------

//=========================================
// 静的変数変数
//=========================================
static RESULT_MODE modeResult;	// リザルトのモード
static Player* s_player;		// プレイヤーデータ
static float upSpead;			// モデルの上昇スピード

//=========================================
// 初期化
//=========================================
void InitResult(void)
{
	modeResult = RESULT_0;	// モードのリセット

	// 初期化処理
	InitResultUI();		// リザルトUI
	InitCamera();		// カメラ
	InitLight();		// ライト
	InitMeshSky();		// メッシュ(空)
	SetMeshSky();		// メッシュ(空)の設定


	// プレイヤーのゲット
	s_player = GetPlayer();
	s_player->pos = ZERO_VECTOR;
	s_player->pos.y = 50.0f;

	// モデル
	for (int i = 0; i < sizeof(s_player->aModel) / sizeof(s_player->aModel[0]); i++)
	{
		if (s_player->aModel[i] == NULL || !s_player->aModel[i]->bUse)
		{
			continue;
		}
		if (s_player->aModel[i]->nIdxModelParent == -2)
		{
			s_player->aModel[i]->bUse = false;
		}

		SetModel(s_player->aModel[i]);	// 設定
	}

	upSpead = 0.01f;
}

//=========================================
// リザルトの終了処理
//=========================================
void UninitResult(void)
{
	// 終了処理
	UninitModel();		// モデル
	UninitPlayer();		// プレイヤー
	UninitResultUI();	// リザルトUI
	UninitCamera();		// カメラ
	UninitLight();		// ライト
	UninitMeshSky();	// メッシュ(空)
}

//=========================================
// リザルトの更新処理
//=========================================
void UpdateResult(void)
{
	// 更新処理
	UpdateResultUI();		// リザルトUI
	UpdateResultCamera();	// カメラ
	UpdateLight();			// ライト
	UpdateModel();			// モデル
	UpdateMeshSky();		// メッシュ(空)

	switch (modeResult)
	{
	case RESULT_0:

		if (DecisionKey())
		{
			modeResult = RESULT_1;
		}

		break;
	case RESULT_1:

		if (s_player->pos.y <= 500.0f)
		{
			upSpead += 0.01f;	// 速度上昇
			s_player->pos.y += upSpead * upSpead;
		}

		if (GetFade() == FADE_NONE && DecisionKey())
		{
			SetFade(MODE_TITLE);	 // タイトル画面に移行
		}
		break;
	default:
		break;
	}

}

//=========================================
// リザルトの描画処理
//=========================================
void DrawResult(void)
{
	// 描画処理
	SetCamera();		// カメラ
	DrawModel();		// モデル
	DrawPlayer();		// プレイヤー
	DrawMeshSky();		// メッシュ(空)
	DrawResultUI();		// リザルトUI
}

//=========================================
// リザルトのモードを取得
//=========================================
RESULT_MODE* GetResult(void)
{
	return &modeResult;
}