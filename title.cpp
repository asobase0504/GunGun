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
#include "sound.h"
#include <assert.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define TITLEPRESS_WIDTH		(620)
#define TITLEPRESS_HEIGHT		(100)
#define TIMEUP_FADE				(90)	// フェードまでの時間

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
static bool s_bSelectSE;

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
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f),&D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), "data/TEXTURE/TITLE/Title_00.png", "Title1");
	SetPolygon(&D3DXVECTOR3(-15.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/TITLE/Title_01.png", "Title2");
	SetPolygon(&D3DXVECTOR3(15.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/TITLE/Title_00.png", "Title3");
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/TITLE/Title_01.png", "Title4");
	if (IsJoyPadUse(0))
	{
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 10.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/RightStick.png", "operation1");
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 30.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/LeftStick.png", "operation2");
	}
	else
	{
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 10.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/QE.png", "operation1");
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 30.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/WASD.png", "operation2");

	}
	SetPolygon(&D3DXVECTOR3(90.0f, 1.0f, 10.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/move.png", "operation3");
	SetPolygon(&D3DXVECTOR3(95.0f, 1.0f, 30.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/CameraMove.png", "operation4");
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, -25.0f), &ZERO_VECTOR, &D3DXVECTOR3(25.0f, 0.0f, 9.0f), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), "data/TEXTURE/WORD/Exit.png", "exit");
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, -25.0f), &ZERO_VECTOR, &D3DXVECTOR3(25.0f, 0.0f, 9.0f), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), "data/TEXTURE/WORD/Start.png", "start");

	// プレイヤーの設定処理
	Player* player = GetPlayer();
	player->pos = D3DXVECTOR3(0.0f, 0.0f, -25.0f);

	player->aModel[0]->quaternion = ZERO_QUATERNION;
	D3DXQuaternionRotationYawPitchRoll(&player->aModel[0]->quaternion, D3DX_PI * 0.25f, D3DX_PI * 0.25f, 0.0f);

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&player->aModel[0]->quaternion, &player->aModel[0]->quaternion);

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

	PlaySound(SOUND_LABEL_BGM_TITLE);
}

//=========================================
// 終了処理
//=========================================
void UninitTitle(void)
{
	StopSound();
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

	if (CollisionPolygon(&GetPlayer()->pos, "Title1") && !GetPolygon("Title1")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title1");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "Title2") && !GetPolygon("Title2")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title2");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "Title3") && !GetPolygon("Title3")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title3");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "Title4") && !GetPolygon("Title4")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title4");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation1") && !GetPolygon("operation1")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation1");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation2") && !GetPolygon("operation2")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation2");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation3") && !GetPolygon("operation3")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation3");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation4") && !GetPolygon("operation4")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation4");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}

	else if (CollisionPolygon(&GetPlayer()->pos, "start") && s_nCnt < TIMEUP_FADE)
	{	// プレイヤーが指定されたポリゴンに乗っていたらカウントを進める

		s_Select = SELECT_GAMESTART;
		s_nCnt++;
		GetPolygon("start")->col = D3DXCOLOR((float)s_nCnt / (float)TIMEUP_FADE, 0.0f, 0.0f, 1.0f);
		s_bSelectSE = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "exit") && s_nCnt < TIMEUP_FADE)
	{
		s_Select = SELECT_EXIT;
		s_nCnt++;
		GetPolygon("exit")->col = D3DXCOLOR((float)s_nCnt / (float)TIMEUP_FADE, 0.0f, 0.0f, 1.0f);
		s_bSelectSE = true;
	}
	else if(s_nCnt < TIMEUP_FADE)
	{
		s_nCnt = 0;
		GetPolygon("start")->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		GetPolygon("exit")->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		StopSound(SOUND_LABEL_SE_CHARGE);
		s_bSelectSE = false;
	}

	if (s_bSelectSE && s_nCnt == 1)
	{
		PlaySound(SOUND_LABEL_SE_CHARGE);
	}

	// 一定以上の値になったら乗っていた項目に以降する
	if (s_nCnt >= TIMEUP_FADE)
	{
		StopSound(SOUND_LABEL_SE_CHARGE);

		if (s_nCnt == TIMEUP_FADE)
		{
			// 何故か鳴らない。
			PlaySound(SOUND_LABEL_SE_SERECT);
			s_nCnt++;
		}
		
		switch (s_Select)
		{
		case SELECT_GAMESTART:
			SetFade(MODE_GAME);	// ゲームモードに移行
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

		// プレイヤーの移動制限
		Player* player = GetPlayer();
		if (player->pos.x < -130.0f)
		{
			player->pos.x = 120.0f;
		}
		else if (player->pos.x > 130.0f)
		{
			player->pos.x = -120.0f;
		}
		if (player->pos.z < -80.0f)
		{
			player->pos.z = 70.0f;
		}
		else if (player->pos.z > 80.0f)
		{
			player->pos.z = -70.0f;
		}
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
		DrawMeshField();		// メッシュフィールド
		DrawPolygon();			// ポリゴン
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
