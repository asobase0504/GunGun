//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
#include "game.h"
#include "main.h"
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

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
#ifdef _DEBUG
	// ラインの初期化処理
	InitLine();
#endif // !_DEBUG

	// ポリゴンの初期化処理
	InitPolygon();

	// プレイヤーの初期化処理
	InitPlayer();

	// 影の初期化処理
	InitShadow();

	// モデルの初期化処理
	InitModel();

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// ビルボードの初期化処理
	InitBillboard();

	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// メッシュの初期化処理
	InitMeshBuild();

	// メッシュ(円柱)の初期化処理
	InitMeshCylinder();

	// メッシュ(球)の初期化処理
	InitMeshSphere();

	// メッシュ(空)の初期化処理
	InitMeshSky();

	// 壁の初期化処理
	InitWall();

	//// 壁の設定処理
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
}

//=========================================
// 終了
//=========================================
void UnInitGame(void)
{
	// ポリゴンの終了処理
	UninitPolygon();

	// プレイヤーの終了処理
	UninitPlayer();

	// モデルの終了処理
	UninitModel();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 影の終了処理
	UninitShadow();

	// ビルボードの終了処理
	UninitBillboard();

	// メッシュの終了処理
	UninitMeshBuild();

	// メッシュ(円柱)の終了処理
	UninitMeshCylinder();

	// メッシュ(球)の終了処理
	UninitMeshSphere();

	// メッシュ(空)の終了処理
	UninitMeshSky();

	// 壁の終了処理
	UninitWall();

#ifdef _DEBUG
	// ラインの更新処理
	UninitLine();
#endif // !_DEBUG
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	// モデルの更新
	UpdateModel();

	// プレイヤーの更新処理
	UpdatePlayer();

	// カメラの更新
	UpdateCamera();

	// ライトの更新
	UpdateLight();

	// ポリゴンの更新
	UpdatePolygon();

	// 影の更新
	UpdateShadow();

	// ビルボードの更新
	UpdateBillboard();

	// メッシュの更新処理
	UpdateMeshBuild();

	// メッシュ(円柱)の更新処理
	UpdateMeshCylinder();

	// メッシュ(球)の更新処理
	UpdateMeshSphere();

	// メッシュ(空)の更新処理
	UpdateMeshSky();

	// 壁の更新
	UpdateWall();

#ifdef _DEBUG
	// ラインの更新処理
	UpdateLine();
#endif // !_DEBUG
}

//=========================================
// 描画
//=========================================
void DrawGame(void)
{
	// カメラの設定処理
	SetCamera();

	// ポリゴンの描画処理
	//DrawPolygon();

	// 壁の描画処理
	DrawWall();

	// モデルの描画処理
	DrawModel();

	// プレイヤーの描画処理
	DrawPlayer();

	// メッシュの描画処理
	DrawMeshBuild();

	//// メッシュ(円柱)の描画処理
	//DrawMeshCylinder();

	//// メッシュ(球)の描画処理
	//DrawMeshSphere();

	//// メッシュ(空)の描画処理
	//DrawMeshSky();

	// 影の描画処理
	DrawShadow();

	// モデルの描画処理
	//		DrawModel();

	//// ビルボードの描画処理
	//DrawBillboard();

#ifdef _DEBUG
	// ラインの描画処理
	DrawLine();
#endif // !_DEBUG
}
