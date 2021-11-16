//=========================================
// 
// カメラの作成
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "camera.h"

//------------------------------------
// static変数
//------------------------------------
Camera s_camera;	// カメラ情報

//=========================================
// 初期化処理
//=========================================
void InitCamera(void)
{
	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f,0.0f,0.0f);
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

//=========================================
// 終了処理
//=========================================
void UninitCamera(void)
{
}

//=========================================
// 更新処理
//=========================================
void UpdateCamera(void)
{
}

//=========================================
// 設定処理
//=========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&s_camera.mtxView, &s_camera.posV, &s_camera.posR, &s_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);	// ビュー座標行列の設定

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&s_camera.mtxProjection,
		D3DXToRadian(45.0f),	// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,100.0f);	// どこから(ニア)(第５引数)どこまで(ファー)(第６引数)をカメラで表示するか設定 

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}
