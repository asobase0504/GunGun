//=========================================
//
// リザルト処理
// Author:Kishimoto Eiji
// Author:Yuda Kaito
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "result.h"
#include "input.h"
#include "common.h"
#include "fade.h"
// 2D

// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "mesh_sky.h"
#include "mesh_field.h"


//------------------------------------
// マクロ定義
//------------------------------------
#define FILENAME	("data/TEXTURE/Result_Hunter.png")

//=========================================
// 静的変数変数
//=========================================
static LPDIRECT3DTEXTURE9 s_pTexture[GAME_MAX] = { NULL,NULL };		// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9 s_pTextureWinner = NULL;					// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;					// 頂点バッファへのポインタ
static bool bWin;

//=========================================
// 初期化
//=========================================
void InitResult(void)
{
	// 初期化処理
	InitCamera();		// カメラ
	InitLight();		// ライト
	InitModel();		// モデル
	InitMeshSky();		// メッシュ(空)
	InitMeshField();	// メッシュ(地面)
}

//=========================================
// リザルトの終了処理
//=========================================
void UninitResult(void)
{
	// 終了処理
	UninitCamera();		// カメラ
	UninitLight();		// ライト
	UninitModel();		// モデル
	UninitMeshSky();	// メッシュ(空)
	UninitMeshField();	// メッシュ(地面)
}

//=========================================
// リザルトの更新処理
//=========================================
void UpdateResult(void)
{
	// 更新処理
	UpdateCamera();		// カメラ
	UpdateLight();		// ライト
	UpdateModel();		// モデル
	UpdateMeshSky();	// メッシュ(空)
	UpdateMeshField();	// メッシュ(地面)

	if (GetFade() == FADE_NONE && (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN)))
	{
		SetFade(MODE_TITLE);	 // タイトル画面に移行
	}
}

//=========================================
// リザルトの描画処理
//=========================================
void DrawResult(void)
{
	// 描画処理
	DrawModel();		// モデル
	DrawMeshSky();		// メッシュ(空)
	DrawMeshField();	// メッシュ(地面)
}
