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
static int	s_nCnt;
static bool	s_bFadeCheek;	// フェード処置に移行するかの処理
static int	s_nFadeCnt;		// フェード処理に行くまでの間隔
static bool s_bExit;		// Exitのフラグ

//=========================================
// 初期化処理
//=========================================
void InitTitle(void)
{
	s_nCnt = 0;

	InitCamera();		// カメラ
	InitLight();		// ライト
	InitPolygon();		// ポリゴン
	InitMeshField();	// メッシュフィールド
	InitModel();		// モデル
	InitPlayer();		// プレイヤー
	DeleteModel();		// プレイヤー以外のモデルの消失

	// ポリゴンの設定処理
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, -25.0f), &ZERO_VECTOR, D3DXVECTOR3(25.0f, 0.0f, 12.5f), "data/TEXTURE/WORD/Exit.jpg","exit");
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, -25.0f), &ZERO_VECTOR, D3DXVECTOR3(25.0f, 0.0f, 12.5f), "data/TEXTURE/WORD/Start.jpg", "start");

	// プレイヤーの設定処理
	Player* player = GetPlayer();
	player->pos = D3DXVECTOR3(0.0f, 0.0f, -25.0f);

	// メッシュフィールドの設定処理
	SetMesh setMesh;
	setMesh.file = NULL;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 30;
	setMesh.nSurfaceWidth = 30;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);

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
	if (CollisionPolygon(&GetPlayer()->pos, "start") && s_nCnt < 50)
	{
		s_Select = SELECT_GAMESTART;
		s_nCnt++;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "exit") && s_nCnt < 50)
	{
		s_Select = SELECT_EXIT;
		s_nCnt++;
	}
	else if(s_nCnt < 50)
	{
		s_nCnt = 0;
	}

	if (s_nCnt >= 50)
	{
		switch (s_Select)
		{
		case SELECT_GAMESTART:
			SetFade(MODE_GAME);	// ゲームモードに移行
			break;
		case SELECT_TUTORIAL:
			SetFade(MODE_TUTORIAL);	// チュートリアル画面に移行
			break;
		case SELECT_EXIT:
			s_bExit = true;
			break;
		default:
			break;
		}
	}
	else
	{
		UpdateCamera();		// カメラ
		UpdateLight();		// ライト
		UpdatePolygon();	// ポリゴン
		UpdateMeshField();	// メッシュフィールド
		UpdateModel();		// モデル
		UpdatePlayer();		// プレイヤー
	}
}

//=========================================
// 選択処理
//=========================================
void SelectTitle(void)
{

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
