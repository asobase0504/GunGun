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
#include "sound.h"
// 2D
#include "result_ui.h"
// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "player.h"
#include "mesh_field.h"
#include "polygon.h"

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
	InitPolygon();		// ポリゴン

	// プレイヤーの設定
	s_player = GetPlayer();
	s_player->pos = ZERO_VECTOR;
	s_player->pos.y = 50.0f;

	// モデルの設定
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

	// ポリゴンの設定
	SetPolygon(&D3DXVECTOR3(0.0f, (400.0f * GetCamera(0)->fDistance / 60.0f) - 10.0f, 0.0f), &D3DXVECTOR3(D3DX_PI*-0.5f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), "data/TEXTURE/sky.png", "wall");

	PlaySound(SOUND_LABEL_BGM_RESULT);
}

//=========================================
// リザルトの終了処理
//=========================================
void UninitResult(void)
{
	StopSound();
	// 終了処理
	UninitModel();		// モデル
	UninitPlayer();		// プレイヤー
	UninitResultUI();	// リザルトUI
	UninitCamera();		// カメラ
	UninitLight();		// ライト
	UninitPolygon();	// ポリゴン
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
	UpdatePolygon();		// ポリゴン

	switch (modeResult)
	{
	case RESULT_0:

		if (DecisionKey())
		{
			modeResult = RESULT_1;
		}

		break;
	case RESULT_1:
	{
		break;
	}
	default:
		break;
	}

	D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 回転軸

	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.025f);	// 回転軸と回転角度を指定

	s_player->aModel[0]->quaternion *= quaternion;
	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&s_player->aModel[0]->quaternion, &s_player->aModel[0]->quaternion);

	if (GetFade() == FADE_NONE && DecisionKey())
	{
		SetFade(MODE_TITLE);	 // タイトル画面に移行
	}
}

//=========================================
// リザルトの描画処理
//=========================================
void DrawResult(int cameraData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	if (cameraData == 0)
	{
		// ビューボードのクリア
		pDevice->SetViewport(&GetCamera(cameraData)->viewport);

		// 画面クリア(バックバッファ＆Zバッファのクリア)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		
		// 描画処理
		SetCamera(cameraData);		// カメラ
		DrawModel();				// モデル
		DrawPlayer();				// プレイヤー
		DrawResultUI();				// リザルトUI
		DrawPolygon();				// ポリゴン
	}
}

//=========================================
// リザルトのモードを取得
//=========================================
RESULT_MODE* GetResult(void)
{
	return &modeResult;
}