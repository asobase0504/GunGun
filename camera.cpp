//=========================================
// 
// カメラの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define CAMERA_MOVE			(1.0f)
#define CAMERA_ROT_VOLUME	(0.03f)

//-----------------------------------------
// 静的変数
//-----------------------------------------
static Camera s_camera;	// カメラ情報
static float rotY;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void InputCamera(void);	// カメラの入力処理

//=========================================
// 初期化
//=========================================
void InitCamera(void)
{
	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f,50.0f,-40.0f);	// 視点
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 注視点
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// 上方向ベクトル
	s_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//s_camera.rot.y = atan2f((s_camera.posV.x - s_camera.posR.x), (s_camera.posV.z - s_camera.posR.z));

	s_camera.fDistance = sqrtf((s_camera.posV.x - s_camera.posR.x) * (s_camera.posV.x - s_camera.posR.x) + (s_camera.posV.z - s_camera.posR.z) * (s_camera.posV.z - s_camera.posR.z));
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
	Camera* pCamara = &(s_camera);

	InputCamera();

	// 角度の正規化
	if (pCamara->rot.y > D3DX_PI)
	{
		pCamara->rot.y -= D3DX_PI * 2;
	}
	if (pCamara->rot.y < -D3DX_PI)
	{
		pCamara->rot.y += D3DX_PI * 2;
	}

	pCamara->posV.x = pCamara->posR.x - sinf(pCamara->rot.y) * pCamara->fDistance;
	pCamara->posV.z = pCamara->posR.z - cosf(pCamara->rot.y) * pCamara->fDistance;

}

//=========================================
// 設定
//=========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camara->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camara->mtxView, &camara->posV, &camara->posR, &camara->vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camara->mtxView);	// ビュー座標行列の設定

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&camara->mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&camara->mtxProjection,
		D3DXToRadian(45.0f),						// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,100.0f);								// どこから(ニア)(第５引数)どこまで(ファー)(第６引数)をカメラで表示するか設定 

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// 入力
//=========================================
void InputCamera(void)
{
	Camera* pCamara = &(s_camera);
	float fAngle;

	// カメラの移動
	if (GetKeyboardPress(DIK_W))
	{
		if (GetKeyboardPress(DIK_A))
		{
			fAngle = D3DX_PI * -0.25f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else if (GetKeyboardPress(DIK_S))
		{
		}
		else if (GetKeyboardPress(DIK_D))
		{
			fAngle = D3DX_PI * 0.25f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else
		{
			fAngle = pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_A))
	{
		if (GetKeyboardPress(DIK_S))
		{
			fAngle = D3DX_PI * -0.75f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else if (GetKeyboardPress(DIK_D))
		{

		}
		else
		{
			fAngle = D3DX_PI * -0.5f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_S))
	{
		if (GetKeyboardPress(DIK_D))
		{
			fAngle = D3DX_PI * 0.75f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else
		{
			fAngle = D3DX_PI + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{
		fAngle = D3DX_PI * 0.5f + pCamara->rot.y;
		pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
		pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
	}
	pCamara->posR.x = pCamara->posV.x + sinf(pCamara->rot.y) * pCamara->fDistance;
	pCamara->posR.z = pCamara->posV.z + cosf(pCamara->rot.y) * pCamara->fDistance;

	// 注視点角度の回転
	if (GetKeyboardPress(DIK_Z))
	{
		pCamara->rot.y += CAMERA_ROT_VOLUME;	// 回転量
		pCamara->posV.x = pCamara->posR.x - sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posV.z = pCamara->posR.z - cosf(pCamara->rot.y) * pCamara->fDistance;
	}
	if (GetKeyboardPress(DIK_C))
	{
		pCamara->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
		pCamara->posV.x = pCamara->posR.x - sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posV.z = pCamara->posR.z - cosf(pCamara->rot.y) * pCamara->fDistance;
	}

	// 視点角度の回転
	if (GetKeyboardPress(DIK_Q))
	{
		pCamara->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
		pCamara->posR.x = pCamara->posV.x + sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posR.z = pCamara->posV.z + cosf(pCamara->rot.y) * pCamara->fDistance;
	}
	if (GetKeyboardPress(DIK_E))
	{
		pCamara->rot.y += CAMERA_ROT_VOLUME;	// 回転量
		pCamara->posR.x = pCamara->posV.x + sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posR.z = pCamara->posV.z + cosf(pCamara->rot.y) * pCamara->fDistance;
	}


	// z = r sinθ cos?
	// x = r sinθ sin(?)
	// y = r cosθ
}

//=========================================
// カメラの角度情報取得
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera.rot;
}