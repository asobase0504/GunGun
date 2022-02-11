//============================
//
// アイテム設定
// Author: Yuda Kaito
//
//============================

//------------------------------------
// include
//------------------------------------
#include <stdio.h>
#include "item.h"
#include "player.h"
#include "model.h"
#include "input.h"
#include "common.h"
#include "camera.h"

//------------------------------------
// マクロ
//------------------------------------
#define Attenuation	(0.5f)		//減衰係数
#define Speed	(1.0f)			//スピード
#define WIDTH (10.0f)			//モデルの半径
#define Vtx (4)					//頂点数

//------------------------------------
// static変数
//------------------------------------
static bool s_bIsLanding;
static Model s_Model;			// ポリゴンの構造体
static int s_nSetModelType;		// 設定するモデルタイプ

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void MoveItem(void);
static void SelectItem(void);

//=========================================
// 初期化処理
//=========================================
void InitItem(void)
{
	s_nSetModelType = 0;
	ZeroMemory(&s_Model, sizeof(s_Model));

	s_Model = *GetPlayerModel()[0];
}

//=========================================
// 終了処理
//=========================================
void UninitItem(void)
{
	Model* model = &s_Model;

	if (model->pTexture != NULL)
	{
		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			if (model->pTexture[j] != NULL)
			{// テクスチャの解放
				model->pTexture[j]->Release();
				model->pTexture[j] = NULL;
			}
		}

		delete[]model->pTexture;
		model->pTexture = NULL;
	}

	// メッシュの解放
	if (model->pMesh != NULL)
	{
		model->pMesh->Release();
		model->pMesh = NULL;
	}
	// マテリアルの解放
	if (model->pBuffMat != NULL)
	{
		model->pBuffMat->Release();
		model->pBuffMat = NULL;
	}
}

//=========================================
// 更新処理
//=========================================
void UpdateItem(void)
{
	Model* model = &s_Model;

	model->pos_old = model->pos;
	model->pos_world = D3DXVECTOR3(model->mtxWorld._41, model->mtxWorld._42, model->mtxWorld._43);

	// 移動処理
	MoveItem();

	// プレイヤーと床の当たり判定
	//CollisionMeshField(&pPlayer->pos);

	// 角度の正規化
	NormalizeRot(&model->rot.y);

	// モデルを選ぶ処理
	SelectItem();
}

//=========================================
// 描画処理
//=========================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	Model* model = &s_Model;

	if (!model->bUse)
	{
		return;
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&model->mtxWorld);

	// スケールの反映
	D3DXMatrixScaling(&mtxScale, model->scale, model->scale, model->scale);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScale);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
																						// 角度の反映
	if (model->isQuaternion)
	{
		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &model->quaternion);			// クオータニオンによる行列回転
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	}
	else
	{
		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	}

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

	for (int j = 0; j < (int)model->nNumMat; j++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[j].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, model->pTexture[j]);

		// モデルパーツの描写
		model->pMesh->DrawSubset(j);
	}
	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 移動処理
//=========================================
static void MoveItem(void)
{
	D3DXVECTOR2 moveInput;
	D3DXVECTOR3 move = ZERO_VECTOR;			// 移動量の初期化
	float moveLength = 0.0f;

	if (IsJoyPadUse(0))
	{// ジョイパッドの使用
		moveInput.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
		moveInput.y = -GetJoypadStick(JOYKEY_LEFT_STICK, 0).y;

		if (moveInput.x != 0.0f || moveInput.y != 0.0f)
		{
			moveLength = D3DXVec2Length(&moveInput);

			if (moveLength > 1.0f)
			{
				moveLength = 1.0f;
			}
		}
	}
	else
	{
		moveInput.x = 0.0f;
		moveInput.y = 0.0f;

		// モデルの移動
		if (GetKeyboardPress(DIK_UP))
		{
			moveInput.y += 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_LEFT))
		{
			moveInput.x -= 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_DOWN))
		{
			moveInput.y -= 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_RIGHT))
		{
			moveInput.x += 1.0f;
			moveLength = 1.0f;
		}
	}

	if (moveLength > 0.0f)
	{
		// カメラの角度情報取得
		D3DXVECTOR3* CameraRot = GetRotCamera();

		D3DXVec2Normalize(&moveInput, &moveInput);

		float c = cosf(-CameraRot->y);
		float s = sinf(-CameraRot->y);

		// move の長さは 1 になる。
		move.x = moveInput.x * c - moveInput.y * s;
		move.z = moveInput.x * s + moveInput.y * c;
	}
	else
	{ // 入力されていない。
		return;
	}

	Model* model = &s_Model;

	// 方向ベクトル掛ける移動量
	model->pos += move * moveLength;

	if (model->pos.y - model->fLength <= 0.0f)
	{
		model->pos.y = model->fLength;
	}
}

//=========================================
// 選択処理
//=========================================
static void SelectItem(void)
{
	Model* model = &s_Model;

	if (GetJoypadTrigger(JOYKEY_A))
	{
		//model->nType = s_nSetModelType;
		//model->nIdxModelParent = -2;
		//model->quaternion = ZERO_QUATERNION;
		//model->isQuaternion = true;
		//model->bUse = true;
		//model->pos = model->pos;
	}
	if (GetJoypadTrigger(JOYKEY_RIGHT_SHOULDER))
	{
		if (s_nSetModelType <= 100)
		{
			s_Model = GetModel()[s_nSetModelType];
			s_nSetModelType++;
		}
	}
	if (GetJoypadTrigger(JOYKEY_LEFT_SHOULDER))
	{
		if (s_nSetModelType >= 1)
		{
			s_Model = GetModel()[s_nSetModelType];
			s_nSetModelType--;
		}
	}
}
