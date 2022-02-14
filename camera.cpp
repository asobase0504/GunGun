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
static Camera s_camera[2];	// カメラ情報

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
	s_camera[0].posV = D3DXVECTOR3(0.0f,50.0f,-60.0f);	// 視点
	s_camera[0].posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 注視点
	s_camera[0].vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// 上方向ベクトル
	s_camera[0].rot = ZERO_VECTOR;	// 向き

	s_camera[0].rot.y = atan2f((s_camera[0].posR.x - s_camera[0].posV.x), (s_camera[0].posR.z - s_camera[0].posV.z));
	s_camera[0].rot.x = atan2f((s_camera[0].posR.z - s_camera[0].posV.z), (s_camera[0].posR.y - s_camera[0].posV.y));

	D3DXVECTOR3 vec = s_camera[0].posV - s_camera[0].posR;
	s_camera[0].fDistance = D3DXVec3Length(&vec);

	s_camera[0].viewport.X = (DWORD)0.0f;
	s_camera[0].viewport.Y = (DWORD)0.0f;
	s_camera[0].viewport.Width = SCREEN_WIDTH;
	s_camera[0].viewport.Height = SCREEN_HEIGHT;
	s_camera[0].viewport.MinZ = 0.0f;
	s_camera[0].viewport.MaxZ = 1.0f;

	s_camera[1].posV = D3DXVECTOR3(0.0f, -200.0f, -60.0f);	// 視点
	s_camera[1].posR = D3DXVECTOR3(0.0f, -200.0f, 0.0f);	// 注視点
	s_camera[1].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	s_camera[1].rot = ZERO_VECTOR;	// 向き
			 
	s_camera[1].rot.y = atan2f((s_camera[1].posR.x - s_camera[1].posV.x), (s_camera[1].posR.z - s_camera[1].posV.z));
	s_camera[1].rot.x = atan2f((s_camera[1].posR.z - s_camera[1].posV.z), (s_camera[1].posR.y - s_camera[1].posV.y));

	vec = s_camera[1].posV - s_camera[1].posR;
	s_camera[1].fDistance = D3DXVec3Length(&vec);

	s_camera[1].viewport.X = (DWORD)0.0f;
	s_camera[1].viewport.Y = (DWORD)(SCREEN_HEIGHT - (80.0f * 4.0f));
	s_camera[1].viewport.Width = (DWORD)(80.0f * 4.0f);
	s_camera[1].viewport.Height = (DWORD)(80.0f * 2.25f);
	s_camera[1].viewport.MinZ = 0.0f;
	s_camera[1].viewport.MaxZ = 1.0f;

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
	s_camera[0].posV = D3DXVECTOR3(0.0f, 100.0f, -30.0f);	// 視点
	s_camera[0].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点
	s_camera[0].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトル

	D3DXVECTOR3 vec = s_camera[0].posV - s_camera[0].posR;
	s_camera[1].fDistance = D3DXVec3Length(&vec);
}

//=========================================
// ゲーム中の更新
//=========================================
void UpdateGameCamera(void)
{
	Camera* pCamera = &(s_camera[0]);

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
	pCamera->posR = player->pos;

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

#ifdef _DEBUG

	if (GetKeyboardPress(DIK_9))
	{
		pCamera->fDistance++;
	}
	if (GetKeyboardPress(DIK_0))
	{
		pCamera->fDistance--;
	}

#endif // !_DEBUG
}

//=========================================
// リザルト中の更新
//=========================================
void UpdateResultCamera(void)
{
	Camera* pCamera = &(s_camera[0]);

	Player *player = GetPlayer();

	// カメラの追従処理
	pCamera->posR.x += (player->pos.x - pCamera->posR.x) * 0.05f;
	pCamera->posR.z += (player->pos.z - pCamera->posR.z) * 0.05f;

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

	pCamera->posR.y = player->pos.y;

	pCamera->fDistance = 80.0f + 1.5f * player->fLength;

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
void SetCamera(int nData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera[nData]);

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
		10.0f,6200.0f);	// どこから(ニア)(第５引数)どこまで(ファー)(第６引数)をカメラで表示するか設定 

	//D3DXMatrixOrthoLH(&camara->mtxProjection, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 10.0f, 400.0f * camara->fDistance / 60.0f);
	
	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// 入力
//=========================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera[0]);

	pCamera->vec = ZERO_VECTOR;

	if (IsJoyPadUse(0))
	{// ジョイパッドの使用
		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x > 0.5f)
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME * GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x;	// 回転量
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
		if (GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x < -0.5f)
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME * GetJoypadStick(JOYKEY_RIGHT_STICK, 0).x;	// 回転量
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
	}
	else
	{
		// 視点角度の回転
		if (GetKeyboardPress(DIK_Q))
		{
			pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
		}
		if (GetKeyboardPress(DIK_E))
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
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
Camera* GetCamera(int nData)
{
	return &s_camera[nData];
}

//=========================================
// カメラの角度情報取得
//=========================================
D3DXVECTOR3* GetRotCamera(void)
{
	return &s_camera[0].rot;
}