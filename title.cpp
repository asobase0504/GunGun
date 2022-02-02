//=========================================
// 
// タイトルの処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "title.h"
#include "input.h"
#include "key_config.h"
#include "fade.h"
#include "common.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "polygon.h"
#include "mesh_field.h"
#include "player.h"
#include "model.h"

#include <assert.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)

//------------------------------------
// プレスエンターの状態の種類
//------------------------------------
typedef enum
{
	TYPE_NOME,	// 変化なし
	TYPE_BLINK,	// 点滅状態
	TYPE_MAX
}PRESS_TYPE;

//------------------------------------
// 配置するオブジェクトの種類
//------------------------------------
typedef enum
{
	OBJ_BG,				// スクリーン背景
	OBJ_SELECTBG,		// 選択画面背景
	OBJ_TITLE,			// タイトル文字
	OBJ_GAMESTART,		// GAMESTART文字
	OBJ_TUTORIAL,		// TUTORIAL文字
	OBJ_EXIT,			// EXIT文字
	OBJ_SERECTCURSOR,	// セレクトカーソル
	OBJ_MAX				// 
}OBJ_TYPE;

//------------------------------------
// 選択されたオブジェクト
//------------------------------------
typedef enum
{
	SELECT_GAMESTART = OBJ_GAMESTART,
	SELECT_TUTORIAL,
	SELECT_EXIT,
}SELECT_MODE;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 Tex = NULL;				// テクスチャへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 角度
	D3DXCOLOR col;								// 色
	bool bUse;									// 使用してるかどうか
	float Height;								// 高さ
	float Width;								// 幅
}OBJECT;

//------------------------------------
// 静的変数変数
//------------------------------------
static SELECT_MODE s_Select;
static OBJECT s_Object[OBJ_MAX] = {};
static bool	s_bFadeCheek;	// フェード処置に移行するかの処理
static int	s_nFadeCnt;		// フェード処理に行くまでの間隔
static bool s_bExit;		// Exitのフラグ

//=========================================
// 初期化処理
//=========================================
void InitTitle(void)
{
	InitCamera();		// カメラ
	InitLight();		// ライト
	InitPolygon();		// ポリゴン
	InitMeshField();	// メッシュフィールド
	InitModel();		// モデル
	InitPlayer();		// プレイヤー

	// ポリゴンの設定処理
	SetPolygon(&ZERO_VECTOR, &ZERO_VECTOR, D3DXVECTOR3(50.0f, 1.0f, 50.0f), "data/TEXTURE/07.彼方への君に捧ぐ.png");
}

//=========================================
// 終了処理
//=========================================
void UninitTitle(void)
{
	UninitCamera();		// カメラ
	UninitLight();		// ライト
	UninitPolygon();	// ポリゴン
	UninitMeshField();	// メッシュフィールド
	UninitModel();		// モデル
	UninitPlayer();		// プレイヤー
}

//=========================================
// 更新処理
//=========================================
void UpdateTitle(void)
{
	UpdateCamera();		// カメラ
	UpdateLight();		// ライト
	UpdatePolygon();	// ポリゴン
	UpdateMeshField();	// メッシュフィールド
	UpdateModel();		// モデル
	UpdatePlayer();		// プレイヤー
}

//=========================================
// 選択処理
//=========================================
void SelectTitle(void)
{
	// ジョイパッドの使用情報の取得
	bool bUseJoyPad = IsJoyPadUse(0);

	switch (s_Select)
	{
	case SELECT_GAMESTART:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_EXIT;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_TUTORIAL;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_EXIT;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_TUTORIAL;
			}
		}
		break;
	case SELECT_TUTORIAL:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_GAMESTART;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_EXIT;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_GAMESTART;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_EXIT;
			}
		}
		break;
	case SELECT_EXIT:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_TUTORIAL;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_GAMESTART;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_TUTORIAL;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_GAMESTART;
			}
		}
		break;
	default:
		break;
	}
}

//=========================================
// 描画処理
//=========================================
void DrawTitle(int cameraData)
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

		SetCamera(cameraData);	// カメラ
		DrawPolygon();			// ポリゴン
		DrawMeshField();		// メッシュフィールド
		DrawModel();		// モデル
		DrawPlayer();			// プレイヤー
	}
}

//=========================================
// 終了情報を取得
//=========================================
bool GetExit(void)
{
	return s_bExit;
}
