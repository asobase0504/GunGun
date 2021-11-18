//=========================================
// 
// カメラの作成
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "camera.h"
#include "input.h"

//-----------------------------------------
// 静的変数
//-----------------------------------------
static Camera s_camera;	// カメラ情報
static float rotY;

//=========================================
// 初期化
//=========================================
void InitCamera(void)
{
	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f,10.0f,-30.0f);
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

//=========================================
// 終了
//=========================================
void UninitCamera(void)
{
}

//=========================================
// 更新
//=========================================
void UpdateCamera(void)
{
	if (GetKeyboardTrigger(DIK_W))
	{
		s_camera.posV.z += 5.0f;
	}

	if (GetKeyboardPress(DIK_A))
	{
		rotY -= 0.5f;
		float radY = rotY / (D3DX_PI * 2.0f);
		float c = cosf(radY);
		float s = sinf(radY);
		s_camera.posV = D3DXVECTOR3(30.0f * c, s_camera.posV.y, 30.0f * s);
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		s_camera.posV.z -= 5.0f;
	}

	if (GetKeyboardPress(DIK_D))
	{
		rotY += 0.5f;
		float radY = rotY / (D3DX_PI * 2.0f);
		float c = cosf(radY);
		float s = sinf(radY);
		s_camera.posV = D3DXVECTOR3(30.0f * c, s_camera.posV.y, 30.0f * s);
	}

	if (GetKeyboardTrigger(DIK_Q))
	{
		s_camera.posV.y += 5.0f;
	}

	if (GetKeyboardTrigger(DIK_Z))
	{
		s_camera.posV.y -= 5.0f;
	}
}

//=========================================
// 設定
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
		D3DXToRadian(45.0f),						// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,100.0f);								// どこから(ニア)(第５引数)どこまで(ファー)(第６引数)をカメラで表示するか設定 

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}
