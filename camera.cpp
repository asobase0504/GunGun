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
#include "player.h"
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
static int i;
//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void InputCamera(void);	// カメラの入力処理

//=========================================
// 初期化
//=========================================
void InitCamera(void)
{
	ZeroMemory(&s_camera, sizeof(s_camera));

	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f,50.0f,-60.0f);	// 視点
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 注視点
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// 上方向ベクトル
	s_camera.rot = ZERO_VECTOR;	// 向き

	s_camera.rot.y = atan2f((s_camera.posR.x - s_camera.posV.x),(s_camera.posR.z - s_camera.posV.z));
	s_camera.rot.x = atan2f((s_camera.posR.z - s_camera.posV.z),(s_camera.posR.y - s_camera.posV.y));

	D3DXVECTOR3 vec = s_camera.posV - s_camera.posR;
	s_camera.fDistance = D3DXVec3Length(&vec);

	i = 1;
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

}

//=========================================
// ゲーム中の更新
//=========================================
void UpdateGameCamera(void)
{
	Camera* pCamera = &(s_camera);

	InputCamera();

	// 角度の正規化
	if (pCamera->rot.y > D3DX_PI)
	{
		pCamera->rot.y -= D3DX_PI * 2;
	}
	if (pCamera->rot.y < -D3DX_PI)
	{
		pCamera->rot.y += D3DX_PI * 2;
	}

	// 追従処理
	Player *player = GetPlayer();

	// カメラの追従処理
	pCamera->posRDest.x = player->pos.x + sinf(player->movevec.x) * 40.0f;
	pCamera->posRDest.y = player->pos.y;
	pCamera->posRDest.z = player->pos.z + sinf(player->movevec.z) * 40.0f;

	pCamera->posR += (pCamera->posRDest - pCamera->posR) * 0.05f;
	pCamera->posR.y = player->pos.y;

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

	if (GetKeyboardPress(DIK_9))
	{
		pCamera->fDistance++;
	}
	if (GetKeyboardPress(DIK_0))
	{
		pCamera->fDistance--;
	}
}

//=========================================
// リザルト中の更新
//=========================================
void UpdateResultCamera(void)
{
	Camera* pCamera = &(s_camera);

	Player *player = GetPlayer();

	// カメラの追従処理
	pCamera->posR.x += (player->pos.x - pCamera->posR.x) * 0.05f;
	pCamera->posR.z += (player->pos.z - pCamera->posR.z) * 0.05f;

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

	pCamera->posR.y = player->pos.y;
	//// 徐々に上に上がる
	//if (pCamera->posR.y <= 75.0f)
	//{
	//	pCamera->posR.y += 0.5f;	// 注視点
	//}

	// 角度の正規化
	if (pCamera->rot.y > D3DX_PI)
	{
		pCamera->rot.y -= D3DX_PI * 2;
	}
	if (pCamera->rot.y < -D3DX_PI)
	{
		pCamera->rot.y += D3DX_PI * 2;
	}
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
		D3DXToRadian(60.0f),						// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,400.0f * camara->fDistance / 60.0f);								// どこから(ニア)(第５引数)どこまで(ファー)(第６引数)をカメラで表示するか設定 

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// 入力
//=========================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera);

	pCamera->vec = ZERO_VECTOR;

	if (IsJoyPadUse(0))
	{// ジョイパッドの使用
		if (GetJoypadPress(JOYKEY_RIGHT_SHOULDER))
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
		if (GetJoypadPress(JOYKEY_LEFT_SHOULDER))
		{
			pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
	}
	else
	{
		// カメラの移動
		if (GetKeyboardPress(DIK_W))
		{
			pCamera->vec.x += sinf(pCamera->rot.y);
			pCamera->vec.z += cosf(pCamera->rot.y);
		}
		if (GetKeyboardPress(DIK_A))
		{
			pCamera->vec.x += sinf(D3DX_PI * -0.5f + pCamera->rot.y);
			pCamera->vec.z += cosf(D3DX_PI * -0.5f + pCamera->rot.y);
		}
		if (GetKeyboardPress(DIK_S))
		{
			pCamera->vec.x += sinf(D3DX_PI + pCamera->rot.y);
			pCamera->vec.z += cosf(D3DX_PI + pCamera->rot.y);
		}
		if (GetKeyboardPress(DIK_D))
		{
			pCamera->vec.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y);
			pCamera->vec.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y);
		}

		// 注視点角度の回転
		if (GetKeyboardPress(DIK_Z))
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
		if (GetKeyboardPress(DIK_C))
		{
			pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}

		// 視点角度の回転
		if (GetKeyboardPress(DIK_Q))
		{
			pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
		}
		if (GetKeyboardPress(DIK_E))
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
		}
		if (GetKeyboardPress(DIK_R))
		{
			pCamera->rot.x += -(CAMERA_ROT_VOLUME);	// 回転量
		}
		if (GetKeyboardPress(DIK_V))
		{
			pCamera->rot.x += CAMERA_ROT_VOLUME;	// 回転量
		}
	}

	D3DXVec3Normalize(&pCamera->vec, &pCamera->vec);	// 正規化する(大きさ１のベクトルにする)
	pCamera->vec *= CAMERA_MOVE;
	pCamera->posV += pCamera->vec;

	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + tanf(-pCamera->rot.x + (D3DX_PI * 0.5f)) * pCamera->fDistance;
}

//=========================================
// カメラの情報取得
//=========================================
Camera* GetCamera(void)
{
	return &(s_camera);
}

//=========================================
// カメラの角度情報取得
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera.rot;
}